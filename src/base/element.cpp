#include "element.hpp"
#include "element.pb.h"
#include "exception.hpp"
#include <tuple>

// 1. Path
void WhiteboardElements::new_path(std::vector<Point> _path_points) {
  type = WhiteboardElementsType::Path;
  path_points = _path_points;
}

std::vector<Point> WhiteboardElements::get_path_points() {
  if (type != WhiteboardElementsType::Path) {
    throw ElementsTypeUnmatch("Cannot [path_points] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return path_points;
}

// 2.Line

void WhiteboardElements::new_line(Point _start, Point _end) {
  type = WhiteboardElementsType::Line;
  pos1 = _start;
  pos2 = _end;
}
Point WhiteboardElements::get_line_start() {
  if (type != WhiteboardElementsType::Line) {
    throw ElementsTypeUnmatch("Cannot [line_start] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}
Point WhiteboardElements::get_line_end() {
  if (type != WhiteboardElementsType::Line) {
    throw ElementsTypeUnmatch("Cannot [line_end] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos2;
}

// 3. Circle

void WhiteboardElements::new_circle(Point _center, float _radius) {
  type = WhiteboardElementsType::Circle;
  pos1 = _center;
  side1 = _radius;
}
Point WhiteboardElements::get_circle_center() {
  if (type != WhiteboardElementsType::Line) {
    throw ElementsTypeUnmatch("Cannot [circle_center] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}
float WhiteboardElements::get_circle_radius() {
  if (type != WhiteboardElementsType::Line) {
    throw ElementsTypeUnmatch("Cannot [circle_radius] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return side1;
}

// 4. Triangle

void WhiteboardElements::new_triangle(Point _p1, Point _p2, Point _p3) {
  type = WhiteboardElementsType::Triangle;
  pos1 = _p1;
  pos2 = _p2;
  pos3 = _p3;
}
std::tuple<Point, Point, Point> WhiteboardElements::get_triangle_points() {
  if (type != WhiteboardElementsType::Triangle) {
    throw ElementsTypeUnmatch("Cannot [triangle_points] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  std::tuple<Point, Point, Point> point_tuple = {pos1, pos2, pos3};
  return point_tuple;
}

// 4. Square
void WhiteboardElements::new_square(Point topleft, float _side_length) {
  type = WhiteboardElementsType::Square;
  pos1 = topleft;
  side1 = _side_length;
}

void WhiteboardElements::new_square(uint32_t _x, uint32_t _y,
                                    float _side_length) {
  pos1.x = _x;
  pos1.y = _y;
  side1 = _side_length;
}

float WhiteboardElements::get_square_side_length() {
  if (type != WhiteboardElementsType::Square) {
    throw ElementsTypeUnmatch("Cannot [side_length] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return side1;
}

Point WhiteboardElements::get_square_topleft() {
  if (type != WhiteboardElementsType::Square) {
    throw ElementsTypeUnmatch("Cannot [topleft] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}

whiteboard::Element WhiteboardElements::get_proto_element() {
  whiteboard::Element ele;
  switch (type) {

  case WhiteboardElementsType::Path: {
    whiteboard::Path path;
    for (auto &p : get_path_points()) {
      // whiteboard::Point point;
      whiteboard::Point *point = path.add_points();
      point->set_x(p.x);
      point->set_y(p.y);
    }
    ele.set_allocated_path(&path);
    break;
  }
  case WhiteboardElementsType::Line: {
    whiteboard::Line line;
    whiteboard::Point *start = line.mutable_start();
    whiteboard::Point *end = line.mutable_end();

    auto start_point = get_line_start();
    auto end_point = get_line_end();
    start->set_x(start_point.x);
    start->set_y(start_point.y);
    end->set_x(end_point.x);
    end->set_x(end_point.y);

    ele.set_allocated_line(&line);
    break;
  }
  case WhiteboardElementsType::Circle: {
    whiteboard::Circle circle;
    whiteboard::Point *center = circle.mutable_center();

    // whiteboard::Point *end = line.mutable_end();

    auto circle_center_point = get_circle_center();
    auto circle_radius = get_circle_radius();

    center->set_x(circle_center_point.x);
    center->set_y(circle_center_point.y);
    circle.set_radius(circle_radius);

    ele.set_allocated_circle(&circle);
    break;
  }
  case WhiteboardElementsType::Triangle:
  case WhiteboardElementsType::Square:
  case WhiteboardElementsType::Text:
  case WhiteboardElementsType::Picture:
  case WhiteboardElementsType::StickyNote:
    break;
  }
  return ele;
}