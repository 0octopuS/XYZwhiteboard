#include "element.hpp"
#include "element.pb.h"
#include "exception.hpp"
#include <string>
#include <tuple>
std::string WhiteboardElementsTypeDict[] = {
    "Path",   "Line", "Circle",  "Triangle",
    "Square", "Text", "Picture", "StickyNote",
};
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
  if (type != WhiteboardElementsType::Circle) {
    throw ElementsTypeUnmatch("Cannot [circle_center] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}
float WhiteboardElements::get_circle_radius() {
  if (type != WhiteboardElementsType::Circle) {
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
std::vector<Point> WhiteboardElements::get_triangle_points() {
  if (type != WhiteboardElementsType::Triangle) {
    throw ElementsTypeUnmatch("Cannot [triangle_points] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return {pos1, pos2, pos3};
}

// 4. Square
void WhiteboardElements::new_square(Point topleft, float _side_length) {
  type = WhiteboardElementsType::Square;
  pos1 = topleft;
  side1 = _side_length;
}

void WhiteboardElements::new_square(uint32_t _x, uint32_t _y,
                                    float _side_length) {
  type = WhiteboardElementsType::Square;
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

// 5. Text
void WhiteboardElements::new_text(Point center, std::string _content) {
  type = WhiteboardElementsType::Text;
  pos1 = center;
  content = _content;
}
Point WhiteboardElements::get_text_center() {
  if (type != WhiteboardElementsType::Text) {
    throw ElementsTypeUnmatch("Cannot [center] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}
std::string WhiteboardElements::get_text_content() {
  if (type != WhiteboardElementsType::Text) {
    throw ElementsTypeUnmatch("Cannot [content] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return content;
}

void WhiteboardElements::new_stickynote(Point _center, float _side_length,
                                        std::string _content) {
  type = WhiteboardElementsType::StickyNote;
  pos1 = _center;
  side1 = _side_length;
  content = _content;
}
Point WhiteboardElements::get_stickynote_center() {
  if (type != WhiteboardElementsType::StickyNote) {
    throw ElementsTypeUnmatch("Cannot [center] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}
float WhiteboardElements::get_stickynote_side_length() {
  if (type != WhiteboardElementsType::StickyNote) {
    throw ElementsTypeUnmatch("Cannot [side_length] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return side1;
}
std::string WhiteboardElements::get_stickynote_content() {
  if (type != WhiteboardElementsType::StickyNote) {
    throw ElementsTypeUnmatch("Cannot [content] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return content;
}

void WhiteboardElements::new_picture(Point _center, std::string _content,
                                     std::string _filename) {
  type = WhiteboardElementsType::Picture;
  pos1 = _center;
  content = _content;
  filename = _filename;
}
Point WhiteboardElements::get_picture_center() {
  if (type != WhiteboardElementsType::Picture) {
    throw ElementsTypeUnmatch("Cannot [center] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return pos1;
}

std::string WhiteboardElements::get_picture_content() {
  if (type != WhiteboardElementsType::Picture) {
    throw ElementsTypeUnmatch("Cannot [content] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return content;
}

std::string WhiteboardElements::get_picture_filename() {
  if (type != WhiteboardElementsType::Picture) {
    throw ElementsTypeUnmatch("Cannot [content] from type" +
                              WhiteboardElementsTypeDict[CASTTYPE(type)]);
  }
  return filename;
}

protobuf::Element WhiteboardElements::to_protobuf() {
  protobuf::Element ele;
  switch (type) {

  case WhiteboardElementsType::Path: {
    protobuf::Path *path = ele.mutable_path();
    for (auto &p : get_path_points()) {
      // protobuf::Point point;
      protobuf::Point *point = path->add_points();
      point->set_x(p.x);
      point->set_y(p.y);
    }
    break;
  }
  case WhiteboardElementsType::Line: {
    protobuf::Line *line = ele.mutable_line();
    protobuf::Point *start = line->mutable_start();
    protobuf::Point *end = line->mutable_end();

    auto start_point = get_line_start();
    auto end_point = get_line_end();
    start->set_x(start_point.x);
    start->set_y(start_point.y);
    end->set_x(end_point.x);
    end->set_y(end_point.y);
    break;
  }
  case WhiteboardElementsType::Circle: {
    protobuf::Circle *circle = ele.mutable_circle();
    protobuf::Point *center = circle->mutable_center();

    // protobuf::Point *end = line.mutable_end();

    auto circle_center_point = get_circle_center();
    auto circle_radius = get_circle_radius();

    center->set_x(circle_center_point.x);
    center->set_y(circle_center_point.y);
    circle->set_radius(circle_radius);

    break;
  }
  case WhiteboardElementsType::Triangle: {
    protobuf::Triangle *triangle = ele.mutable_triangle();
    protobuf::Point *point0 = triangle->mutable_point1();
    protobuf::Point *point1 = triangle->mutable_point2();
    protobuf::Point *point2 = triangle->mutable_point3();
    // protobuf:: triangle->
    // protobuf::Point *end = line.mutable_end();

    auto triangle_points = get_triangle_points();

    point0->set_x(triangle_points[0].x);
    point0->set_y(triangle_points[0].y);
    point1->set_x(triangle_points[1].x);
    point1->set_y(triangle_points[1].y);
    point2->set_x(triangle_points[2].x);
    point2->set_y(triangle_points[2].y);

    break;
  }
  case WhiteboardElementsType::Square: {
    protobuf::Square *square = ele.mutable_square();
    protobuf::Point *topleft = square->mutable_topleft();

    // protobuf::Point *end = line.mutable_end();

    auto square_topleft = get_square_topleft();
    auto square_side_length = get_square_side_length();

    topleft->set_x(square_topleft.x);
    topleft->set_y(square_topleft.y);
    square->set_side_length(square_side_length);
    break;
  }
  case WhiteboardElementsType::Text: {
    protobuf::Text *text = ele.mutable_text();
    protobuf::Point *center = text->mutable_center();

    // protobuf::Point *end = line.mutable_end();

    auto text_center = get_text_center();
    auto text_content = get_text_content();

    center->set_x(text_center.x);
    center->set_y(text_center.y);
    text->set_content(content);
    break;
  }
  case WhiteboardElementsType::Picture: {
    protobuf::Picture *pic = ele.mutable_picture();
    protobuf::Point *center = pic->mutable_center();
    auto pic_center = get_picture_center();
    auto pic_content = get_picture_content();
    auto pic_filename = get_picture_filename();
    center->set_x(pic_center.x);
    center->set_y(pic_center.y);
    pic->set_data(pic_content);
    pic->set_file_name(pic_filename);
    break;
  }
  case WhiteboardElementsType::StickyNote: {
    protobuf::StickyNote *note = ele.mutable_stickynote();
    protobuf::Point *center = note->mutable_center();

    // protobuf::Point *end = line.mutable_end();

    auto stickynote_center = get_stickynote_center();
    auto stickynote_side_length = get_stickynote_side_length();
    auto stickynote_content = get_stickynote_content();

    center->set_x(stickynote_center.x);
    center->set_y(stickynote_center.y);
    note->set_side_length(stickynote_side_length);
    note->set_content(stickynote_content);
    break;
  }
  case WhiteboardElementsType::Null:
    break;
  }
  return ele;
}

WhiteboardElements
WhiteboardElements::from_protobuf(const protobuf::Element &ele) {
  WhiteboardElements whiteboardElement;
  if (ele.has_path()) {
    const protobuf::Path &path = ele.path();
    std::vector<Point> pathPoints;
    for (const auto &point : path.points()) {
      pathPoints.push_back({point.x(), point.y()});
    }
    whiteboardElement.new_path(pathPoints);
  } else if (ele.has_line()) {
    const protobuf::Line &line = ele.line();
    const protobuf::Point &start = line.start();
    const protobuf::Point &end = line.end();
    whiteboardElement.new_line({start.x(), start.y()}, {end.x(), end.y()});
  } else if (ele.has_circle()) {
    const protobuf::Circle &circle = ele.circle();
    const protobuf::Point &center = circle.center();
    whiteboardElement.new_circle({center.x(), center.y()}, circle.radius());
  } else if (ele.has_triangle()) {
    const protobuf::Triangle &triangle = ele.triangle();
    const protobuf::Point &point0 = triangle.point1();
    const protobuf::Point &point1 = triangle.point2();
    const protobuf::Point &point2 = triangle.point3();
    whiteboardElement.new_triangle({point0.x(), point0.y()},
                                   {point1.x(), point1.y()},
                                   {point2.x(), point2.y()});
  } else if (ele.has_square()) {
    const protobuf::Square &square = ele.square();
    const protobuf::Point &topleft = square.topleft();
    whiteboardElement.new_square({topleft.x(), topleft.y()},
                                 square.side_length());
  } else if (ele.has_text()) {
    const protobuf::Text &text = ele.text();
    const protobuf::Point &center = text.center();
    whiteboardElement.new_text({center.x(), center.y()}, text.content());
  } else if (ele.has_stickynote()) {
    const protobuf::StickyNote &note = ele.stickynote();
    const protobuf::Point &center = note.center();
    whiteboardElement.new_stickynote({center.x(), center.y()},
                                     note.side_length(), note.content());
  } else if (ele.has_picture()) {
    const protobuf::Picture &pic = ele.picture();
    const protobuf::Point &center = pic.center();
    const std::string pic_content = pic.data();
    const std::string pic_filename = pic.file_name();
    whiteboardElement.new_picture({center.x(), center.y()}, pic_content,
                                  filename);
  } else {
  }
  return whiteboardElement;
}

void WhiteboardElements::print() {
  std::cout << " ---------------------------\n";
  switch (type) {
  case WhiteboardElementsType::Path: {
    auto vec = get_path_points();
    std::cout << "    TYPE: Path\n";
    std::cout << "    Path_points(" << vec.size() << "):\n";

    for (auto &v : vec) {
      std::cout << "      " << v.str() << "\n";
    }
    break;
  }
  case WhiteboardElementsType::Line: {
    auto start = get_line_start();
    auto end = get_line_end();
    std::cout << "    TYPE: Line\n";
    std::cout << "      start: " << start.str() << "\n";
    std::cout << "      end  : " << end.str() << "\n";
    break;
  }
  case WhiteboardElementsType::Circle: {
    auto center = get_circle_center();
    auto radius = get_circle_radius();
    std::cout << "    TYPE: Circle\n";
    std::cout << "      center: " << center.str() << "\n";
    std::cout << "      radius  : " << radius << "\n";
    break;
  }
  case WhiteboardElementsType::Triangle: {
    auto points = get_triangle_points();
    std::cout << "    TYPE: Triangle\n";
    std::cout << "      point1: " << points[0].str() << "\n";
    std::cout << "      point2: " << points[1].str() << "\n";
    std::cout << "      point3: " << points[2].str() << "\n";
    break;
  }
  case WhiteboardElementsType::Square: {
    auto side_length = get_square_side_length();
    auto topleft = get_square_topleft();
    std::cout << "    TYPE: Square\n";
    std::cout << "      side_length: " << side_length << "\n";
    std::cout << "      topleft: " << topleft.str() << "\n";
    break;
  }
  case WhiteboardElementsType::Text: {
    auto center = get_text_center();
    auto content = get_text_content();
    std::cout << "    TYPE: Text\n";
    std::cout << "      center : " << center.str() << "\n";
    std::cout << "      content: " << content << "\n";
    break;
  }
  case WhiteboardElementsType::Picture:
  case WhiteboardElementsType::StickyNote: {
    auto center = get_stickynote_center();
    auto side_length = get_stickynote_side_length();
    auto content = get_stickynote_content();
    std::cout << "    TYPE: StickyNote\n";
    std::cout << "      center     : " << center.str() << "\n";
    std::cout << "      side_length: " << side_length << "\n";
    std::cout << "      content    : " << content << "\n";
    break;
  }
  case WhiteboardElementsType::Null: {
    std::cout << "    Empty element\n";
    break;
  }
  }
  std::cout << " ---------------------------\n";
}