#ifndef ISHI_RENDERING_RENDERER_H_
#define ISHI_RENDERING_RENDERER_H_

#include <stdio.h>

namespace ishi {

class Shape;
class TriangleMesh;
class Triangle;
class Quad;

/**
 * Interface for all renders. Implemented using the Visitor pattern.
 */
class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  virtual void Render(const Shape &s) const = 0;
  virtual void Render(const TriangleMesh &s) const = 0;
  virtual void Render(const Triangle &t) const = 0;
  virtual void Render(const Quad &q) const = 0;
};

}  // namespace ishi

#endif
