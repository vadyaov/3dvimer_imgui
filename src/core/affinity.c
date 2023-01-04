#include "affinity.h"

void move(model *m, float x, float y, float z) {
  _move(m->vertexArray, m->vertexNumber * 3, x, y, z);
  _move(m->linesArray, m->lineIndex * 3, x, y, z);
  _move(m->trianglesArray, m->allIndex * 3, x, y, z);
}

void rotate(model *m, float angle, char axis) {
  _rotate(m->vertexArray, angle, m->vertexNumber * 3, axis);
  _rotate(m->linesArray, angle, m->lineIndex * 3, axis);
  _rotate(m->trianglesArray, angle, m->allIndex * 3, axis);
}

void scale(model *m, float xs, float ys, float zs) {
  _scale(m->vertexArray, m->vertexNumber * 3, xs, ys, zs);
  _scale(m->linesArray, m->lineIndex * 3, xs, ys, zs);
  _scale(m->trianglesArray, m->allIndex * 3, xs, ys, zs);
}

void _move(float *array, size_t size, float x, float y, float z) {
  for (size_t i = 0; i < size; i += 3) {
    array[i] += x;
    array[i + 1] += y;
    array[i + 2] += z;
  }
}

void _rotate(float *array, float angle, size_t size, char axis) {
  for (size_t i = 0; i < size; i += 3) {
    float x = array[i];
    float y = array[i + 1];
    float z = array[i + 2];
    if (axis == 'x') {
      array[i + 1] = y * cos(angle) - z * sin(angle);
      array[i + 2] = y * sin(angle) + z * cos(angle);
    } else if (axis == 'y') {
      array[i] = x * cos(angle) + z * sin(angle);
      array[i + 2] = -x * sin(angle) + z * cos(angle);
    } else if (axis == 'z') {
      array[i] = x * cos(angle) - y * sin(angle);
      array[i + 1] = x * sin(angle) + y * cos(angle);
    }
  }
}

void _scale(float *array, size_t size, float xs, float ys, float zs) {
  for (size_t i = 0; i < size; i += 3) {
    array[i] *= xs;
    array[i + 1] *= ys;
    array[i + 2] *= zs;
  }
}
