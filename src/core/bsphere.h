#ifndef ISHI_CORE_BSPHERE_H_
#define ISHI_CORE_BSPHERE_H_

namespace ishi {

#include <core/point.h>

class BBox;
class Vector;

/// Represents a bounding sphere
class BSphere {
 private:
  Point c;     // Center of the sphere
  float r;     // Radius of the sphere

 public:
  BSphere();

  BSphere(const Point &center, float radius);

  /// Return true if this sphere and a bounding box overlaps
  bool Overlaps(const BBox &b) const;
};

}  // namespace ishi

#endif
