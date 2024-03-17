
#include <exception>
#include <string>
using namespace ::std;
#ifndef NDEBUG
#define DEBUG_MSG printf("\n*** %s:\n", __PRETTY_FUNCTION__);
#endif
class ElementsTypeUnmatch : public exception {

public:
  ElementsTypeUnmatch(const string &msg) : msg_(msg) {}
  ~ElementsTypeUnmatch() {}

  const char *what() const noexcept override { return msg_.c_str(); }

private:
  string msg_;
};

class ClientReceivedWrongPacketType : public exception {

public:
  ClientReceivedWrongPacketType() {}
  ClientReceivedWrongPacketType(const string &msg) : msg_(msg) {}
  ~ClientReceivedWrongPacketType() {}

  const char *what() const noexcept override { return msg_.c_str(); }

private:
  string msg_ = "Client should not received this type of packet";
};

class ClientReceiveServerSideFail : public exception {

public:
  ClientReceiveServerSideFail() {}
  ClientReceiveServerSideFail(const string &msg) : msg_(msg) {}
  ~ClientReceiveServerSideFail() {}

  const char *what() const noexcept override { return msg_.c_str(); }

private:
  string msg_ = "Client received server side fail";
};
