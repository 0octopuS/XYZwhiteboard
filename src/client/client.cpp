#include "action.pb.h"
#include <iostream>

using namespace std;
int main(int argc, char const *argv[]) {
  action::Session s;
  s.set_session_id("2233");
  // cout << "Hello Proto!" << s.session_id << endl;
  return 0;
}
