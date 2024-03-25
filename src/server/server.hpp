#include "../base/packet.hpp"
#include "action.pb.h"
#include "mongocxx/uri.hpp"
#include "packet.pb.h"
#include "thread_pool.hpp"
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <thread>
#include <unordered_map>
#include <vector>
using boost::asio::ip::tcp;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
namespace ssl = boost::asio::ssl;
class WhiteboardServer {
private:
  uint32_t version = 1;
  boost::asio::io_context io_context;
  boost::asio::ssl::context ssl_context;

  tcp::acceptor acceptor;
  ThreadPool thread_pool{5};

  // mongocxx::uri uri;
  mongocxx::instance instance{};
  mongocxx::client mongoclient{mongocxx::uri{}};
  mongocxx::v_noabi::database whiteboard_db;
  mongocxx::v_noabi::collection whiteboard_collection;
  mongocxx::v_noabi::collection user_collection;
  std::unordered_map<std::string, std::set<ssl::stream<tcp::socket> *>>
      whiteboard_sessions; // Mapping of whiteboard ID to connected sockets
  std::mutex map_mutex;
  void add_socket_to_whiteboard(const std::string &whiteboard_id,
                                ssl::stream<tcp::socket> *tcp_socket) {
    std::lock_guard<std::mutex> lock(
        map_mutex); // Lock the mutex to ensure thread safety
    whiteboard_sessions[whiteboard_id].insert(tcp_socket);
  }

  void remove_socket_from_whiteboard(const std::string &whiteboard_id,
                                     ssl::stream<tcp::socket> *tcp_socket) {
    auto it = whiteboard_sessions.find(whiteboard_id);
    if (it != whiteboard_sessions.end()) {
      auto &socket_set = it->second;
      socket_set.erase(tcp_socket);
      if (socket_set.empty()) {
        whiteboard_sessions.erase(it);
      }
    }
  }

  bool is_whiteboard_shared(const std::string &whiteboard_id) {
    std::lock_guard<std::mutex> lock(
        map_mutex); // Lock the mutex to ensure thread safety
    return whiteboard_sessions.count(whiteboard_id) > 0;
  }

  bool user_in_active_user_ids(const bsoncxx::document::view &whiteboard_doc,
                               int user_id) {
    auto active_user_ids_field = whiteboard_doc["active_user_ids"];
    if (active_user_ids_field) {
      auto active_user_ids = active_user_ids_field.get_array().value;
      for (const auto &id : active_user_ids) {
        if (id.get_int32().value == user_id) {
          return true;
        }
      }
    }
    return false;
  }

  void handle_connection(tcp::socket socket);
  void handle_connection(boost::asio::ssl::stream<tcp::socket> ssl_socket);
  void accept_connections();

  protobuf::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);

  // Different Methods for Example
  void
  handle_create_whiteboard_request(const protobuf::whiteboardPacket &packet,
                                   ssl::stream<tcp::socket> &tcp_socket);

  void handle_create_session_request(const protobuf::whiteboardPacket &packet,
                                     ssl::stream<tcp::socket> &tcp_socket);

  void handle_join_session_request(const protobuf::whiteboardPacket &packet,
                                   ssl::stream<tcp::socket> &tcp_socket);

  void handle_quit_session_request(const protobuf::whiteboardPacket &packet,
                                   ssl::stream<tcp::socket> &tcp_socket);

  void handle_add_element_request(const protobuf::whiteboardPacket &packet,
                                  ssl::stream<tcp::socket> &tcp_socket);
  void handle_modify_element_request(const protobuf::whiteboardPacket &packet,
                                     ssl::stream<tcp::socket> &tcp_socket);
  void handle_login_request(const protobuf::whiteboardPacket &packet,
                            ssl::stream<tcp::socket> &tcp_socket);
  void handle_register_request(const protobuf::whiteboardPacket &packet,
                               ssl::stream<tcp::socket> &tcp_socket);

  uint32_t handle_assign_user_id(ssl::stream<tcp::socket> &tcp_socket);
  void handle_broadcast(std::string whiteboard_id);
  // General method for send packet
  void handle_unicast(const std::string &whiteboard_id,
                      ssl::stream<tcp::socket> &tcp_socket);
  void send_packet(boost::asio::ssl::stream<tcp::socket> &ssl_socket,
                   const WhiteboardPacket &packet);

public:
  WhiteboardServer(unsigned short port)
      : io_context(), ssl_context(boost::asio::ssl::context::sslv23_server),
        acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {

    whiteboard_db = mongoclient["whiteboards"];
    whiteboard_collection = whiteboard_db["whiteboards"];
    user_collection = whiteboard_db["users"];
    ssl_context.set_options(boost::asio::ssl::context::default_workarounds |

                            boost::asio::ssl::context::single_dh_use);
    ssl_context.use_certificate_file("/etc/ssl/certs/cert.pem",
                                     boost::asio::ssl::context::pem);
    ssl_context.use_private_key_file("/etc/ssl/private/key.pem",
                                     boost::asio::ssl::context::pem);
  }
  ~WhiteboardServer() {}
  void start() {
    accept_connections();
    io_context.run();
  }
  // void recv_packet() {}
};