#ifndef ISHI_SHAPES_SHAPE_H_
#define ISHI_SHAPES_SHAPE_H_

#include <core/transform.h>
#include <core/bbox.h>
#include <core/renderer.h>

namespace ishi {

// class Renderer;

/**
 * The Shape class is a container for geometric information and geometric
 * methods (and those only).
 */
class Shape {
 public:
  Transform *ObjectToWorld;
  Transform *WorldToObject;
  int shapeID;                // ID of this shape. Used for vertex list
  static int nextShapeID;     // The next ID to be assigned

 public:
  Shape(Transform *o2w, Transform *w2o);
  virtual ~Shape();

  /// Return the bounding box of the shape in object space
  /// @note This method must be overridden by derived classes
  virtual BBox ObjectBound() const = 0;

  /// Return the bounding box of the shape, in world space
  /// @note A default implementation is provided, but shapes should override
  /// this method if it can compute a tighter bounding box
  virtual BBox WorldBound() const;

  virtual void Render(const Renderer &r) = 0;
};

}  // namespace ishi

#endif
