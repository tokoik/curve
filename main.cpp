#include <cstdlib>
#include <cmath>
#include <vector>
 
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

#include "curve.h"

struct vec3 { float p[3]; };
static std::vector<vec3> point;
static std::vector<float> key;

#define STEP 10

static void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  if (point.size() > 0)
  {
    glPointSize(5.0);
    glColor3d(0.0, 1.0, 0.0);
    glVertexPointer(3, GL_FLOAT, sizeof (vec3), point[0].p);
    glDrawArrays(GL_POINTS, 0, point.size());

    glColor3d(1.0, 0.0, 0.0);
    for (std::vector<vec3>::iterator i = point.begin(); i != point.end() - 1; ++i)
    {
      float p[STEP + 1][3], t = (float)(i - point.begin());

      for (int j = 0; j <= STEP; ++j)
      {
        curve(p[j], &point[0].p, &key[0], point.size(), t + (float)j / (float)STEP);
      }

      glVertexPointer(3, GL_FLOAT, 0, p);
      glDrawArrays(GL_LINE_STRIP, 0, STEP + 1);
    }
  }

  glFlush();
}

static void resize(int w, int h)
{
  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);

  /* 変換行列の初期化 */
  glLoadIdentity();

  /* スクリーン上の座標系をマウスの座標系に一致させる */
  glOrtho(-0.5, (GLfloat)w - 0.5, (GLfloat)h - 0.5, -0.5, -1.0, 1.0);
}

static void mouse(int button, int state, int x, int y)
{
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      key.push_back(static_cast<float>(point.size()));
      vec3 p = { { static_cast<float>(x), static_cast<float>(y), 0.0f } };
      point.push_back(p);
      glutPostRedisplay();
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
    {
      point.clear();
      glutPostRedisplay();
    }
    break;
  default:
    break;
  }
}

static void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
  case '\033':
  case 'q':
  case 'Q':
    exit(0);
  }
}

static void init(void)
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow("Catmull-Rom Spline Curve");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
