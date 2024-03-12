#include "../base/exception.hpp"
#include "../base/packet.hpp" // Include your WhiteboardPacket class
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <queue>
using namespace std;

using boost::asio::ip::tcp;

class WhiteboardClient {
private:
  uint32_t version = 1;
  uint32_t user_id = 0;
  boost::asio::io_context io_context;
  tcp::resolver resolver;
  tcp::socket socket;
  // boost::asio::streambuf receive_buffer;
  bool connected_;
  std::mutex connected_mutex_;
  std::condition_variable connected_cv_;
  std::thread receive_thread_;
  std::thread send_thread_;
  // char receive_buffer_[1024];
  std::queue<WhiteboardPacket> send_queue_;
  boost::asio::ip::tcp::resolver::iterator iter;

public:
  WhiteboardClient(const std::string &server_ip, unsigned short port)
      : io_context(), resolver(io_context), socket(io_context),
        connected_(false), connected_mutex_(), connected_cv_() {
#ifndef NDEBUG
    DEBUG_MSG;
#endif
    iter = resolver.resolve(server_ip, std::to_string(port));
    // tcp::resolver::results_type endpoints =
    // resolver.resolve(server_ip, std::to_string(port));
    // boost::asio::ip::tcp::resolver::query query(server_ip,
    // std::to_string(port));

    boost::system::error_code error;
    // boost::asio::async_connect(socket,
    // iter,
    //                            boost::bind(&WhiteboardClient::handle_connect,
    //                                        this,
    //                                        boost::asio::placeholders::error));
    boost::asio::connect(socket, iter, error);
    if (!error) {
      // Start asynchronous
      // receive operation
      std::lock_guard<std::mutex> lock(connected_mutex_);
      connected_ = true;
      connected_cv_.notify_all(); // start_receive();
      printf("Connected, "
             "notify.\n");
    } else {
      // Handle connection error
      std::cerr << "Error connecting "
                   "to server: "
                << error.message() << std::endl;
    }
    // boost::asio::async_connect(socket,
    // iter,
    //                            boost::bind(&WhiteboardClient::handle_connect,
    //                                        this,
    //                                        boost::asio::placeholders::error));
  }

  ~WhiteboardClient() {
    close(); // Gracefully close connection in destructor
    // receive_thread_.join();
    // send_thread_.join();
    // thread_.join(); // Wait for thread to finish
  }

  void send_packet(const WhiteboardPacket &packet);
  void send_create_whiteboard_request();
  void send_join_session_request() {}
  void send_add_element_request(WhiteboardElements _ele);
  bool handle_receive();
  void handle_connect(const boost::system::error_code &error);
  protobuf::whiteboardPacket parse_packet(boost::asio::streambuf *buffer);
  void start_receive();
  void handle_send();
  // void start(const std::string &server_ip, unsigned short port);
  void close();
};
