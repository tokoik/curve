#if defined(__APPLE__)
#  define GL_SILENCE_DEPRECATION
#  include <GLUT/glut.h>
#else
#  if defined(_WIN32)
//#    pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#    define _USE_MATH_DEFINES
#    define _CRT_SECURE_NO_WARNINGS
#  endif
// macOS 以外では通常の GLUT ヘッダーを読み込む。
#  include <GL/glut.h>
#endif
#include <cstdlib>
#include <cmath>
#include <vector>

// 曲線の関数
#include "curve.h"

// 3 次元座標を 1 点分保持する構造体
struct vec3 { float p[3]; };

// マウスで入力された制御点列
static std::vector<vec3> point;

// 各制御点に対応するパラメータ値
static std::vector<float> key;

// 1 区間の分割数
#define STEP 10

//
// 描画
//
static void display()
{
  // カラーバッファを消去する
  glClear(GL_COLOR_BUFFER_BIT);

  // 制御点が 1 つ以上あれば
  if (point.size() > 0)
  {
    // 制御点を５ピクセルの青い点で描く
    glPointSize(5.0);
    glColor3d(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, sizeof (vec3), point[0].p);
    glDrawArrays(GL_POINTS, 0, (GLsizei)point.size());

    // 補間曲線を赤色で描く
    glColor3d(1.0, 0.0, 0.0);

    // 隣接する制御点対ごとに 1 区間の曲線を生成する
    for (std::vector<vec3>::iterator i = point.begin(); i != point.end() - 1; ++i)
    {
      // 1 区間分の描画点列
      float p[STEP + 1][3];

      // 区間の開始パラメータ
      float t = (float)(i - point.begin());

      // 区間を等分分割しながら補間点を順に計算する
      for (int j = 0; j <= STEP; ++j)
      {
        // 曲線上の 1 点を計算して配列に保存する
        curve(p[j], &point[0].p, &key[0], (int)point.size(), t + (float)j / (float)STEP);
      }

      // 計算した補間点列を折れ線で描画する
      glVertexPointer(3, GL_FLOAT, 0, p);
      glDrawArrays(GL_LINE_STRIP, 0, STEP + 1);
    }
  }

  // ここまでの描画命令を実行に反映する。
  glFlush();
}

//
// ウィンドウサイズ変更時の処理
//
static void resize(int w, int h)
{
  // ウィンドウ全体をビューポートにする
  glViewport(0, 0, w, h);

  // 現在の変換行列を単位行列で初期化する
  glLoadIdentity();

  // スクリーン上の座標系をマウスの座標系に一致させる
  glOrtho(-0.5, (GLfloat)w - 0.5, (GLfloat)h - 0.5, -0.5, -1.0, 1.0);
}

//
// マウス入力時の処理
//
static void mouse(int button, int state, int x, int y)
{
  // 押されたマウスボタンごとに
  switch (button)
  {
  case GLUT_LEFT_BUTTON:

    // 左ボタンが押された瞬間だけ制御点を追加する
    if (state == GLUT_DOWN)
    {
      // 新しい制御点に対応するパラメータ値を追加する
      key.push_back(static_cast<float>(point.size()));

      // マウス位置を z=0 の 3 次元点として生成する
      vec3 p = { { static_cast<float>(x), static_cast<float>(y), 0.0f } };

      // 生成した点を制御点列の末尾に追加する
      point.push_back(p);

      // 表示更新を要求して追加結果を描画させる
      glutPostRedisplay();
    }
    break;

  case GLUT_RIGHT_BUTTON:

    // 右ボタンが押されたら制御点列を消去する
    if (state == GLUT_DOWN)
    {
      // すべての制御点を削除する
      point.clear();

      // 表示更新を要求して画面を消去させる
      glutPostRedisplay();
    }
    break;
  default:

    // 未対応のボタン入力は無視する
    break;
  }
}

//
// キーボード入力時の処理
//
static void keyboard(unsigned char key, int x, int y)
{
  // 押されたキーごとに
  switch(key)
  {
  case '\033':
  case 'q':
  case 'Q':

  // Esc または q/Q が押されたらアプリケーションを終了する
    exit(0);
  }
}

//
// OpenGL の初期設定
//
static void init()
{
  // 頂点配列を使う
  glEnableClientState(GL_VERTEX_ARRAY);

  // 背景色
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

//
// メインプログラム
//
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
