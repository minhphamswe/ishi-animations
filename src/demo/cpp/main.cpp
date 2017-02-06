// C++ library includes
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>

#include <core/color.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/transform.h>
#include <core/bbox.h>

#include "./common.h"

#include "./types.h"
#include "./bvh_defs.h"
#include "./joint.h"
#include "./loader.h"
#include "./geom.h"

using namespace std;
using namespace ishi;

// window parameters
int window_width = 800, window_height = 600;
float window_aspect = window_width/static_cast<float>(window_height);

// callback prototypes
void InitGL();
void Display();
void Resize(int width, int height);
void Keyboard(unsigned char key, int x, int y);
void SpecialKeys(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Idle();

void SetLighting();

// Application initialization
void Init();

#define PI 3.14159265f

vector<SceneGraph> sg;    // Vector of scene graphs
vector<Color> sgc;        // Vector of scene graph colors

Point eye, center;        // Position of camera, focal point
Vector up;                // The up direction for the camera

BBox bbox = BBox(Point(-100, -100, -100), Point(100, 100, 100));
float maxDist;

Transform orbitLeft = RotateY(-0.1f);        // Orbitting left transform

bool showAxis = true;       // If true, show axis
bool showBounds = false;    // If true, show bounding box
bool showFloor = true;      // If true, draw the floor
bool animate = false;       // If true, animate character

int prevTime;     // The last time (in millisecond) the timer was queried

int prevX;        // The X coordinate where the mouse was last clicked
int prevY;        // The Y coordinate where the mouse was last clicked
int prevButton;   // The last mouse button that was clicked

float axisLen = 1.0f;   // The rendered length of the basis normals

// lighting configurations
GLfloat light_position[] = {0.f, 50.f, 0.f};
GLfloat light_ambient[] = {0.25, 0.25, 0.25};
GLfloat light_diffuse[] = {0.25, 0.25, 0.25};
GLfloat light_specular[] = {1.f, 1.f, 1.f};

// material configurations
GLfloat mat_ambient[] = {0.5, 0.5, 0.5};
GLfloat mat_diffuse[] = {0.5, 0.5, 0.5};
GLfloat mat_specular[] = {1.f, 1.f, 1.f};
GLfloat mat_shininess[] = {100.f};

void InitGL() {
  // Perform any necessary GL initialization in this function

  // enable depth testing, and set clear color to white
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  SetLighting();

  Init();

  // resize the window
  Resize(window_width, window_height);
}

void ComputeLookAt() {
  // Recompute world center and bound
  maxDist = Length(bbox.pMax - bbox.pMin);
  center = (bbox.pMax + bbox.pMin)/2.0f;
  axisLen = maxDist*0.05f;
}

void Init() {
  // Compute world bounds
  ComputeLookAt();

  // Compute default camera position
  up = Vector(0.0f, 1.0f, 0.0f);
  eye = center + Vector(0.5f*maxDist, 0.75f*maxDist, 1.5f*maxDist);

  // Zero out the position of the scene graph
  for (unsigned int i = 0; i < sg.size(); i++)
    sg[i].SetCurrentFrame(0);
}

void SetLighting() {
  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);

  // Set light configurations for the global light source
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // Set material lighting configurations
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void SetCamera() {
  gluLookAt(eye[0], eye[1], eye[2],
            center[0], center[1], center[2],
            up[0], up[1], up[2]);
}

void SetProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(40.0, window_aspect, 1, 1500);
}

void SetDrawMode() {
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);
}

void DrawAxis() {
  Point c = (bbox.pMin+bbox.pMax) * 0.5f;
  float L = Length(bbox.pMax-bbox.pMin) * 0.2;

  Vector X(L, 0, 0), Y(0, L, 0), Z(0, 0, L);

  glLineWidth(2.0);

  glBegin(GL_LINES);

  glColor3f(1, 0, 0);
  glVertex3f(c.x, c.y, c.z);
  glVertex3f((c+X).x, (c+X).y, (c+X).z);

  glColor3f(0, 1, 0);
  glVertex3f(c.x, c.y, c.z);
  glVertex3f((c+Y).x, (c+Y).y, (c+Y).z);

  glColor3f(0, 0, 1);
  glVertex3f(c.x, c.y, c.z);
  glVertex3f((c+Z).x, (c+Z).y, (c+Z).z);

  glEnd();
}

void DrawFloor(float W, float H, float w, float h) {
  float a = H/h, b = W/w;
  int M = static_cast<int>(floor(a+0.5f));
  int N = static_cast<int>(floor(b+0.5f));
  int i = 0, j = 0;
  Vec3f u = {{w, 0, 0}}, v = {{0, 0, h}}, r = {{-(N/2)*w, 0, -(M/2)*h}};
  Vec3f p0, p1, p2, p3;
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glBegin(GL_QUADS);
  glColor3f(0.7, 0.7, 0.7);
  for (j = 0; j < N; j++) {
    p0 = r+u*static_cast<float>(j%2);
    for (i = j%2; i < M; i += 2) {
      p1 = p0+u;
      p2 = p1+v;
      p3 = p2-u;

      glVertex3fv(p3.x);
      glVertex3fv(p2.x);
      glVertex3fv(p1.x);
      glVertex3fv(p0.x);

      p0 += u*2.0f;
    }
    r += v;
  }
  glEnd();
}

void DrawRect(const Point &base, const Vector &ext1, const Vector &ext2) {
  Point tmp;
  glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 1);

    glVertex3fv(&(base.x));
    tmp = base + ext1;
    glVertex3fv(&(tmp.x));

    tmp = base + ext1 + ext2;
    glVertex3fv(&(tmp.x));

    tmp = base + ext2;
    glVertex3fv(&(tmp.x));
    glVertex3fv(&(base.x));
  glEnd();
}

void DrawBounds() {
  Point u, v, m1[] = {bbox.pMin, bbox.pMax}, m2[] = {bbox.pMax, bbox.pMin};
  Vector uu, vv;

  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 3; i++) {
      for (int j = i+1; j < 3; j++) {
        u = m1[k];
        v = m1[k];
        u[i] = m2[k][i];
        v[j] = m2[k][j];
        uu = u - m1[k];
        vv = v - m1[k];
        DrawRect(m1[k], uu, vv);
      }
    }
  }
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SetProjection();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  SetLighting();
  SetCamera();
  SetDrawMode();

  if (showAxis) DrawAxis();
  if (showBounds) DrawBounds();
  if (showFloor) DrawFloor(800, 800, 80, 80);

  // Render scene graph
  for (unsigned int i = 0; i < sg.size(); i++) {
    glColor3f(sgc[i].r, sgc[i].g, sgc[i].b);
    sg[i].root->Render();
  }

  glFlush();          // finish the drawing commands
  glutSwapBuffers();  // and update the screen
}

// This reshape function is called whenever the user
// resizes the display window.
void Resize(int width, int height) {
  window_width = width;
  window_height = height;
  float fheight = static_cast<float>(height);
  window_aspect = width / fheight;

  // resize the window
  glViewport(0, 0, window_width, window_height);

  SetProjection();

  // setup basic orthographic projection
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // let glut know to redraw the screen
  glutPostRedisplay();
}

// This function is called whenever the user hits letters or numbers
// on the keyboard.  The 'key' variable has the character the user hit,
// and x and y tell where the mouse was when it was hit.
void Keyboard(unsigned char key, int x, int y) {
  y = window_height - y;
  if (key == '1') {
    eye = center + Vector(0.5f*maxDist, 0.75f*maxDist, 1.5f*maxDist);
    ComputeLookAt();
  } else if (key =='2') {
    eye = center + Vector(0, 0.1f*maxDist, 1.5f*maxDist);
    ComputeLookAt();
  } else if (key =='3') {
    eye = center + Vector(1.5f*maxDist, 0.1f*maxDist, 0);
    ComputeLookAt();
  } else if (key =='z') {
    eye = center + ((eye - center) * 0.95);
    ComputeLookAt();
  } else if (key =='Z') {
    eye = center + ((eye - center) * 1.05);
    ComputeLookAt();
  } else if (key =='j') {
    orbitLeft.Apply(&eye);
    ComputeLookAt();
  } else if (key =='k') {
    orbitLeft.ApplyInvert(&eye);
    ComputeLookAt();
  } else if (key ==' ') {
    animate = !animate;
  } else if (key =='a') {
    showAxis = !showAxis;
  } else if (key =='b') {
    showBounds = !showBounds;
  } else if (key =='f') {
    showFloor = !showFloor;
  } else if (key =='q' || key ==27 /* esc */) {
    exit(0);
  }

  // let glut know to redraw the screen
  glutPostRedisplay();
}

/// Called whenever a special key is pressed
void SpecialKeys(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT) {
    orbitLeft.Apply(&eye);
  } else if (key == GLUT_KEY_RIGHT) {
    orbitLeft.ApplyInvert(&eye);
  } else if (key == GLUT_KEY_UP) {
    eye = Rotate(0.057, Cross(up, center - eye))(eye);
  } else if (key == GLUT_KEY_DOWN) {
    eye = Rotate(-0.057, Cross(up, center - eye))(eye);
  }
  ComputeLookAt();
  glutPostRedisplay();
}

/// Called whenever a mouse button is pressed
void Mouse(int button, int state, int x, int y) {
  if (button == 3) /* mouse wheel up */ {
    if (state == GLUT_DOWN) {
      eye = center + ((eye - center) * 0.95);
      ComputeLookAt();
    }
  } else if (button == 4) /* mouse wheel down */ {
    if (state == GLUT_DOWN) {
      eye = center + ((eye - center) * 1.05);
      ComputeLookAt();
    }
  } else if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      prevX = x;
      prevY = y;
      prevButton = GLUT_LEFT_BUTTON;
    } else if (state == GLUT_UP) {
      prevButton = -1;
    }
  }
  glutPostRedisplay();
}

/// Called whenever a mouse button is pressed the mouse is moved
void Motion(int x, int y) {
  if (prevButton == GLUT_LEFT_BUTTON) {
    float deltaX = (prevX - x) / static_cast<float>(window_width);
    float deltaY = (prevY - y) / static_cast<float>(window_height);
    eye = Rotate(-deltaY, Cross(up, center - eye))(eye);
    eye = RotateY(2 * deltaX)(eye);
    ComputeLookAt();
    prevX = x;
    prevY = y;
  }
  glutPostRedisplay();
}

/// Called whenever the program is not busy doing something else
void Idle() {
  // Calculate elapsed frames since last tick
  int currentTime = glutGet(GLUT_ELAPSED_TIME);
  int frameDelta;

  for (unsigned int i = 0; i < sg.size(); i++) {
    frameDelta = (currentTime - prevTime) * sg[i].FramePerMs();

    if (animate && frameDelta > 0)
      // If animating and enough time has passed:
      // raise frame index && update position for all joints
      sg[i].SetCurrentFrame((sg[i].GetCurrentFrame() + frameDelta));
  }

  // Save tick time
  prevTime = currentTime;

  // update the screen
  glutPostRedisplay();
}

void processCommandLine(int argc, char *argv[]) {
  if (argc>1) {
    for (int i = 1; i < argc; i++) {
      char filename[500];
      SceneGraph *s = new SceneGraph;

      snprintf(&(filename[0]), strlen(argv[i])+1, "%s", argv[i]);
      BVHLoader::loadBVH(filename, s);
      sg.push_back(*s);

      float r = static_cast<float>((i + 15) % 3) / 3;
      float g = static_cast<float>((i + 16) % 3) / 3;
      float b = static_cast<float>((i + 17) % 3) / 3;
      sgc.push_back(Color(r, g, b));
    }
    ComputeLookAt();
  } else {
    printf("Filename argument required.\n");
    exit(0);
  }
}

void showMenu() {
  cout << "------------ menu ------------" << endl;
  cout << "q - quit" << endl;
  cout << "a - show/hide axis" << endl;
  cout << "b - show/hide bounds" << endl;
  cout << "f - show/hide floor" << endl;
  cout << "[1-3] - move to waypoint" << endl;
  cout << "z - zoom in" << endl;
  cout << "Z - zoom out" << endl;
  cout << "j - rotate left" << endl;
  cout << "k - rotate right" << endl;
  cout << "[MOUSE WHEEL] - zoom in/out" << endl;
  cout << "[SPACE] - start/stop" << endl;
}

int main(int argc, char *argv[]) {
   glewInit();

  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 100);
  // Do not change the name of the window -- it is needed for the screenshot
  glutCreateWindow("Mocap");
  glutDisplayFunc(Display);
  glutReshapeFunc(Resize);
  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(SpecialKeys);
  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  glutIdleFunc(Idle);

  processCommandLine(argc, argv);

  showMenu();

  InitGL();

  glutMainLoop();

  return 0;
}
