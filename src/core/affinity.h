#ifndef _SRC_AFFINITY_H
#define _SRC_AFFINITY_H

#include "parseobj.h"

void move(model *m, float x, float y, float z);
void _move(float *array, size_t size, float x, float y, float z);
void rotate(model  *m, float angle, char axis);
void _rotate(float *array, float angle, size_t size, char axis);
void scale(model *m, float xs, float ys, float zs);
void _scale(float *array, size_t size, float xs, float ys, float zs);

#endif /*  _SRC_AFFINITY_H */
