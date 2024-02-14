#include "action.pb.h"
#include "element.pb.h"
#include <iostream>

using namespace std;

class WhiteboardClient {
private:
  bool is_host = false;

public:
  void create_whiteboard();
  void add_element();
};
