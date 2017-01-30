#ifndef ISHI_CORE_BBOX_H_
#define ISHI_CORE_BBOX_H_

#include <core/point.h>

namespace ishi {

/** Represents an axis-aligned bounding box */
class BBox {
 public:
  Point pMin;
  Point pMax;

 public:
  /// Create a degenerate bounding box
  BBox();
  /// Create a bounding box around one point
  explicit BBox(const Point &p);

  /// Create a bounding box to bound two points
  BBox(const Point &p1, const Point &p2);
};

/// Return the common bounds on a bounding box and another point
BBox Union(const BBox &b, const Point &p);

/// Return the space bounding both bounding boxes
BBox Union(const BBox &b1, const BBox &b2);

/// Return true if two bounding boxes overlap, false otherwise
bool Overlaps(const BBox &b1, const BBox &b2);

}  // namespace ishi

#endif
