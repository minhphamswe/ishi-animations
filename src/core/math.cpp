#include <algorithm>
#include <cmath>

#include <core/math.h>

bool feq(float x, float y) {
  // Difference is less than tiny fraction of the largest number
  const float eps = std::numeric_limits<float>::epsilon();

  return std::fabs(x - y) <= eps * std::max(std::fabs(x), std::fabs(y));
}