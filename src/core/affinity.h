#ifndef _SRC_AFFINITY_H
#define _SRC_AFFINITY_H

#include "parseobj.h"

void move(model *m, double x, double y, double z);
void _move(float *array, size_t size, double x, double y, double z);
void rotate(model  *m, double angle, char axis);
void _rotate(float *array, double angle, size_t size, char axis);
void scale(model *m, double xs, double ys, double zs);
void _scale(float *array, size_t size, double xs, double ys, double zs);

#endif /*  _SRC_AFFINITY_H */
