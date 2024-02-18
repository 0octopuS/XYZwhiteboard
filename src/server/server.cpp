#include "server.hpp"
#include "packet.pb.h"
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <google/protobuf/io/zero_copy_stream_impl.h>

void WhiteboardServer::accept_connections() {
  try {
    while (true) {
      // Accept a new connection
      tcp::socket socket(io_context);
      acceptor.accept(socket);
#ifndef NDEBUG
      std::string sClientIp = socket.remote_endpoint().address().to_string();
      unsigned short uiClientPort = socket.remote_endpoint().port();
      std::cout << "Connecting from " << sClientIp << " " << uiClientPort
                << std::endl;
#endif
      // Enqueue connection handling task to the thread pool
      thread_pool.enqueue([this, socket_ptr = std::make_shared<tcp::socket>(
                                     std::move(socket))]() mutable {
        handle_connection(std::move(*socket_ptr));
      });
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception in accept_connections: " << e.what() << std::endl;
  }
}

whiteboard::whiteboardPacket
WhiteboardServer::parse_packet(boost::asio::streambuf *buffer) {
  whiteboard::whiteboardPacket packet;
  std::istream is(buffer);
  google::protobuf::io::IstreamInputStream istream_input_stream(&is);
  google::protobuf::io::CodedInputStream code_input_stream(
      &istream_input_stream);
  uint32_t messageSize;
  code_input_stream.ReadVarint32(&messageSize);
#ifndef NDEBUG
  std::cout << "Received data: " << messageSize << std::endl;
#endif
  if (!packet.ParseFromCodedStream(&code_input_stream)) {
    std::cout << "Failed to parse packet\n";
  } else {
    std::cout << "Parse success\n";
  }
  return packet;
}
void WhiteboardServer::handle_connection(tcp::socket socket) {
  try {
    while (true) {
      // Receive packet

      boost::asio::streambuf buffer;
      boost::system::error_code ec;
      // Read more here
      size_t bytes_transferred =
          boost::asio::read_until(socket, buffer, '\n', ec);
#ifndef NDEBUG
      std::cout << "Receive from network" << bytes_transferred << std::endl;
#endif
      if (ec == boost::asio::error::eof) {
        // Client disconnected
        std::cout << "Client disconnected\n";
        break;
      } else if (ec) {
        // Error reading from socket
        std::cout << "System error\n";
        throw boost::system::system_error(ec);
      }

      whiteboard::whiteboardPacket packet = parse_packet(&buffer);

      // Classify packet type and call corresponding method

      switch (static_cast<WhiteboardPacketType>(packet.packet_type())) {
      case WhiteboardPacketType::createWhiteboard: // CreateWhiteBoardRequest
        handle_create_whiteboard_request(packet.action().createwhiteboard(),
                                         socket);
        break;
      case WhiteboardPacketType::createShareUrl: // CreateSessionRequest
        handle_create_session_request(packet.action().createsession());
        break;
      // Add cases for other packet types...
      default:
        std::cout << "Unknown packet type\n";
        break;
      }
    }
  } catch (const std::exception &e) {
    std::cout << "Exception in handle_connection: " << e.what() << std::endl;
  }
}

void WhiteboardServer::handle_create_whiteboard_request(
    const whiteboard::CreateWhiteBoardRequest &request, tcp::socket &socket) {
  // Handle CreateWhiteBoardRequest
  std::cout << "Received CreateWhiteBoardRequest with user_id: "
            << request.user_id() << std::endl;
  try {
    whiteboard::whiteboardPacket response;
    whiteboard::ActionResponse *response_action =
        response.mutable_action()->mutable_actionresponse();
    response_action->set_success(true);
    response_action->set_message("Whiteboard created successfully");
    boost::asio::streambuf response_stream;
    std::ostream os(&response_stream);
    response.SerializeToOstream(&os);

    // Send response to client
    boost::asio::write(socket, response_stream);
  } catch (const std::exception &e) {
    std::cerr << "Exception in handle_create_whiteboard_request: " << e.what()
              << std::endl;
  }
}

void WhiteboardServer::handle_create_session_request(
    const whiteboard::CreateSessionRequest &request) {
  // Handle CreateSessionRequest
  std::cout << "Received CreateSessionRequest\n";
}
