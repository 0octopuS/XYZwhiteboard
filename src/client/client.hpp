#include "../base/packet.hpp" // Include your WhiteboardPacket class
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include <boost/asio.hpp>
#include <cstdio>
#include <iostream>
using namespace std;

using boost::asio::ip::tcp;

class WhiteboardClient {
private:
  uint32_t version = 1;
  uint32_t user_id = 0;
  boost::asio::io_context io_context;
  tcp::socket socket;

public:
  WhiteboardClient(const std::string &server_ip, unsigned short port)
      : socket(io_context) {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
        resolver.resolve(server_ip, std::to_string(port));
    boost::asio::connect(socket, endpoints);
  }

  void send_packet(const WhiteboardPacket &packet) {
    // Serialize WhiteboardPacket to a buffer
    // std::ostringstream oss;
    boost::asio::streambuf request;
    std::ostream os(&request);
    {
      ::google::protobuf::io::OstreamOutputStream raw_output_stream(&os);
      ::google::protobuf::io::CodedOutputStream coded_output_stream(
          &raw_output_stream);

      packet.serialize(&coded_output_stream);
    }

    // serialize the WhiteboardPacket instance into the buffer
    // oss << packet.serialize();
    // Send the buffer over the socket
    // std::string data = oss.str();
    // size_t len = socket.se()(request.data(), endpoint, 0, error);

    size_t byte = boost::asio::write(socket, request);
#ifndef NDEBUG
    printf(">>> send_packet: %zu\n", byte);
    printf(">>> send packet type");
    packet.print();
#endif
  }
  void send_create_whiteboard_request();
  void send_add_element_request();
  void handle_response();
  void close() { socket.close(); }
};
