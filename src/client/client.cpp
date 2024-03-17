#include "client.hpp"
#include "google/protobuf/io/coded_stream.h"
#include <boost/asio/read_until.hpp>
#include <cstdint>
#include <cstdio>
#include <exception>
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
    handle_action_response(received_packet);
    break;
  }
  case WhiteboardPacketType::tempIdResponse: {
    handle_temp_id_response(received_packet);
    break;
  }
  default:
    std::cout << "Unknown packet type\n";
    throw ClientReceivedWrongPacketType();
    break;
  }

  received_queue.emplace(WhiteboardPacket(received_packet));
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
  WhiteboardPacket packet(version, WhiteboardPacketType::createWhiteboard,
                          ++packet_counter);
  packet.new_create_whiteboard_request(user_id);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
  // send_queue_.push(packet);
#ifndef NDEBUG
  printf(">>> send_queue len %zu\n", send_queue.size());
#endif
}

void WhiteboardClient::send_create_session_request() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  WhiteboardPacket packet(version, WhiteboardPacketType::createSession,
                          ++packet_counter);
  packet.new_create_session_request(user_id, whiteboard_id);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
  // send_queue_.push(packet);
#ifndef NDEBUG
  printf(">>> send_queue len %zu\n", send_queue.size());
#endif
}

void WhiteboardClient::send_join_session_request() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif

  WhiteboardPacket packet(version, WhiteboardPacketType::joinSession,
                          ++packet_counter);
  packet.new_join_session_request(user_id, whiteboard_id);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
  // send_queue_.push(packet);
#ifndef NDEBUG
  printf(">>> send_queue len %zu\n", send_queue.size());
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
  if (!code_input_stream.ReadRaw(const_cast<char *>(serializedData.data()),
                                 messageSize)) {
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

  // Before sending the packet, client add this packet to its sending queue
  send_queue[packet.get_packet_id()] = packet.get_packet_type();
#ifndef NDEBUG
  std::cout << ">>>  Send a packet of type "
            << static_cast<uint32_t>(packet.get_packet_type()) << ", id "
            << packet.get_packet_id() << "\n";
#endif
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
  WhiteboardPacket packet(version, WhiteboardPacketType::addElement,
                          ++packet_counter);
  packet.new_add_element_request(user_id, whiteboard_id, _ele);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
}

void WhiteboardClient::handle_action_response(
    const protobuf::whiteboardPacket &packet) {
  auto response = packet.action().actionresponse();
#ifndef NDEBUG
  std::cout << "Received action response: "
            << (response.success() ? "success " : "fail ") << response.message()
            << std::endl;
#endif
  auto packet_id = packet.packet_id();
  switch (send_queue[packet_id]) {
  case WhiteboardPacketType::createWhiteboard: {
    // In case of createWhiteboard, the return value should be a whiteboard id;
    if (response.success()) {
      whiteboard_id = response.message();
    } else {
      throw ClientReceiveServerSideFail("Server fail to create a whiteboard");
    }
#ifndef NDEBUG
    std::cout << "Whiteboard_id recorded" << whiteboard_id << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::createSession: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to create a session");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::joinSession: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to join a session");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::quitSession: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to quit a session");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::addElement:
  case WhiteboardPacketType::modifyElement:
  case WhiteboardPacketType::deleteElement:
  case WhiteboardPacketType::saveWhiteboard:
    break;
  default:
    printf("Received not validate actionResponse with packet id %d type %d\n",
           packet_id, static_cast<uint32_t>(send_queue[packet_id]));
    break;
  }
}

void WhiteboardClient::handle_temp_id_response(
    const protobuf::whiteboardPacket &packet) {
  auto response = packet.action().tempidresponse();
  bool success = response.success();
  uint32_t uid = response.user_id();
#ifndef NDEBUG
  std::cout << ">>> GET user id " << uid << '\n';
#endif
  if (success) {
    user_id = uid;
  } else {
    throw ClientReceiveServerSideFail("Server fail to create user_id");
  }
}