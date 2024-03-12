#include "client.hpp"
#include "google/protobuf/io/coded_stream.h"
#include <boost/asio/read_until.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>
#include <thread>

// void WhiteboardClient::start(const std::string &server_ip,
//                              unsigned short port) {
// #ifndef NDEBUG
//   DEBUG_MSG;
// #endif

//   tcp::resolver resolver(io_context);
//   tcp::resolver::results_type endpoints =
//       resolver.resolve(server_ip, std::to_string(port));
//   boost::asio::ip::tcp::resolver::query query(server_ip,
//   std::to_string(port)); boost::asio::ip::tcp::resolver::iterator iter =
//   resolver.resolve(query); boost::system::error_code error;
//   // boost::asio::async_connect(socket, iter,
//   // boost::bind(&WhiteboardClient::handle_connect,
//   //                                        this,
//   // boost::asio::placeholders::error)); boost::asio::connect(socket,
//   resolver.resolve(query), error); if (!error) {
//     // Start asynchronous receive operation
//     std::lock_guard<std::mutex> lock(connected_mutex_);
//     connected_ = true;
//     connected_cv_.notify_all(); // start_receive();
//     printf("Connected, notify.\n");
//   } else {
//     // Handle connection error
//     std::cerr << "Error connecting to server: " << error.message() <<
//     std::endl;
//   }
// }

void WhiteboardClient::handle_connect(const boost::system::error_code &error) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  if (!error) {
    // Start asynchronous receive operation
    {
      std::lock_guard<std::mutex> lock(connected_mutex_);
      connected_ = true;
    }
    connected_cv_.notify_all(); // start_receive();
    printf("Connected, notify.\n");
  } else {
    // Handle connection error
    std::cerr << "Error connecting to server: " << error.message() << std::endl;
  }
}

void WhiteboardClient::start_receive() {
  std::unique_lock<std::mutex> lock(connected_mutex_);

  connected_cv_.wait(
      lock, [this] { return connected_; }); // Wait for connection establishment

#ifndef NDEBUG
  DEBUG_MSG;
#endif
  while (connected_) {
    // boost::asio::async_read_until(
    //     socket, receive_buffer, '\n',
    //     boost::bind(&WhiteboardClient::handle_receive, this,
    //                 boost::asio::placeholders::error,
    //                 boost::asio::placeholders::bytes_transferred));
    boost::asio::streambuf buffer;
    boost::system::error_code ec;

    boost::asio::read_until(socket, buffer, '\n', ec);
    if (ec == boost::asio::error::eof) {
      // Client disconnected
      std::cout << "Server disconnected\n";
      break;
    } else if (ec) {
      // Error reading from socket
      std::cout << "System error\n";
      throw boost::system::system_error(ec);
    }
    try {
      protobuf::whiteboardPacket packet = parse_packet(&buffer);
      switch (static_cast<WhiteboardPacketType>(packet.packet_type())) {
      default:
        std::cout << "Unknown packet type\n";
        throw ClientReceivedWrongPacketType();
        break;
      }
    } catch (const std::exception &e) {
      std::cerr << "Exception in handle_response: " << e.what() << std::endl;
    }
  }
}

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

  printf("1\n");

  // protobuf::whiteboardPacket packet;
  // Read from socket
  // Process received data
  protobuf::whiteboardPacket received_packet = parse_packet(&buffer);
  printf("1\n");

  switch (static_cast<WhiteboardPacketType>(received_packet.packet_type())) {
  default:
    std::cout << "Unknown packet type\n";
    throw ClientReceivedWrongPacketType();
    break;
  }
  return true;
}

void WhiteboardClient::handle_send() {
  std::unique_lock<std::mutex> lock(connected_mutex_);
  connected_cv_.wait(lock, [this] { return connected_; });

#ifndef NDEBUG
  DEBUG_MSG;
#endif
  while (connected_) {
    if (!send_queue_.empty()) {
      // Dequeue a packet and send it to the server
      boost::asio::streambuf request;
      auto packet = send_queue_.front();
      send_queue_.pop();
      std::ostream os(&request);
      {
        ::google::protobuf::io::OstreamOutputStream raw_output_stream(&os);
        ::google::protobuf::io::CodedOutputStream coded_output_stream(
            &raw_output_stream);

        packet.serialize(&coded_output_stream);
      }
      boost::asio::write(socket, request);
    }
    // std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
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
  {
    std::lock_guard<std::mutex> lock(connected_mutex_);
    connected_ = false;
    connected_cv_.notify_all();
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
#ifndef NDEBUG
  std::cout << "| Received data: " << messageSize << std::endl;
#endif
  if (!packet.ParseFromCodedStream(&code_input_stream)) {
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
