#include "../server/server.hpp" // Include your WhiteboardServer header
#include <iostream>

int main() {
  try {
    WhiteboardServer server(12345); // Listen on port 12345
    server.start();
  } catch (const std::exception &e) {
    std::cerr << "Exception in test_server: " << e.what() << std::endl;
  }
  return 0;
}
