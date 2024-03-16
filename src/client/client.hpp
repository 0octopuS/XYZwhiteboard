#include "../base/exception.hpp"
#include "../base/packet.hpp" // Include your WhiteboardPacket class
#include "action.pb.h"
#include <boost/asio.hpp>
#include <queue>
using namespace std;

using boost::asio::ip::tcp;

class WhiteboardClient {
private:
  uint32_t version = 1;
  uint32_t user_id = 0;
  boost::asio::io_context io_context;
  tcp::resolver resolver;
  tcp::socket socket;
  // boost::asio::streambuf receive_buffer;
  bool connected_;
  // char receive_buffer_[1024];
  std::queue<WhiteboardPacket> send_queue_;
  boost::asio::ip::tcp::resolver::iterator iter;

public:
  WhiteboardClient(const std::string &server_ip, unsigned short port)
      : io_context(), resolver(io_context), socket(io_context),
        connected_(false) {
#ifndef NDEBUG
    DEBUG_MSG;
#endif
    iter = resolver.resolve(server_ip, std::to_string(port));

    boost::system::error_code error;
    boost::asio::connect(socket, iter, error);
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
  void send_join_session_request() {}
  void send_add_element_request(WhiteboardElements _ele);
  bool handle_receive();
  void handle_connect(const boost::system::error_code &error);
  protobuf::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);
  void handle_action_response(const protobuf::ActionResponse &response);
  void handle_temp_id_response(const protobuf::TempIDResponse &response);
  // void start(const std::string &server_ip, unsigned short port);
  void close();
};
