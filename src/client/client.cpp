#include "client.hpp"
#include "google/protobuf/io/coded_stream.h"
#include <boost/asio/read_until.hpp>
#include <cstdio>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>

bool WhiteboardClient::handle_receive() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  boost::asio::streambuf buffer;
  boost::system::error_code ec;

  size_t byte = boost::asio::read_until(socket, buffer, '\n', ec);

  // size_t byte = boost::asio::read(socket, receive_buffer,
  //                                 boost::asio::transfer_all(), ec);
  if (ec == boost::asio::error::eof) {
    std::cout << "Server disconnected\n";
    return false;
  } else if (ec) {
    throw boost::system::system_error(ec);
  }
  if (byte == 0)
    return false;

  // protobuf::whiteboardPacket packet;
  // Read from socket
  // Process received data
  protobuf::whiteboardPacket received_packet = parse_packet(&buffer);

  switch (static_cast<WhiteboardPacketType>(received_packet.packet_type())) {
  case WhiteboardPacketType::actionResponse: {
    handle_action_response(received_packet.action().actionresponse());
    break;
  }
  case WhiteboardPacketType::tempIdResponse: {
    handle_temp_id_response(received_packet.action().tempidresponse());
    break;
  }
  default:
    std::cout << "Unknown packet type\n";
    throw ClientReceivedWrongPacketType();
    break;
  }
  return true;
}

void WhiteboardClient::close() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  if (connected_) {
    // Gracefully close the connection
    boost::asio::socket_base::shutdown_type shutdown_type =
        boost::asio::socket_base::shutdown_both;
    io_context.post([this, shutdown_type]() {
      boost::system::error_code ec;
      ec = socket.shutdown(shutdown_type, ec);
      if (!ec) {
        socket.close();
        std::cout << "Connection closed successfully." << std::endl;
      } else {
        std::cerr << "Error closing connection: " << ec.message() << std::endl;
      }
    });
  }
}
void WhiteboardClient::send_create_whiteboard_request() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  WhiteboardPacket packet(version);
  packet.new_create_whiteboard_request(user_id);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
  // send_queue_.push(packet);
#ifndef NDEBUG
  printf(">>> send_queue len %zu\n", send_queue_.size());
#endif
}

protobuf::whiteboardPacket
WhiteboardClient::parse_packet(boost::asio::streambuf *buffer) {
  protobuf::whiteboardPacket packet;
  std::istream is(buffer);
  google::protobuf::io::IstreamInputStream istream_input_stream(&is);
  google::protobuf::io::CodedInputStream code_input_stream(
      &istream_input_stream);
  uint32_t messageSize;
  code_input_stream.ReadVarint32(&messageSize);
  std::string serializedData;
  serializedData.resize(messageSize);
  if (!code_input_stream.ReadRaw(serializedData.data(), messageSize)) {
    std::cout << "Failed to read serialized protobuf data" << std::endl;
  }
#ifndef NDEBUG
  std::cout << "| Received data: " << messageSize << std::endl;
#endif
  if (!packet.ParseFromString(serializedData))
  // if (!packet.ParseFromCodedStream(&code_input_stream))
  {
    std::cout << "| Failed to parse packet\n";
  } else {
    std::cout << "| Parse success\n";
  }
  return packet;
}

void WhiteboardClient::send_packet(const WhiteboardPacket &packet) {
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

  size_t byte = boost::asio::write(socket, request);
  // send_queue_.push(request);
#ifndef NDEBUG
  DEBUG_MSG;
  printf(">>> send_packet: %zu\n", byte);
  printf(">>> send packet\n");
  packet.print();
#endif
}

// void WhiteboardClient::send_create_whiteboard_request

void WhiteboardClient::send_add_element_request(WhiteboardElements _ele) {
  WhiteboardPacket packet(version);
  packet.new_add_element_request(_ele);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
}

void WhiteboardClient::handle_action_response(
    const protobuf::ActionResponse &request) {
  std::cout << "Received action response: "
            << (request.success() ? "success" : "fail") << request.message()
            << std::endl;
}

void WhiteboardClient::handle_temp_id_response(
    const protobuf::TempIDResponse &response) {
  bool success = response.success();
  uint32_t uid = response.user_id();
#ifndef NDEBUG
  std::cout << ">>> GET user id " << uid << '\n';
#endif
  if (success) {
    user_id = uid;
  }
}