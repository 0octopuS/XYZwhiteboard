
#include <exception>
#include <string>
using namespace ::std;

class ElementsTypeUnmatch : public exception {

public:
  ElementsTypeUnmatch(const string &msg) : msg_(msg) {}
  ~ElementsTypeUnmatch() {}

  const char *what() const noexcept override { return msg_.c_str(); }

private:
  string msg_;
};
