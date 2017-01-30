#ifndef __JOINT_H__
#define __JOINT_H__

#include <core/point.h>
#include <core/vector.h>
#include <core/transform.h>

#include <vector>
#include <cstring>
#include <string>

#include "./bvh_defs.h"
#include "./vec.h"

using namespace std;
using namespace ishi;

class Segment;
class SceneGraph;

class Segment {
 public:
  /* Identification information */
  uint32_t id;
  std::string name;

  /* Hierarchy information */
  vector<Segment*> chd;   // pointers to child nodes
  Segment *par;           // pointer to parent node
  Transform w2o;          // world space to object space transformation

  /* Geometric information */
  Vector offset;
  Point endpoint;
  Point basepoint;

  /* Motion information */
  uint16_t numChannels;       // number of channels (movement types) this has
  vector<int> channelOrder;   // how to interpret motion data
  uint16_t channelFlags;      // bit mask specifying available channels
  uint32_t frameIndex;        // index of the motion frame this is at

  vector<vector<float> > frameData;   // all frame data for this node

 public:
  /// Initialize a node
  Segment(const char *name, uint32_t id);

  /// Return true if the segment is the root segment
  bool IsRoot();

  /// Return true if the segment is an endsite
  bool IsEndSite();
  void Update();

  void DistributeFrame(float** data);

  /// Called to render all nodes from this node down
  void Render();
};

class SceneGraph {
 private:
  vector<Segment*> nodes;     // list of all nodes

  uint32_t numFrames;         // how many frames there are in total
  uint32_t frameSize;         // how many data points each frame has
  float frameTime;            // time between each frame (in milliseconds)
  float invFrameTime;         // number of frames per millisecond
  uint32_t currentFrame;      // index of the motion frame this is at

 public:
  Segment *root;              // point to root of the scene graph tree

 public:
  /// Initialize a SceneGraph
  SceneGraph() {
    nodes = vector<Segment*>();
  }

  /*  Hierarchy Specification methods */
  /// Create the root node
  void CreateRoot(const char * name, uint32_t id);
  void CreateJoint(const char * name, uint32_t id);
  void CreateEndSite(const char * name, uint32_t id);

  /// Set the child of some node to another node
  void SetChild(uint32_t parent, uint32_t child);

  /*  Animation-related methods */
  void SetOffset(uint32_t id, float * offset);
  void SetNumChannels(uint32_t id, uint16_t num);
  void SetChannelFlags(uint32_t id, uint16_t flags);
  void SetChannelOrder(uint32_t id, int * order);
  void SetFrameIndex(uint32_t id, uint32_t index);
  void SetFrameTime(float delta);
  void SetNumFrames(uint32_t num);
  void SetFrameSize(uint32_t size);
  void AddFrame(float * data);
  void SetCurrentFrame(uint32_t frameNumber);

  /// Return the time between frames, in milliseconds
  float MsPerFrame();

  /// Return the number of frames per millisecond
  float FramePerMs();

  /// Return the current frame index
  uint32_t GetCurrentFrame();
};


#endif

