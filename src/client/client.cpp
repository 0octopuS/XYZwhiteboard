#include "client.hpp"

void WhiteboardClient::add_element() {

  // A possible example from a xml <Path> component, not the real action
  element::Path path;
  path.set_id("lutz@fb10dtools_654");
  path.set_stroke_color("0.0,0.0,0.0");

  // Add points to the path
  path.add_points()->set_x(2286.0);
  path.mutable_points(0)->set_y(1237.6575);
  path.add_points()->set_x(2283.0);
  path.mutable_points(1)->set_y(1237.6575);
  // Add more points as needed...

  // Add x and y attributes
  path.mutable_points(0)->set_x(119.0);
  path.mutable_points(0)->set_y(354.0);
}

int main(int argc, char const *argv[]) {
  action::Session s;
  s.set_session_id("2233");
  // cout << "Hello Proto!" << s.session_id << endl;
  return 0;
}
