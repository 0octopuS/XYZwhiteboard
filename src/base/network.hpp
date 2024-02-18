#define PORT_NUMBER 12880
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iomanip>
#include <iostream>
#include <memory>
using boost::asio::ip::tcp;
using boost::system::error_code;

struct Session : std::enable_shared_from_this<Session> {
  template <typename Executor>
  Session(Executor executor) : _socket(make_strand(executor)) {}

  auto &Socket() { return _socket; }

  void StartSession();

private:
  tcp::socket _socket;
  std::string received;
  void on_received(error_code ec, size_t /*bytes_transferred*/);
};

using SessionPtr = std::shared_ptr<Session>;

namespace clientsession {
struct SessionManager {
  static SessionManager *GetInstance() {
    static SessionManager instance;
    return &instance;
  }
  template <typename... T> SessionPtr GenerateSession(T &&...args) {
    return std::make_shared<Session>(std::forward<T>(args)...);
  }
};
} // namespace clientsession

class Listener {
public:
  Listener(boost::asio::io_context &iosvc, tcp::endpoint ep)
      : _iosvc(iosvc), _acceptor(iosvc, ep) {}

  void Start_accept();

private:
  boost::asio::io_context &_iosvc;
  tcp::acceptor _acceptor;
  void OnAcceptComplete(error_code error, SessionPtr session);
};
