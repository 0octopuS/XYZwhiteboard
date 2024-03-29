#include "../client/client.hpp" // Include your WhiteboardClient header
#include <iostream>
#include <thread>

std::vector<pair<string, string>> user = {{"Alice", "123456"},
                                          {"Bob", "bobIsGood"},
                                          {"Calico", "Youdontknowmypassword"},
                                          {"David", "ahahahhahas"},
                                          {"Elio", "eliooile"},
                                          {"Francisco", "willyougoodluck"},
                                          {"God", "Ihavenetwork"}};
int user_index = 2;
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
  client->send_create_session_request();
}

void test_joinSession(WhiteboardClient *client,
                      std::string whiteboard_id = "65f95be22289cf2d410f1331") {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  // std::cout << "Enter whiteboard ID (press Enter for default): ";
  // std::string input;
  // std::getline(std::cin, input);

  // If input is not empty, use the input as whiteboard_id
  // if (!input.empty()) {
  //   whiteboard_id = input;
  // }
  client->send_join_session_request(whiteboard_id); // Pass user ID
}

void test_quitSession(WhiteboardClient *client) {
#ifndef NDEBUG
  DEBUG_MSG;
#endif
  client->send_quit_session_request();
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
void test_login(WhiteboardClient *client) {
  auto [username, userpass] = user[user_index];
  client->send_login_request(username, userpass);
}

void test_register(WhiteboardClient *client) {
  auto [username, userpass] = user[user_index];
  client->send_register_request(username, userpass);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    user_index = 1;
  } else {

    if (user_index < 0 || user_index >= static_cast<int>(user.size())) {
      cerr << "Invalid user index." << endl;
      return 1;
    } else {
      user_index = stoi(argv[1]);
      cout << "USER:" << user[user_index].first << "\n";
    }
  }

  try {

    WhiteboardClient client("127.0.0.1", 12345); // Assuming server IP and port
    // client.start;
    // test_createWhiteboard(&client);
    // client.start();
    int times = 1000;
    while (times) {
      test_login(&client);
      // client.handle_receive();
      // test_createWhiteboard(&client);
      client.handle_receive();
      test_createWhiteboard(&client);
      client.handle_receive();
      test_joinSession(&client);
      client.handle_receive();
      test_addElement(&client);
      client.handle_receive();
      client.handle_receive();
      test_addElement(&client);
      client.handle_receive();
      client.handle_receive();
      test_addElement(&client);
      client.handle_receive();
      client.handle_receive();
      --times;
      // Check if there is any input available from the user
      // if (std::cin.peek() == '\n') {
      //   std::cin.ignore(); // Ignore newline character
      //   continue; // Continue running without any action if no input is given
      // }

      // Display options to the user
      // std::cout << "Select an action:\n";
      // std::cout << "1. Create Whiteboard\n";
      // std::cout << "2. Create Session\n";
      // std::cout << "3. Join Session\n";
      // std::cout << "4. Quit Session\n";
      // std::cout << "5. Add Element\n";
      // std::cout << "6. Delete Element\n";
      // std::cout << "7. Save Whiteboard\n";
      // std::cout << "8. Login\n";
      // std::cout << "9. Register\n";
      // std::cout << "0. Exit\n";

      // Get user input
      // int choice;
      // std::cin >> choice;

      // // Perform actions based on user input
      // switch (choice) {
      // case 0:
      //   std::cout << "Exiting...\n";
      //   return 0;
      // case 1:
      //   test_createWhiteboard(&client);
      //   break;
      // case 2:
      //   test_createSession(&client);
      //   break;
      // case 3:
      //   test_joinSession(&client);
      //   break;
      // case 4:
      //   test_quitSession(&client);
      //   break;
      // case 5:
      //   test_addElement(&client);
      //   break;
      // case 6:
      //   test_deleteElement(&client);
      //   break;
      // case 7:
      //   test_saveWhiteboard(&client);
      //   break;
      // case 8:
      //   test_login(&client);
      //   break;
      // case 9:
      //   test_register(&client);
      //   break;
      // default:
      //   std::cout << "Invalid choice. Please try again.\n";
      //   break;
      // }

      // Receive data from the server
      // client.handle_receive_async();
      // std::cout << "Received packet len: " << client.received_queue.size()
      // << "\n";

      // while (true) {
      //   bool received = client.handle_receive();
      //   if (received)
      //     std::cout << "Recevie packet len:" << client.received_queue.size()
      //               << "\n";

      // }
    }
    // test_createSession(&client);
    // test_joinSession(&client);
    // test_quitSession(&client);

    // // Simulate some work on the main thread while the client runs
    // std::cout << "Doing some work on the main thread..." << std::endl;

    // // std::this_thread::sleep_for(std::chrono::seconds(3));
    // // Close the connection after some time
    // client.close();
    // std::cout << "Client program exiting." << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception in test_client: " << e.what() << std::endl;
  }
  return 0;
}
