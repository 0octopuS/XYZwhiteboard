#include "../base/exception.hpp"
#include "../base/packet.hpp" // Include your WhiteboardPacket class
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <cstdint>
#include <queue>
using namespace std;

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

class WhiteboardClient {
private:
  uint32_t version = 1;
  uint32_t user_id;
  string whiteboard_id;
  uint32_t packet_counter = 0;
  boost::asio::io_context io_context;
  tcp::resolver resolver;
  ssl::context ctx;
  ssl::stream<boost::asio::ip::tcp::socket> socket;
  boost::asio::deadline_timer deadline_;
  boost::asio::deadline_timer heartbeat_timer_;
  // boost::asio::streambuf receive_buffer;
  bool connected_;
  // char receive_buffer_[1024];
  std::unordered_map<uint32_t, WhiteboardPacketType> send_queue;
  boost::asio::streambuf input_buffer_;
  boost::asio::ip::tcp::resolver::iterator iter;

public:
  std::queue<WhiteboardPacket> received_queue;
  WhiteboardClient(const std::string &server_ip, unsigned short port)
      : io_context(), resolver(io_context), ctx(ssl::context::sslv23_client),
        socket(io_context, ctx), deadline_(io_context),
        heartbeat_timer_(io_context), connected_(true), send_queue() {
#ifndef NDEBUG
    DEBUG_MSG;
#endif
    iter = resolver.resolve(server_ip, std::to_string(port));
    ctx.set_options(boost::asio::ssl::context::default_workarounds |
                    boost::asio::ssl::context::single_dh_use);
    boost::system::error_code error;
    // boost::asio::connect(socket, iter, error);
    boost::asio::connect(socket.next_layer(), iter);
    socket.handshake(ssl::stream_base::client);
    if (error) {
      // Handle connection error
      std::cerr << "Error connecting "
                   "to server: "
                << error.message() << std::endl;
    }
  }

  ~WhiteboardClient() {
    close(); // Gracefully close connection in destructor
  }

  void send_packet(const WhiteboardPacket &packet);
  void send_create_whiteboard_request();
  void send_create_session_request();
  void send_join_session_request(string _whiteboard_id);
  void send_quit_session_request();
  void send_add_element_request(WhiteboardElements _ele);
  void send_modify_element_request(WhiteboardElements _ele);
  void send_delete_element_request(WhiteboardElements _ele);
  void send_login_request(string username, string password);
  void send_register_request(string username, string password);
  void handle_write(const boost::system::error_code &ec);
  bool handle_receive();
  void handle_receive_async();
  void handle_single_packet(const boost::system::error_code &error);
  void handle_connect(const boost::system::error_code &error);
  protobuf::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);
  void handle_action_response(const protobuf::whiteboardPacket &response);
  void handle_temp_id_response(const protobuf::whiteboardPacket &response);
  vector<WhiteboardElements>
  handle_broadcast(const protobuf::whiteboardPacket &response);
  // void start(const std::string &server_ip, unsigned short port);
  void check_deadline();
  void start_connect(tcp::resolver::iterator endpoint_iter);
  void handle_connect(const boost::system::error_code &ec,
                      tcp::resolver::iterator endpoint_iter);
  void start();
  void close();
};
