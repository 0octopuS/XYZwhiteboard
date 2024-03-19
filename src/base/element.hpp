#include "element.pb.h"
#include <cstdint>
#include <string>

enum class WhiteboardElementsType : uint8_t {
  Path = 0, // Path: random curve path
  Line = 1, // Line: straight line
  Circle = 2,
  Triangle = 3,
  Square = 4,
  Text = 5,
  Picture = 6,
  StickyNote = 7,
  Null = 16,
};

#define CASTTYPE(type) static_cast<int>(type)

class Point {
public:
  float x, y;
  std::string str() {
    std::string res = "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    return res;
  }
};

class WhiteboardElements {
private:
  WhiteboardElementsType type = WhiteboardElementsType::Null;
  // >>> pos1
  //
  Point pos1;
  // >>> pos2
  //
  Point pos2;
  Point pos3;
  // >>> side1
  //     square: side_length
  //     circuit: radius
  float side1;
  std::vector<Point> path_points;
  std::string content;

public:
  // For path
  void new_path(std::vector<Point> path_points);
  std::vector<Point> get_path_points();

  // For Line
  void new_line(Point _start, Point _end);
  Point get_line_start();
  Point get_line_end();

  // For Circle

  void new_circle(Point _center, float _radius);
  Point get_circle_center();
  float get_circle_radius();

  // For triangle

  void new_triangle(Point _p1, Point _p2, Point _p3);
  std::vector<Point> get_triangle_points();

  // For square
  void new_square(Point topleft, float side_length);
  void new_square(uint32_t _x, uint32_t _y, float _side_length);
  float get_square_side_length();
  Point get_square_topleft();

  // For text
  void new_text(Point center, std::string _content);
  Point get_text_center();
  std::string get_text_content();

  // For picture
  // void new_picture(std::string file_name, Point center, float
  // top_side_length, byte data);

  // For sticky note
  void new_stickynote(Point center, float _side_length, std::string _content);
  Point get_stickynote_center();
  float get_stickynote_side_length();
  std::string get_stickynote_content();
  // For general use
  protobuf::Element to_protobuf();
  WhiteboardElements from_protobuf(const protobuf::Element &ele);
  WhiteboardElementsType get_type() { return type; }
  void print();
};