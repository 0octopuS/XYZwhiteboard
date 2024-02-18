#include "element.pb.h"
#include <cstdint>

enum class WhiteboardElementsType : uint8_t {
  Path,
  Line,
  Triangle,
  Square,

};

class Point {
  uint32_t x, y;
};

class WhiteboardElements {
private:
  WhiteboardElementsType type;
  Point pos;

public:
};