#include "../client/client.hpp" // Include your WhiteboardClient header
#include <iostream>
#include <thread>

void test_createwhiteboard(WhiteboardClient *client) {
  client->send_create_whiteboard_request(); // Pass user ID

  // WhiteboadElements sqaure(1, 2);
  // Elements sqaure2;
  // sqaure2.new_square(1, 2);
  // // WhiteboardPacket packet;
  // // packet.new_add_element_request(a);?
  // client->send_add_element_request(sqaure);
  // client->send_modify_element_request(sqaure1, sqaure2);

  std::cout << "Sent CreateWhiteBoardRequest\n";
}

int main() {
  try {
    WhiteboardClient client("127.0.0.1", 12345); // Assuming server IP and port
    test_createwhiteboard(&client);

    // Continuously handle responses
    while (true) {
      client.handle_response();
      // Optionally sleep or perform other tasks
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception in test_client: " << e.what() << std::endl;
  }
  return 0;
}
