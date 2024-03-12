#include "../client/client.hpp" // Include your WhiteboardClient header
#include <iostream>
#include <thread>

void test_createWhiteboard(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  client->send_create_whiteboard_request(); // Pass user ID
}

void test_createSession(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  client->send_create_whiteboard_request();
}

void test_Joinsession(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  client->send_join_session_request(); // Pass user ID
}

void test_quitSession(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_addElement(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  WhiteboardElements square;
  square.new_square({1, 2}, 3.9);
  client->send_add_element_request(square);
}
void test_modifyElement(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_deleteElement(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_saveWhiteboard(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_actionResponse(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_broadcast(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}
void test_tempIdResponse(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
}

int main() {
  try {

    WhiteboardClient client("127.0.0.1", 12345); // Assuming server IP and port
    // client.start;
    test_createWhiteboard(&client);

    // Simulate some work on the main thread while the client runs
    std::cout << "Doing some work on the main thread..." << std::endl;
    while (true) {
      bool received = client.handle_receive();
      if (received)
        break;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    // Close the connection after some time
    client.close();
    std::cout << "Client program exiting." << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception in test_client: " << e.what() << std::endl;
  }
  return 0;
}
