#include <core/bbox.h>
#include <core/geometry.h>
#include <core/common.h>

namespace ishi {

BBox::BBox()
    : pMin(INFINITY, INFINITY, INFINITY),
      pMax(-INFINITY, -INFINITY, -INFINITY) {}

BBox::BBox(const Point& p)
    : pMin(p), pMax(p) {}

BBox::BBox(const Point& p1, const Point& p2) {
  pMin = Point(Min(p1.x, p2.x), Min(p1.y, p2.y), Min(p1.z, p2.z));
  pMax = Point(Max(p1.x, p2.x), Max(p1.y, p2.y), Max(p1.z, p2.z));
}

BBox Union(const BBox& b, const Point& p) {
  BBox r;
  r.pMin.x = Min(b.pMin.x, p.x);
  r.pMin.y = Min(b.pMin.y, p.y);
  r.pMin.z = Min(b.pMin.z, p.z);
  r.pMax.x = Max(b.pMax.x, p.x);
  r.pMax.y = Max(b.pMax.y, p.y);
  r.pMax.z = Max(b.pMax.z, p.z);
  return r;
}

BBox Union(const BBox& b1, const BBox& b2) {
  BBox r;
  r.pMin.x = Min(b1.pMin.x, b2.pMin.x);
  r.pMin.y = Min(b1.pMin.y, b2.pMin.y);
  r.pMin.z = Min(b1.pMin.z, b2.pMin.z);
  r.pMax.x = Max(b1.pMax.x, b2.pMax.x);
  r.pMax.y = Max(b1.pMax.y, b2.pMax.y);
  r.pMax.z = Max(b1.pMax.z, b2.pMax.z);
  return r;
}

bool Overlaps(const BBox& b1, const BBox& b2) {
  bool x = (b1.pMin.x <= b2.pMax.x) && (b1.pMax.x >= b2.pMin.x);
  bool y = (b1.pMin.y <= b2.pMax.y) && (b1.pMax.y >= b2.pMin.y);
  bool z = (b1.pMin.z <= b2.pMax.z) && (b1.pMax.z >= b2.pMin.z);
  return x && y && z;
}

}  // namespace ishi
