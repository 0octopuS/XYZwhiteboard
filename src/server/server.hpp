#include "../base/packet.hpp"
#include "packet.pb.h"
#include "thread_pool.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
using boost::asio::ip::tcp;

class WhiteboardServer {
private:
  boost::asio::io_context io_context;
  tcp::acceptor acceptor;
  ThreadPool thread_pool{5};
  void handle_connection(tcp::socket socket);
  void accept_connections();

  whiteboard::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);

  uint32_t handle_assign_user_id();
  void handle_create_whiteboard_request(
      const whiteboard::CreateWhiteBoardRequest &request, tcp::socket &socket);

  void handle_create_session_request(
      const whiteboard::CreateSessionRequest &request);
  void send_response(whiteboard::whiteboardPacket &response,
                     tcp::socket &tcp_socke, std::string messaget);

public:
  WhiteboardServer(unsigned short port)
      : acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {}
  void start() {
    accept_connections();
    io_context.run();
  }
  // void recv_packet() {}
};