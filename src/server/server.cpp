#include "server.hpp"
#include "action.pb.h"
#include "packet.pb.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <cstdint>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <iostream>

void WhiteboardServer::accept_connections() {
  try {
    while (true) {
      // Accept a new connection
      tcp::socket socket(io_context);
      acceptor.accept(socket);
#ifndef NDEBUG
      std::string sClientIp = socket.remote_endpoint().address().to_string();
      unsigned short uiClientPort = socket.remote_endpoint().port();
      std::cout << ">>> Connecting from (" << sClientIp << "," << uiClientPort
                << ")\n";
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

protobuf::whiteboardPacket
WhiteboardServer::parse_packet(boost::asio::streambuf *buffer) {
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
void WhiteboardServer::handle_connection(tcp::socket socket) {
  try {
    while (true) {
      // Receive packet

      boost::asio::streambuf buffer;
      boost::system::error_code ec;
      // Read more here
      // size_t bytes_transferred =

      boost::asio::read_until(socket, buffer, '\n', ec);
      if (ec == boost::asio::error::eof) {
        // Client disconnected
        std::cout << "Client disconnected\n";
        break;
      } else if (ec) {
        // Error reading from socket
        std::cout << "System error\n";
        throw boost::system::system_error(ec);
      }

      protobuf::whiteboardPacket packet = parse_packet(&buffer);

      // Classify packet type and call corresponding method

      switch (static_cast<WhiteboardPacketType>(packet.packet_type())) {
      case WhiteboardPacketType::createWhiteboard: // CreateWhiteBoardRequest
        handle_create_whiteboard_request(packet, socket);
        break;
      case WhiteboardPacketType::createSession: // CreateSessionRequest
        handle_create_session_request(packet, socket);
        break;
      case WhiteboardPacketType::joinSession:
        handle_join_session_request(packet, socket);
        break;
      case WhiteboardPacketType::quitSession:
        handle_quit_session_request(packet, socket);
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
int getNextSequence(mongocxx::client &client, const std::string &db_name,
                    const std::string &sequence_collection_name) {
  auto collection = client[db_name][sequence_collection_name];
  bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
      collection.find_one_and_update(
          bsoncxx::builder::stream::document{}
              << bsoncxx::builder::stream::finalize,
          bsoncxx::builder::stream::document{}
              << "$inc" << bsoncxx::builder::stream::open_document << "seq" << 1
              << bsoncxx::builder::stream::close_document
              << bsoncxx::builder::stream::finalize,
          mongocxx::options::find_one_and_update{}.upsert(true));

  if (maybe_result) {
    auto doc = maybe_result.value().view();
    if (doc["seq"]) {
      return doc["seq"].get_int32().value;
    }
  }
  return 0; // Default value if sequence is not found
}
uint32_t WhiteboardServer::handle_assign_user_id(tcp::socket &tcp_socket) {
  uint32_t temp_user_id = 1; // change to a RNG
  std::cout << (temp_user_id);
  int uid = getNextSequence(mongoclient, "whiteboards", "user_collection");

  bsoncxx::document::value document = bsoncxx::builder::stream::document{}
                                      << "_id" << uid << "username"
                                      << ""
                                      << "password_hash"
                                      << ""
                                      << bsoncxx::builder::stream::finalize;

  auto insert_result = user_collection.insert_one(document.view());

#ifndef NDEBUG
  std::cout << "User with ID " << uid << " has been inserted." << std::endl;
#endif
  // When the type is tempIdResponse, it is a server spontaneous behavior.
  // Client does not have a corresponding request, so the packet_id is not
  // necessary here.
  WhiteboardPacket response(version, WhiteboardPacketType::tempIdResponse,
                            rand());

  bool success = insert_result
                     ? insert_result.value().result().inserted_count() == 1
                     : false;
  // TODO: exception handle should be added here
  response.new_temp_id_response(success, uid);
  send_packet(tcp_socket, response);
  return uid;
}

void WhiteboardServer::handle_create_whiteboard_request(
    const protobuf::whiteboardPacket &packet, tcp::socket &tcp_socket) {
  // Handle CreateWhiteBoardRequest
  auto packet_id = packet.packet_id();
  auto request = packet.action().createwhiteboard();
  auto user_id = request.user_id();
  std::cout << "Received CreateWhiteBoardRequest with user_id: " << user_id
            << "packet_id: " << packet_id << std::endl;

  // Step 1: If the user is anonymous, then server first assign
  // a temporary id; send a packet back to client
  // them connect the user_id with new whiteboard.
  if (request.user_id() == 0) {
    user_id = handle_assign_user_id(tcp_socket);
  }

  // Step 2: Create whiteboard, return to user

  auto doc_value = make_document(
      kvp("uid", static_cast<int>(user_id)), kvp("elements", make_array()),
      kvp("connected_user_num", 1),
      kvp("created_at",
          bsoncxx::types::b_date(std::chrono::system_clock::now())));
  ;
  auto insert_one_result = whiteboard_collection.insert_one(doc_value.view());
  WhiteboardPacket response(version, WhiteboardPacketType::actionResponse,
                            packet_id);
  if (insert_one_result) {
    auto doc_id = insert_one_result->inserted_id().get_oid();
    // Now, you can construct your response message using the inserted ObjectId
    std::cout << "***" << doc_id.value.to_string() << '\n';
    response.new_action_response(true, doc_id.value.to_string());
  } else {
    // Handle insertion failure
    response.new_action_response(false, "Fail to create whiteboard document");
  }
  send_packet(tcp_socket, response);
}

// TODO: unfinished function
void WhiteboardServer::handle_create_session_request(
    const protobuf::whiteboardPacket &packet, tcp::socket &tcp_socket) {
  // Handle CreateSessionRequest
  std::cout << "Received CreateSessionRequest\n";
  auto request = packet.action().createsession();
  auto packet_id = packet.packet_id();
  auto user_id = static_cast<int>(request.user_id());
  auto whiteboard_id = request.whiteboard_id();

  // Step 1. fetch from database to see if the client is the admin
  auto filter = bsoncxx::builder::stream::document{}
                << "_id" << bsoncxx::oid{whiteboard_id}
                << bsoncxx::builder::stream::finalize;
  auto result = whiteboard_collection.find_one(filter.view());
  WhiteboardPacket response(version, WhiteboardPacketType::actionResponse,
                            packet_id);
  if (result) {
    auto whiteboard_doc = result->view();
    auto uid_field = whiteboard_doc["uid"];
    if (uid_field &&
        uid_field.get_int32().value == static_cast<int32_t>(user_id)) {
      // User ID matches, send success response
      add_socket_to_whiteboard(whiteboard_id, &tcp_socket);
      auto update = bsoncxx::builder::stream::document{}
                    << "$set" << bsoncxx::builder::stream::open_document
                    << "active_user_ids" << bsoncxx::builder::stream::open_array
                    << user_id << bsoncxx::builder::stream::close_array
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;
      whiteboard_collection.update_one(filter.view(), update.view());
      response.new_action_response(true, "Session created successfully");
      // Send response to the client (implementation of sending response to
      // socket goes here)
    } else {
      // User ID does not match, send failure response
      response.new_action_response(false, "User ID does not match");
      // Send response to the client (implementation of sending response to
      // socket goes here)
    }
  } else {
    // Whiteboard not found, send failure response
    response.new_action_response(false, "Whiteboard not found");
    // Send response to the client (implementation of sending response to socket
    // goes here)
  }
  send_packet(tcp_socket, response);
}

void WhiteboardServer::handle_join_session_request(
    const protobuf::whiteboardPacket &packet, tcp::socket &tcp_socket) {
  // Handle JoinSessionRequest
  std::cout << "Received JoinSessionRequest\n";
  auto request = packet.action().joinsession();
  auto packet_id = packet.packet_id();
  auto user_id = static_cast<int>(request.user_id());
  auto whiteboard_id = request.whiteboard_id();

  // Step 1. Fetch from database to see if the whiteboard exists
  auto filter = bsoncxx::builder::stream::document{}
                << "_id" << bsoncxx::oid{whiteboard_id}
                << bsoncxx::builder::stream::finalize;
  auto result = whiteboard_collection.find_one(filter.view());
  WhiteboardPacket response(version, WhiteboardPacketType::actionResponse,
                            packet_id);
  if (result) {
    auto whiteboard_doc = result->view();
    auto active_user_ids_field = whiteboard_doc["active_user_ids"];
    if (active_user_ids_field) {
      // Step 2. Add the socket to the whiteboard
      add_socket_to_whiteboard(whiteboard_id, &tcp_socket);

      // Step 3. Update active_user_ids field in the whiteboard document
      auto update = bsoncxx::builder::stream::document{}
                    << "$addToSet" << bsoncxx::builder::stream::open_document
                    << "active_user_ids" << user_id
                    << bsoncxx::builder::stream::close_document
                    << bsoncxx::builder::stream::finalize;
      whiteboard_collection.update_one(filter.view(), update.view());
      // Success response
      response.new_action_response(true, "Joined session successfully");
    } else {
      // Admin ID not found, send failure response
      response.new_action_response(false, "Whiteboard is not in shared status");
    }
  } else {
    // Whiteboard not found, send failure response
    response.new_action_response(false, "Whiteboard not found");
  }
  send_packet(tcp_socket, response);
}

void WhiteboardServer::handle_quit_session_request(
    const protobuf::whiteboardPacket &packet, tcp::socket &tcp_socket) {
  // Handle QuitSessionRequest
  std::cout << "Received QuitSessionRequest\n";
  auto request = packet.action().quitsession();
  auto packet_id = packet.packet_id();
  auto user_id = static_cast<int>(request.user_id());
  auto whiteboard_id = request.whiteboard_id();

  // Step 1. Remove the socket from the whiteboard_sessions
  remove_socket_from_whiteboard(whiteboard_id, &tcp_socket);

  // Step 2. Update active_user_ids field in the whiteboard document
  auto filter = bsoncxx::builder::stream::document{}
                << "_id" << bsoncxx::oid{whiteboard_id}
                << bsoncxx::builder::stream::finalize;
  auto update = bsoncxx::builder::stream::document{}
                << "$pull" << bsoncxx::builder::stream::open_document
                << "active_user_ids" << user_id
                << bsoncxx::builder::stream::close_document
                << bsoncxx::builder::stream::finalize;
  whiteboard_collection.update_one(filter.view(), update.view());

  // Step 3. Check if the whiteboard should be deleted
  auto result = whiteboard_collection.find_one(filter.view());
  if (result) {
    auto whiteboard_doc = result->view();
    auto active_user_ids_field = whiteboard_doc["active_user_ids"];
    if (active_user_ids_field) {
      auto active_user_ids = active_user_ids_field.get_array().value;
      bool should_delete_whiteboard = true;
      for (bsoncxx::array::element user_id : active_user_ids) {
        should_delete_whiteboard = false; // If there is at least one active
        // user, do not delete the whiteboard
        break;
      }

      if (should_delete_whiteboard) {
        // Delete the whiteboard document if no active users remaining
        whiteboard_collection.delete_one(filter.view());
      }
    }
  }
  // Send response to the client
  WhiteboardPacket response(version, WhiteboardPacketType::actionResponse,
                            packet_id);
  response.new_action_response(true, "Quit session successfully");
  send_packet(tcp_socket, response);
}

void WhiteboardServer::handle_add_element_request(
    const protobuf::AddElementRequest &request) {
  // Step 1
  WhiteboardElements new_ele;
  auto proto_ele = request.element();
  std::string proto_ele_string = proto_ele.SerializeAsString();
  auto doc_value = make_document(kvp("elements", proto_ele_string));
  protobuf::whiteboardPacket new_proto_ele;
  new_proto_ele.ParseFromString(proto_ele_string);
  // auto insert_one_result =
  whiteboard_collection.insert_one(doc_value.view());
}

void WhiteboardServer::send_packet(tcp::socket &tcp_socket,
                                   const WhiteboardPacket &packet) {
  // Serialize WhiteboardPacket to a buffer
  // std::ostringstream oss;
  try {
    boost::asio::streambuf request;
    std::ostream os(&request);
    {
      ::google::protobuf::io::OstreamOutputStream raw_output_stream(&os);
      ::google::protobuf::io::CodedOutputStream coded_output_stream(
          &raw_output_stream);

      packet.serialize(&coded_output_stream);
    }

    size_t byte = boost::asio::write(tcp_socket, request);
#ifndef NDEBUG
    std::string sClientIp = tcp_socket.remote_endpoint().address().to_string();
    unsigned short uiClientPort = tcp_socket.remote_endpoint().port();
    std::cout << ">>> Send to (" << sClientIp << "," << uiClientPort << ")\n";
    printf(">>> send_packet: %zu\n", byte);
    printf(">>> send packet type\n");
    packet.print();
#endif
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
