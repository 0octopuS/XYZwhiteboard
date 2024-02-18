#include "client.hpp"

void WhiteboardClient::send_create_whiteboard_request() {
#ifndef NDEBUG
  printf(">>> WhiteboardClient::send_create_whiteboard_request()\n");
#endif
  WhiteboardPacket packet(version);
  packet.new_create_whiteboard_request(user_id);
#ifndef NDEBUG
  // packet.print();
#endif
  send_packet(packet);
}

void WhiteboardClient::send_add_element_request() {

  // A possible example from a xml <Path> component, not the real action
  // whiteboard::Path path;
  // path.mutable_common()->set_id("lutz@fb10dtools_654");
  // path.mutable_common()->set_stroke_color("0.0,0.0,0.0");

  // // Add points to the path
  // path.add_points()->set_x(2286.0);
  // path.mutable_points(0)->set_y(1237.6575);
  // path.add_points()->set_x(2283.0);
  // path.mutable_points(1)->set_y(1237.6575);
  // // Add more points as needed...

  // // Add x and y attributes
  // path.mutable_points(0)->set_x(119.0);
  // path.mutable_points(0)->set_y(354.0);
}

void WhiteboardClient::handle_response() {
  try {
    whiteboard::whiteboardPacket packet;
    boost::asio::streambuf buffer;
    boost::system::error_code ec;

    // Read from socket
    boost::asio::read(socket, buffer, ec);
    if (ec == boost::asio::error::eof) {
      std::cout << "Server disconnected\n";
      return;
    } else if (ec) {
      throw boost::system::system_error(ec);
    }

    // Process received data
    std::istream is(&buffer);
    if (!packet.ParseFromIstream(&is)) {
      std::cerr << "Failed to parse packet\n";
      return;
    }
    std::cout << &buffer << std::endl;
    // Handle response packet
    // ...
  } catch (const std::exception &e) {
    std::cerr << "Exception in handle_response: " << e.what() << std::endl;
  }
}