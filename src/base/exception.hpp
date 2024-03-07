
#include <exception>
#include <string>
using namespace ::std;

class ElementsTypeUnmatch : public exception {

public:
  ElementsTypeUnmatch(const string &msg) : msg_(msg) {}
  ~ElementsTypeUnmatch() {}

  string getMessage() const { return (msg_); }

private:
  string msg_;
};
