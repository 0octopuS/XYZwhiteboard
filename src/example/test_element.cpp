#include "../base/element.hpp"
#include <iostream>

#define PRINT_PROTOBUF_POINT(p) std::cout << "(" << p.x() << "," << p.y() << ")"

void test_null_element() {
  WhiteboardElements element;
  element.print();
}
void test_path_element() {
  WhiteboardElements path;
  std::vector<Point> points = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
  path.new_path(points);
  path.print();

  // Protobuf test
  auto path_buf = path.to_protobuf();
  auto protobuf_path = path_buf.path();
  auto protobuf_points = protobuf_path.points();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> Points num:" << protobuf_points.size() << "\n";
  std::cout << "   >>> Points detail:";
  for (auto &p : protobuf_points) {
    PRINT_PROTOBUF_POINT(p);
  }
  std::cout << std::endl;
}

void test_line_element() {
  WhiteboardElements line;
  line.new_line({1, 2}, {2, 3});
  line.print();

  // Protobuf test
  auto line_buf = line.to_protobuf();
  auto protobuf_line = line_buf.line();
  auto protobuf_start = protobuf_line.start();
  auto protobuf_end = protobuf_line.end();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> Start:";
  PRINT_PROTOBUF_POINT(protobuf_start);
  std::cout << "\n";
  std::cout << "   >>> End  :";
  PRINT_PROTOBUF_POINT(protobuf_end);
  std::cout << "\n";
}

void test_circle_element() {
  WhiteboardElements element;
  element.new_circle({1, 2}, 3.7);
  element.print();

  // Protobuf test
  auto pb_ele = element.to_protobuf().circle();
  auto protobuf_center = pb_ele.center();
  auto protobuf_radius = pb_ele.radius();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> center:";
  PRINT_PROTOBUF_POINT(protobuf_center);
  std::cout << "\n";
  std::cout << "   >>> radius:" << protobuf_radius << "\n";
}

void test_triangle_element() {
  WhiteboardElements element;
  element.new_triangle({1, 2}, {2, 3}, {3, 1});
  element.print();

  // Protobuf test
  auto pb_ele = element.to_protobuf().triangle();
  auto protobuf_point1 = pb_ele.point1();
  auto protobuf_point2 = pb_ele.point2();
  auto protobuf_point3 = pb_ele.point3();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> point1:";
  PRINT_PROTOBUF_POINT(protobuf_point1);
  std::cout << "\n";
  std::cout << "   >>> point2:";
  PRINT_PROTOBUF_POINT(protobuf_point2);
  std::cout << "\n";
  std::cout << "   >>> point3:";
  PRINT_PROTOBUF_POINT(protobuf_point3);
  std::cout << "\n";
}

void test_square_element() {
  WhiteboardElements element;
  element.new_square({1, 2}, 3.8);
  element.print();

  // Protobuf test
  auto pb_ele = element.to_protobuf().square();
  auto protobuf_topleft = pb_ele.topleft();
  auto protobuf_side_length = pb_ele.side_length();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> side_length:" << protobuf_side_length << "\n";
  std::cout << "   >>> topleft:";
  PRINT_PROTOBUF_POINT(protobuf_topleft);
  std::cout << "\n";
}

void test_text_element() {
  WhiteboardElements element;
  element.new_text({1, 2}, "Greeting from 42");
  element.print();

  // Protobuf test
  auto pb_ele = element.to_protobuf().text();
  auto protobuf_center = pb_ele.center();
  auto protobuf_content = pb_ele.content();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> center:";
  PRINT_PROTOBUF_POINT(protobuf_center);
  std::cout << "\n";
  std::cout << "   >>> content:" + protobuf_content + "\n";
}

void test_stickynote_element() {
  WhiteboardElements element;
  element.new_stickynote({1, 2}, 3.8, "Greeting from 42");
  element.print();

  // Protobuf test
  auto pb_ele = element.to_protobuf().stickynote();
  auto protobuf_center = pb_ele.center();
  auto protobuf_side_length = pb_ele.side_length();
  auto protobuf_content = pb_ele.content();
  std::cout << "   >>> Content from converted protobuf\n";
  std::cout << "   >>> center     :";
  PRINT_PROTOBUF_POINT(protobuf_center);
  std::cout << "\n";
  std::cout << "   >>> side_length:" << protobuf_side_length << "\n";
  std::cout << "   >>> content    :" + protobuf_content + "\n";
}
int main() {
  //   test_null_element();
  //   test_path_element();
  //   test_line_element();
  //   test_circle_element();
  //   test_triangle_element();
  //   test_square_element();
  //   test_text_element();
  test_stickynote_element();
}