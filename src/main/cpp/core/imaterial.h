#ifndef ISHI_CORE_MATERIAL_H_
#define ISHI_CORE_MATERIAL_H_

#include <core/vector.h>

namespace ishi {

class Texture {
 private:
  Vector a;   // Material ambient component
  Vector d;   // Material diffuse component
  Vector s;   // Material specular component
  float sk;   // Material specular coefficient

 public:
  Texture(const Vector &ambient,
          const Vector &diffuse,
          const Vector &specular,
          float specular_coefficient);

  /// Get the ambient component
  Vector Ambient() const;

  /// Get the diffuse component
  Vector Diffuse() const;

  /// Get the specular component
  Vector Specular() const;

  /// Get the specular coefficient
  float SpecularCoefficient() const;
};

}   // namespace ishi

#endif
