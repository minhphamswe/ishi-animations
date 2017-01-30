#include <core/imaterial.h>

namespace ishi {

Texture::Texture(const Vector& ambient, const Vector& diffuse,
                   const Vector& specular, float specular_coefficient)
    : a(ambient), d(diffuse), s(specular), sk(specular_coefficient) {}

Vector Texture::Ambient() const {
  return a;
}

Vector Texture::Diffuse() const {
  return d;
}

Vector Texture::Specular() const {
  return s;
}

float Texture::SpecularCoefficient() const {
  return sk;
}

}   // namespace ishi
