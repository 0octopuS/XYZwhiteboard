#include "client.hpp"
#include "../base/utils.hpp"
#include "google/protobuf/io/coded_stream.h"
#include <boost/asio/read_until.hpp>
#include <boost/bind/bind.hpp>
#include <boost/core/ref.hpp>
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
  case WhiteboardPacketType::broadcast: {
    auto elements = handle_broadcast(received_packet);
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

void WhiteboardClient::handle_receive_async() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  deadline_.expires_from_now(boost::posix_time::seconds(3));

  // boost::asio::streambuf buffer;

  boost::asio::async_read_until(
      socket, input_buffer_, '\n',
      boost::bind(&WhiteboardClient::handle_single_packet, this,
                  boost::asio::placeholders::error));
}

void WhiteboardClient::handle_single_packet(
    const boost::system::error_code &ec) {
  //, boost::asio::streambuf &buffer) {
  if (!connected_)
    return;

  if (!ec) {
    // Extract the newline-delimited message from the buffer.
    // std::string line;
    // std::istream is(&input_buffer_);
    // std::getline(is, line);
    protobuf::whiteboardPacket received_packet = parse_packet(&input_buffer_);
    switch (static_cast<WhiteboardPacketType>(received_packet.packet_type())) {
    case WhiteboardPacketType::actionResponse: {
      handle_action_response(received_packet);
      break;
    }
    case WhiteboardPacketType::tempIdResponse: {
      handle_temp_id_response(received_packet);
      break;
    }
    case WhiteboardPacketType::broadcast: {
      auto elements = handle_broadcast(received_packet);
      break;
    }
    default:
      std::cout << "Unknown packet type\n";
      throw ClientReceivedWrongPacketType();
      break;
    }

    received_queue.emplace(WhiteboardPacket(received_packet));

    // Empty messages are heartbeats and so ignored.

    handle_receive_async();
  } else {
    std::cout << "Error on receive: " << ec.message() << "\n";
  }
}

void WhiteboardClient::close() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  if (connected_) {
    connected_ = false;
    // Gracefully close the connection
    boost::asio::socket_base::shutdown_type shutdown_type =
        boost::asio::socket_base::shutdown_both;
    io_context.post([this, shutdown_type]() {
      boost::system::error_code ec;
      ec = socket.lowest_layer().shutdown(shutdown_type, ec);
      if (!ec) {
        socket.lowest_layer().close();
        std::cout << "Connection closed successfully." << std::endl;
      } else {
        std::cerr << "Error closing connection: " << ec.message() << std::endl;
      }
    });
  }
  // deadline_.cancel();
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

void WhiteboardClient::send_join_session_request(string _whiteboard_id) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif

  WhiteboardPacket packet(version, WhiteboardPacketType::joinSession,
                          ++packet_counter);
  packet.new_join_session_request(user_id, _whiteboard_id);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
  // send_queue_.push(packet);
#ifndef NDEBUG
  printf(">>> send_queue len %zu\n", send_queue.size());
#endif
}

void WhiteboardClient::send_quit_session_request() {
#ifndef NDEBUG
  DEBUG_MSG;
#endif

  WhiteboardPacket packet(version, WhiteboardPacketType::quitSession,
                          ++packet_counter);
  packet.new_quit_session_request(user_id, whiteboard_id);
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
  std::cout << ">>>  Send a packet of type "
            << static_cast<uint32_t>(packet.get_packet_type()) << ", id "
            << packet.get_packet_id() << ", length " << byte << "\n";
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

void WhiteboardClient::send_login_request(string username, string password) {
  class SHA256 sha;
  WhiteboardPacket packet(version, WhiteboardPacketType::loginRequest,
                          ++packet_counter);
  sha.update(password);
  auto password_digest = sha.digest();
  auto password_hash = sha.toString(password_digest);
  packet.new_login_request(username, password_hash);
#ifndef NDEBUG
  packet.print();
#endif
  send_packet(packet);
}

void WhiteboardClient::send_register_request(string username, string password) {
  class SHA256 sha;
  WhiteboardPacket packet(version, WhiteboardPacketType::registerRequest,
                          ++packet_counter);
  sha.update(password);
  auto password_digest = sha.digest();
  auto password_hash = sha.toString(password_digest);
  packet.new_register_request(username, password_hash);
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
    if (response.success()) {
      whiteboard_id = response.message();
    } else {
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
  case WhiteboardPacketType::addElement: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to add an element");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::modifyElement: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to modify an element");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::deleteElement: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to delete an element");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::saveWhiteboard: {
    // the return value should be just string message;
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server help you save the whiteboard");
    }
#ifndef NDEBUG
    std::cout << "<<<" << response.message() << "\n";
#endif
    break;
  }
  case WhiteboardPacketType::loginRequest: {
    if (!response.success()) {
      throw ClientReceiveServerSideFail("Server fail to login the user");
    }
    break;
  }
  case WhiteboardPacketType::registerRequest: {
    if (!response.success())
      throw ClientReceiveServerSideFail("Server fail to register the user");
    break;
  }
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

vector<WhiteboardElements>
WhiteboardClient::handle_broadcast(const protobuf::whiteboardPacket &response) {

  auto broadcast_action = response.action().broadcast();
#ifndef NDEBUG
  std::cout << "<<< Receive boradcast " << broadcast_action.elements_size()
            << "\n";
#endif
  vector<WhiteboardElements> element;
  for (auto ele : broadcast_action.elements()) {
    WhiteboardElements temp;
    temp.from_protobuf(ele);
    element.emplace_back(temp);
  }
  return element;
}

void WhiteboardClient::check_deadline() {
  if (!connected_)
    return;

  // Check whether the deadline has passed. We compare the deadline against
  // the current time since a new asynchronous operation may have moved the
  // deadline before this actor had a chance to run.
  if (deadline_.expires_at() <=
      boost::asio::deadline_timer::traits_type::now()) {
    // The deadline has passed. The socket is closed so that any outstanding
    // asynchronous operations are cancelled.
    socket.lowest_layer().close();

    // There is no longer an active deadline. The expiry is set to positive
    // infinity so that the actor takes no action until a new deadline is set.
    deadline_.expires_at(boost::posix_time::pos_infin);
  }

  // Put the actor back to sleep.
  deadline_.async_wait(boost::bind(&WhiteboardClient::check_deadline, this));
}

void WhiteboardClient::start_connect(tcp::resolver::iterator endpoint_iter) {
  if (endpoint_iter != tcp::resolver::iterator()) {
    std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

    // Set a deadline for the connect operation.
    deadline_.expires_from_now(boost::posix_time::seconds(60));

    // Start the asynchronous connect operation.
    socket.lowest_layer().async_connect(
        endpoint_iter->endpoint(),
        boost::bind(&WhiteboardClient::handle_connect, this,
                    std::placeholders::_1, endpoint_iter));
  } else {
    // There are no more endpoints to try. Shut down the client.
    close();
  }
}
void WhiteboardClient::start() {
  start_connect(iter);
  deadline_.async_wait(boost::bind(&WhiteboardClient::check_deadline, this));
  io_context.run();
}

void WhiteboardClient::handle_connect(const boost::system::error_code &ec,
                                      tcp::resolver::iterator endpoint_iter) {
  // if (!connected_)
  //   return;

  // The async_connect() function automatically opens the socket at the start
  // of the asynchronous operation. If the socket is closed at this time then
  // the timeout handler must have run first.
  if (!socket.lowest_layer().is_open()) {
    std::cout << "Connect timed out\n";

    // Try the next available endpoint.
    start_connect(++endpoint_iter);
  }

  // Check if the connect operation failed before the deadline expired.
  else if (ec) {
    std::cout << "Connect error: " << ec.message() << "\n";

    // We need to close the socket used in the previous connection attempt
    // before starting a new one.
    socket.lowest_layer().close();

    // Try the next available endpoint.
    start_connect(++endpoint_iter);
  }

  // Otherwise we have successfully established a connection.
  else {
    std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

    // Start the input actor.
    handle_receive_async();

    // Start the heartbeat actor.
    // start_write();
  }
}