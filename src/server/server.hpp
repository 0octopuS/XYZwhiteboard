#include "../base/packet.hpp"
#include "mongocxx/uri.hpp"
#include "packet.pb.h"
#include "thread_pool.hpp"
#include <boost/asio.hpp>
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
using boost::asio::ip::tcp;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class WhiteboardServer {
private:
  uint32_t version = 1;
  boost::asio::io_context io_context;
  tcp::acceptor acceptor;
  ThreadPool thread_pool{5};

  // mongocxx::uri uri;
  mongocxx::instance instance{};
  mongocxx::client mongoclient{mongocxx::uri{}};
  mongocxx::v_noabi::database whiteboard_db;
  mongocxx::v_noabi::collection whiteboard_collection;
  mongocxx::v_noabi::collection user_collection;

  void handle_connection(tcp::socket socket);
  void accept_connections();

  protobuf::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);

  // Different Methods for Example
  uint32_t handle_assign_user_id(tcp::socket &tcp_socket);
  void handle_create_whiteboard_request(
      const protobuf::CreateWhiteBoardRequest &request, tcp::socket &socket);

  void
  handle_create_session_request(const protobuf::CreateSessionRequest &request);

  void send_packet(tcp::socket &tcp_socket, const WhiteboardPacket &packet);

public:
  WhiteboardServer(unsigned short port)
      : acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {

    whiteboard_db = mongoclient["whiteboards"];
    whiteboard_collection = whiteboard_db["whiteboards"];
    user_collection = whiteboard_db["users"];
  }
  ~WhiteboardServer() {}
  void start() {
    accept_connections();
    io_context.run();
  }
  // void recv_packet() {}
};