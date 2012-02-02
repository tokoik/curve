#include "catmull_rom.h"

/*
** Catmull-Rom ï‚ä‘
*/
static double interpolate(double x0, double x1, double x2, double x3, double t)
{
  double v1 = (x2 - x0) * 0.5, v2 = (x3 - x1) * 0.5;

  return (((2.0 * x1 - 2.0 * x2 + v1 + v2) * t
    - 3.0 * x1 + 3.0 * x2 - 2.0 * v1 - v2) * t
    + v1) * t + x1;
}

/*
** Catmull-Rom ã»ê¸
*/
void catmull_rom(double *p, double *p0, double *p1, double *p2, double *p3, double t)
{
  p[0] = interpolate(p0[0], p1[0], p2[0], p3[0], t);
  p[1] = interpolate(p0[1], p1[1], p2[1], p3[1], t);
  p[2] = interpolate(p0[2], p1[2], p2[2], p3[2], t);
}
