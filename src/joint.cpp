#include <core/common.h>
#include <core/vector.h>
#include <core/transform.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdint.h>
#include <iostream>
#include <vector>
#include <cstdio>

#include "./bvh_defs.h"
#include "./joint.h"

using namespace std;
using namespace ishi;

/* Segment methods */

Segment::Segment(const char* name, uint32_t id) {
  /* Identification information */
  this->id = id;
  this->name = name;

  /* Hierarchy information */
  this->par = NULL;
  this->w2o = Transform();

  /* Geometric information */
  this->offset = Vector();    // in local coordinates
  this->endpoint = Point();   // in world coordinates
  this->basepoint = Point();  // in world coordinates

  /* Motion information */
  this->numChannels = 0;
}

/// The root node is defined as the node without a parent
bool Segment::IsRoot() {
  return (par == NULL);
}

/// The end note is defined as the node without any child
bool Segment::IsEndSite() {
  return (chd.size() == 0);
}

void Segment::Update() {
  vector<float> frame = frameData[frameIndex];  // Data for this frame
  Vector trans = Vector(0, 0, 0);               // Translation vector
  Transform rot = Transform();                  // Rotation data holder

  for (unsigned int i = 0; i < numChannels; i++) {
    // Get channel to update by order
    float f = frame[i];         // The data point
    int c = channelOrder[i];    // The channel it applies to

    // Read frame data
    if (c == BVH_XPOS_IDX && (channelFlags & BVH_XPOS)) {
      trans.x = f;
    } else if (c == BVH_YPOS_IDX && (channelFlags & BVH_YPOS)) {
      trans.y = f;
    } else if (c == BVH_ZPOS_IDX && (channelFlags & BVH_ZPOS)) {
      trans.z = f;
    } else if (c == BVH_XROT_IDX && (channelFlags & BVH_XROT)) {
      if (f != 0)
        rot = rot * RotateX(Radian(f));
    } else if (c == BVH_YROT_IDX && (channelFlags & BVH_YROT)) {
      if (f != 0)
        rot = rot * RotateY(Radian(f));
    } else if (c == BVH_ZROT_IDX && (channelFlags & BVH_ZROT)) {
      if (f != 0)
        rot = rot * RotateZ(Radian(f));
    }
  }

  // Recompute world-to-object transformation
  if (par)
    this->w2o = par->w2o * Translate(trans + this->offset) * rot;
  else
    this->w2o = Translate(trans + this->offset) * rot;

  // Recompute basepoint
  this->basepoint = this->w2o(Point());

  // Recompute endpoint
  if (chd.size() > 0)
    this->endpoint = this->w2o(Point()+chd[0]->offset);
  else
    this->endpoint = this->basepoint;

  // Do the same for all children (order doesn't matter)
  for (unsigned int i = 0; i < chd.size(); i++) {
    chd[i]->frameIndex = this->frameIndex;
    chd[i]->Update();
  }
}

void Segment::DistributeFrame(float** data) {
  // Distribute to self first
  vector<float> frame;
  for (unsigned int i = 0; i < numChannels; i++) {
    frame.push_back(**data);
    (*data)++;
  }
  frameData.push_back(frame);

  // Then distribute to the children (in order)
  for (unsigned int i = 0; i < chd.size(); i++)
    chd[i]->DistributeFrame(data);
}

void Segment::Render() {
  std::cout << "Rendering" << std::endl;
  // Render this node
  Vector dir = Inverse(w2o)(endpoint-basepoint);
  float scale = 1.25 * Length(dir);

  // Draw wireframe "muscle"
  glBegin(GL_LINES);
    glVertex3f(basepoint.x, basepoint.y, basepoint.z);
    glVertex3f(endpoint.x, endpoint.y, endpoint.z);
  glEnd();

  // Save the modelview transformation state
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    // Go to joint location
    glMultTransposeMatrixf(reinterpret_cast<float*>(w2o.Matrix().m));

    // Stretch sphere to the "muscle" length
    if (!IsEndSite() && !IsRoot()) {
      glMultTransposeMatrixf(
        reinterpret_cast<float*>(Inverse(AlignZ(dir)).Matrix().m));
      glTranslatef(0, 0, scale/2.5);
      glScalef(scale/4, scale/4, scale);
      glMultTransposeMatrixf(reinterpret_cast<float*>(AlignZ(dir).Matrix().m));
    }

    // Draw sphere
    glutSolidSphere(INV_PI, 16, 16);

  // Restore the modelview transformation state
  glPopMatrix();

  // Then render all children
  for (unsigned int i = 0; i < chd.size(); i++)
    chd[i]->Render();
}

/* SceneGraph Methods */
void SceneGraph::CreateRoot(const char * name, uint32_t id) {
  nodes.insert(nodes.begin() + id, new Segment(name, id));
  root = nodes[id];
}

void SceneGraph::CreateJoint(const char * name, uint32_t id) {
  nodes.insert(nodes.begin() + id, new Segment(name, id));
}

void SceneGraph::CreateEndSite(const char * name, uint32_t id) {
  nodes.insert(nodes.begin() + id, new Segment(name, id));
}

void SceneGraph::SetChild(uint32_t parent, uint32_t child) {
  nodes[child]->par = nodes[parent];
  nodes[parent]->chd.push_back(nodes[child]);
}

void SceneGraph::SetOffset(uint32_t id, float * offset) {
  nodes[id]->offset = Vector(offset[0], offset[1], offset[2]);
}

void SceneGraph::SetNumChannels(uint32_t id, uint16_t num) {
  nodes[id]->numChannels = num;
}

void SceneGraph::SetChannelFlags(uint32_t id, uint16_t flags) {
  nodes[id]->channelFlags = flags;
}

void SceneGraph::SetChannelOrder(uint32_t id, int * order) {
  for (unsigned int i = 0; i < nodes[id]->numChannels; i++) {
    nodes[id]->channelOrder.push_back(*order);
    order++;
  }
}

void SceneGraph::SetFrameIndex(uint32_t id, uint32_t index) {
  nodes[id]->frameIndex = index;
}

void SceneGraph::SetFrameTime(float delta) {
  frameTime = delta * 1000;
  invFrameTime = 1/frameTime;
}

void SceneGraph::SetNumFrames(uint32_t num) {
  numFrames = num;
  printf("Number of frames: %d\n", num);
}

void SceneGraph::SetFrameSize(uint32_t size) {
  frameSize = size;
}

void SceneGraph::AddFrame(float * data) {
  // Distribute frame data to all nodes, starting at the root
  root->DistributeFrame(&data);
}

void SceneGraph::SetCurrentFrame(uint32_t frameNumber) {
  // Frame should loop around if the number of frames is exceeded
  while (frameNumber >= numFrames)
    frameNumber -= numFrames;
  this->currentFrame = frameNumber;

  // Change frame number for all children and request update
  root->frameIndex = frameNumber;
  root->Update();
}

float SceneGraph::MsPerFrame() {
  return frameTime;
}

float SceneGraph::FramePerMs() {
  return invFrameTime;
}

uint32_t SceneGraph::GetCurrentFrame() {
  return currentFrame;
}
