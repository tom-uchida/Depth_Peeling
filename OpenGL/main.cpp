#include <stdlib.h>
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#  include "glext.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

/*
** Depth Peeling のレイヤー
*/
static int layer = 0;

/*
** 光源
*/
static const GLfloat lightpos[] = { 4.0, 9.0, 5.0, 1.0 }; /* 位置　　　　　　　 */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* 直接光強度　　　　 */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* 環境光強度　　　　 */

/*
** テクスチャ
*/
#define TEXWIDTH  512                                     /* テクスチャの幅　　 */
#define TEXHEIGHT 512                                     /* テクスチャの高さ　 */

// プロトタイプ宣言
static void tile(double w, double d, int nw, int nd);
static void box(double x, double y, double z);
void scene(void);

void trackballInit(void);
void trackballRegion(int w, int h);
void trackballStart(int x, int y);
void trackballMotion(int x, int y);
void trackballStop(int x, int y);
double *trackballRotation(void);


/*
** 初期化
*/
static void init(void)
{
  /* テクスチャの割り当て－このテクスチャを前方のデプスバッファとして使う */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXWIDTH, TEXHEIGHT, 0,
    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  
  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  /* テクスチャの繰り返し方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  /* 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  
  /* もしＲの値がテクスチャの値を超えていたら真（フラグメントを描く） */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
  
  /* 比較の結果をアルファ値として得る */
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
  
  /* アルファテストの比較関数（しきい値） */
  glAlphaFunc(GL_GEQUAL, 0.5f);

  /* テクスチャ座標に視点座標系における物体の座標値を用いる */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

  /* 生成したテクスチャ座標をそのまま (S, T, R, Q) に使う */
  static const GLdouble genfunc[][4] = {
    { 1.0, 0.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 0.0, 1.0 },
  };
  glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
  glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
  glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
  glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

  /* 初期設定 */
  glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  
  /* 光源の初期設定 */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);

  /* Polygon Offset の設定 */
  glPolygonOffset(1.0f, 1.0f);
}


/****************************
** GLUT のコールバック関数 **
****************************/

/* トラックボール処理用関数の宣言 */
#include "trackball.h"

/* シーンを描く関数の宣言 */
#include "scene.h"

static void display(void)
{
  GLdouble projection[16]; /* 透視変換行列の保存用 */
  int i;

  /* モデルビュー変換行列の設定 */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* 視点の位置を設定する（物体の方を奥に移動する）*/
  glTranslated(0.0, 0.0, -10.0);
  
  /* トラックボール式の回転を与える */
  glMultMatrixd(trackballRotation());
  
  /* 光源の位置を設定する */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* テクスチャ変換行列を設定する */
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  
  /* テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める */
  glTranslated(0.5, 0.5, 0.5);
  glScaled(0.5, 0.5, 0.5);
  
  /* 現在の透視変換行列を取り出す */
  glGetDoublev(GL_PROJECTION_MATRIX, projection);

  /* 透視変換行列をテクスチャ変換行列に設定する */
  glMultMatrixd(projection);

  /* モデルビュー変換行列に戻す */
  glMatrixMode(GL_MODELVIEW);
  
  /* デプスバッファを０でクリア */
  glClearDepth(0.0);
  glClear(GL_DEPTH_BUFFER_BIT);

  /* デプスバッファの消去値を１に戻す */
  glClearDepth(1.0);

  /* テクスチャマッピングとテクスチャ座標の自動生成を有効にする */
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glEnable(GL_TEXTURE_GEN_Q);
  
  /* アルファテストを有効にする */
  glEnable(GL_ALPHA_TEST);

  /* Polygon Offset を有効にする */
  glEnable(GL_POLYGON_OFFSET_FILL);

  for (i = 0; i <= layer; ++i) {

    /* デプスバッファをデプステクスチャにコピー */
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXWIDTH, TEXHEIGHT);

    /* フレームバッファとデプスバッファをクリアする */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* シーンを描画する */
    scene();
  }  
    
  /* Polygon Offset を無効にする */
  glDisable(GL_POLYGON_OFFSET_FILL);

  /* アルファテストを無効にする */
  glDisable(GL_ALPHA_TEST);

  /* テクスチャマッピングとテクスチャ座標の自動生成を無効にする */
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);
  glDisable(GL_TEXTURE_2D);

  /* ダブルバッファリング */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* ウィンドウサイズの拡大縮小を制限する */
  if (w != TEXWIDTH || h != TEXHEIGHT) {
    if (w != TEXWIDTH) w = TEXWIDTH;
    if (h != TEXHEIGHT) h = TEXHEIGHT;
    glutReshapeWindow(w, h);
  }

  /* トラックボールする範囲 */
  trackballRegion(w, h);
  
  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);
  
  /* 透視変換行列の指定 */
  glMatrixMode(GL_PROJECTION);
  
  /* 透視変換行列の初期化 */
  glLoadIdentity();
  gluPerspective(40.0, (GLdouble)w / (GLdouble)h, 3.0, 15.0);
}

static void idle(void)
{
  /* 画面の描き替え */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* トラックボール開始 */
      trackballStart(x, y);
      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      /* トラックボール停止 */
      trackballStop(x, y);
      glutIdleFunc(0);
      break;
    default:
      break;
    }
    break;
    default:
      break;
  }
}

static void motion(int x, int y)
{
  /* トラックボール移動 */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  if (key >= '0' && key <= '9') {
    /* '0'～'9' のキーで Depth Peeling のレイヤー変更 */
    layer = key - '0';
    glutPostRedisplay();
  }
  else {
    switch (key) {
    case 'q':
    case 'Q':
    case '\033':
      /* ESC か q か Q をタイプしたら終了 */
      exit(0);
    default:
      break;
    }
  }
}

/*
** メインプログラム
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(TEXWIDTH, TEXHEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}

///////////////
// scene.cpp
///////////////
/*
** タイルの描画
*/
static void tile(double w, double d, int nw, int nd)
{
  /* タイルの色 */
  static const GLfloat color[][4] = {
    { 0.6, 0.6, 0.6, 1.0 },
    { 0.3, 0.3, 0.3, 1.0 }
  };
  
  int i, j;

  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
  for (j = 0; j < nd; ++j) {
    GLdouble dj = d * j, djd = dj + d;

    for (i = 0; i < nw; ++i) {
      GLdouble wi = w * i, wiw = wi + w;

      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color[(i + j) & 1]);
      glVertex3d(wi,  0.0, dj);
      glVertex3d(wi,  0.0, djd);
      glVertex3d(wiw, 0.0, djd);
      glVertex3d(wiw, 0.0, dj);
    }
  }
  glEnd();
}

/*
** 箱の描画
*/
static void box(double x, double y, double z)
{
  /* 頂点データ */
  const GLdouble vertex[][3] = {
    { 0.0, 0.0, 0.0 },
    {   x, 0.0, 0.0 },
    {   x,   y, 0.0 },
    { 0.0,   y, 0.0 },
    { 0.0, 0.0,   z },
    {   x, 0.0,   z },
    {   x,   y,   z },
    { 0.0,   y,   z },
  };
  
  /* 面データ */
  static const int face[][4] = {
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 },
  };
  
  /* 面の法線ベクトル */
  static const GLdouble normal[][3] = {
    {  0.0,  0.0, -1.0 },
    {  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0 },
    { -1.0,  0.0,  0.0 },
    {  0.0, -1.0,  0.0 },
    {  0.0,  1.0,  0.0 },
  };
  
  /* 箱の色 */
  static const GLfloat color[] = { 0.8, 0.8, 0.2, 1.0 };
  
  int i, j;
  
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glBegin(GL_QUADS);
  for (j = 0; j < 6; ++j) {
    glNormal3dv(normal[j]);
    for (i = 4; --i >= 0;) {
      glVertex3dv(vertex[face[j][i]]);
    }
  }
  glEnd();
}

/*
** シーンの描画
*/
void scene(void)
{
  static const GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };

  /* タイルを描く */
  glPushMatrix();
  glTranslated(-3.0, -2.0, -3.0);
  tile(1.0, 1.0, 6, 6);
  glPopMatrix();
  
  /* 箱を描く */
  glPushMatrix();
  glTranslated(-1.0, -1.5, -1.0);
  box(2.0, 1.0, 2.0);
  glPopMatrix();
  
  /* 球を描く */
  glPushMatrix();
  glTranslated(1.0, 1.0, 1.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
  glutSolidTeapot(1.0);
  glPopMatrix();
}


///////////////////
// trackball.cpp
///////////////////
#include <math.h>

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

/* ドラッグ開始位置 */
static int cx, cy;

/* マウスの絶対位置→ウィンドウ内での相対位置の換算係数 */
static double sx, sy;

/* マウスの相対位置→回転角の換算係数 */
#define SCALE (2.0 * M_PI)

/* 回転の初期値 (クォータニオン) */
static double cq[4] = { 1.0, 0.0, 0.0, 0.0 };

/* ドラッグ中の回転 (クォータニオン) */
static double tq[4];

/* 回転の変換行列 */
static double rt[16] = {
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0,
};

/* ドラッグ中か否か */
static int drag = 0;

/*
** r <- p x q
*/
static void qmul(double r[], const double p[], const double q[])
{
  r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
  r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
  r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
  r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

/*
** 回転変換行列 r <- クォータニオン q
*/
static void qrot(double r[], double q[])
{
  double x2 = q[1] * q[1] * 2.0;
  double y2 = q[2] * q[2] * 2.0;
  double z2 = q[3] * q[3] * 2.0;
  double xy = q[1] * q[2] * 2.0;
  double yz = q[2] * q[3] * 2.0;
  double zx = q[3] * q[1] * 2.0;
  double xw = q[1] * q[0] * 2.0;
  double yw = q[2] * q[0] * 2.0;
  double zw = q[3] * q[0] * 2.0;
  
  r[ 0] = 1.0 - y2 - z2;
  r[ 1] = xy + zw;
  r[ 2] = zx - yw;
  r[ 4] = xy - zw;
  r[ 5] = 1.0 - z2 - x2;
  r[ 6] = yz + xw;
  r[ 8] = zx + yw;
  r[ 9] = yz - xw;
  r[10] = 1.0 - x2 - y2;
  r[ 3] = r[ 7] = r[11] = r[12] = r[13] = r[14] = 0.0;
  r[15] = 1.0;
}

/*
** トラックボール処理の初期化
** 　　プログラムの初期化処理のところで実行する
*/
void trackballInit(void)
{
  /* ドラッグ中ではない */
  drag = 0;

  /* 単位クォーターニオン */
  cq[0] = 1.0;
  cq[1] = 0.0;
  cq[2] = 0.0;
  cq[3] = 0.0;
  
  /* 回転行列の初期化 */
  qrot(rt, cq);
}

/*
** トラックボールする領域
** 　　Reshape コールバック (resize) の中で実行する
*/
void trackballRegion(int w, int h)
{
  /* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
  sx = 1.0 / (double)w;
  sy = 1.0 / (double)h;
}

/*
** ドラッグ開始
** 　　マウスボタンを押したときに実行する
*/
void trackballStart(int x, int y)
{
  /* ドラッグ開始 */
  drag = 1;

  /* ドラッグ開始点を記録 */
  cx = x;
  cy = y;
}

/*
** ドラッグ中
** 　　マウスのドラッグ中に実行する
*/
void trackballMotion(int x, int y)
{
  if (drag) {
    double dx, dy, a;
    
    /* マウスポインタの位置のドラッグ開始位置からの変位 */
    dx = (x - cx) * sx;
    dy = (y - cy) * sy;
    
    /* マウスポインタの位置のドラッグ開始位置からの距離 */
    a = sqrt(dx * dx + dy * dy);
    
    if (a != 0.0) {
      double ar = a * SCALE * 0.5;
      double as = sin(ar) / a;
      double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };
      
      /* クォータニオンを掛けて回転を合成 */
      qmul(tq, dq, cq);
      
      /* クォータニオンから回転の変換行列を求める */
      qrot(rt, tq);
    }
  }
}

/*
** 停止
** 　　マウスボタンを離したときに実行する
*/
void trackballStop(int x, int y)
{
  /* ドラッグ終了点における回転を求める */
  trackballMotion(x, y);

  /* 回転の保存 */
  cq[0] = tq[0];
  cq[1] = tq[1];
  cq[2] = tq[2];
  cq[3] = tq[3];

  /* ドラッグ終了 */
  drag = 0;
}

/*
** 回転の変換行列を戻す
** 　　戻り値を glMultMatrixd() などで使用してオブジェクトを回転する
*/
double *trackballRotation(void)
{
  return rt;
}