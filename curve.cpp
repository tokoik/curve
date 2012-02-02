#include <cmath>

#include "curve.h"

/*
** Catmull-Rom Spline
*/
static float catmull_rom(float x0, float x1, float x2, float x3, float t)
{
  float v1 = (x2 - x0) * 0.5, v2 = (x3 - x1) * 0.5;

  return (((2.0 * x1 - 2.0 * x2 + v1 + v2) * t
    - 3.0 * x1 + 3.0 * x2 - 2.0 * v1 - v2) * t
    + v1) * t + x1;
}

/*
** Catmull-Rom Spline �ɂ��_��̕��
*/
static void interpolate(float *p, const float *p0, const float *p1, const float *p2, const float *p3, float t)
{
  p[0] = catmull_rom(p0[0], p1[0], p2[0], p3[0], t);
  p[1] = catmull_rom(p0[1], p1[1], p2[1], p3[1], t);
  p[2] = catmull_rom(p0[2], p1[2], p2[2], p3[2], t);
}

/*
** �C�ӂ̐��̓_��� Catmull-Rom Spline �ɂ����
**   p: ��Ԃ���_��̍��W�l
**   t: ��Ԃ���_��̃^�C�����C���i�l�͏����Ɋi�[����Ă���j
**   n: �_�̐�
**   u: ��Ԓl�𓾂�p�����[�^ (t[0]��u��t[n - 1]�j
*/
void curve(float *q, const float (*p)[3], const float *t, int n, float u)
{
  if (--n < 0)
    return;
  else if (n == 0) {
    q[0] = p[0][0];
    q[1] = p[0][1];
    q[2] = p[0][2];
  }
  else {
    int i = 0, j = n;
    
    // u ���܂� t �̋�� [t[i], t[i+1]) ��񕪖@�ŋ��߂�
    while (i < j) {
      int k = (i + j) / 2;
      if (t[k] < u)
        i = k + 1;
      else
        j = k;
    }
    
    if (--i < 0) i = 0;
    if (i < n) {
      int i0 = i - 1;
      if (i0 < 0) i0 = 0;
      int i1 = i + 1;
      int i2 = i1 + 1;
      if (i2 > n) i2 = n;
      
#if 0
      // �^�C�����C�����X�v���C����Ԃ���ꍇ
      interpolate(q, p[i0], p[i], p[i1], p[i2],
        catmull_rom(t[i0], t[i], t[i1], t[i2], (u - t[i]) / (t[i1] - t[i])) - t[i]);
#else
      // �^�C�����C���͐��`�i�܂���j��Ԃ���ꍇ
      interpolate(q, p[i0], p[i], p[i1], p[i2], (u - t[i]) / (t[i1] - t[i]));
#endif
    }
    else {
      q[0] = p[n][0];
      q[1] = p[n][1];
      q[2] = p[n][2];
    }
  }
}
