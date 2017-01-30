#include <core/bsphere.h>

#include <core/bbox.h>
#include <core/common.h>

namespace ishi {

bool BSphere::Overlaps(const BBox& b) const {
  float sd = 0.f;     // Square of distance between bounding sphere and box

  sd += (c.x < b.pMin.x) ? Square(c.x - b.pMin.x) : 0;
  sd += (c.x > b.pMax.x) ? Square(c.x - b.pMax.x) : 0;

  sd += (c.y < b.pMin.y) ? Square(c.y - b.pMin.y) : 0;
  sd += (c.y > b.pMax.y) ? Square(c.y - b.pMax.y) : 0;

  sd += (c.z < b.pMin.z) ? Square(c.z - b.pMin.z) : 0;
  sd += (c.z > b.pMax.z) ? Square(c.z - b.pMax.z) : 0;

  return sd < Square(c);
}

}  // namespace ishi
