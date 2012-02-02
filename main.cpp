#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
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

#define MAXPOINTS 1000
static float point[MAXPOINTS][3];
static float key[MAXPOINTS];
static int count = 0;

#define STEP 10

static void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glPointSize(5.0);
  glColor3d(0.0, 1.0, 0.0);
  glBegin(GL_POINTS);
  for (int i = 0; i < count; ++i) glVertex3fv(point[i]);
  glEnd();

  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < count - 1; ++i) {
    for (int j = 0; j < STEP; ++j) {
      float p[3];
      curve(p, point, key, count, (float)i + (float)j * 0.1f);
      glVertex3fv(p);
    }
  }
  glEnd();

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
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      if (count < MAXPOINTS) {
        point[count][0] = x;
        point[count][1] = y;
        point[count][2] = 0.0;
        key[count] = (float)count;
        ++count;
        glutPostRedisplay();
      }
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    count = 0;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

static void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case '\033':
  case 'q':
  case 'Q':
    exit(0);
  }
}

static void init(void)
{
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
