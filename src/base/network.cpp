#include "network.hpp"

void Session::StartSession() {
  std::cout << "Session: " << this << " StartSession" << std::endl;
  async_read(_socket, boost::asio::dynamic_buffer(received),
             [self = shared_from_this()](auto ec, size_t tx) {
               self->on_received(ec, tx);
             });
}
void Session::on_received(error_code ec, size_t /*bytes_transferred*/) {
  std::cout << "Session: " << this << " on_received " << ec.message() << " "
            << std::quoted(received) << std::endl;
}

void Listener::Start_accept() {
  // Create Client Session
  auto new_session =
      clientsession::SessionManager::GetInstance()->GenerateSession(
          _iosvc.get_executor());

  std::cout << "Listening.....\n\n";
  _acceptor.async_accept(new_session->Socket(),
                         boost::bind(&Listener::OnAcceptComplete, this,
                                     boost::asio::placeholders::error,
                                     new_session));
}

void Listener::OnAcceptComplete(error_code error, SessionPtr session) {
  if (!error) {
    std::cout << "accept completed\n";
    session->StartSession();
    Start_accept();
  } else {
    std::cout << error.message() << '\n';
  }
}
