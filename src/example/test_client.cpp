#include "../client/client.hpp" // Include your WhiteboardClient header
#include <iostream>
#include <thread>

int main() {
  try {
    WhiteboardClient client("127.0.0.1", 12345); // Assuming server IP and port
    client.send_create_whiteboard_request();     // Pass user ID
    std::cout << "Sent CreateWhiteBoardRequest\n";

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
