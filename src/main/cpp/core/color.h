#ifndef CORE_COLOR_H_
#define CORE_COLOR_H_

namespace ishi {

class Color {
 public:
  float r, g, b, a;

 public:
  Color(float rr, float gg, float bb);
  Color(float rr, float gg, float bb, float aa);
};

}  // namespace ishi

#endif
