#include "affinity.h"

void move(model *m, double x, double y, double z) {
  for (size_t i = 0; i < m->allIndex * 3; i += 3) {
    m->vertexArray[i] += x;
    m->vertexArray[i + 1] += y;
    m->vertexArray[i + 2] += z;
  }
  for (size_t j = 0; j < m->lineIndex * 3; j += 3) {
    m->linesArray[j] += x;
    m->linesArray[j + 1] += y;
    m->linesArray[j + 2] += z;
  }
}

// angle in degrees!
void rotate(model *m, double angle, char axis) {
  angle *= M_PI / 180.0;
  for (size_t i = 0; i < m->allIndex * 3; i += 3) {
    float x = m->vertexArray[i];
    float y = m->vertexArray[i + 1];
    float z = m->vertexArray[i + 2];
      if (axis == 'x') {
        m->vertexArray[i + 1] = y * cos(angle) - z * sin(angle);
        m->vertexArray[i + 2] = y * sin(angle) + z * cos(angle);
      } else if (axis == 'y') {
        m->vertexArray[i] = x * cos(angle) + z * sin(angle);
        m->vertexArray[i + 2] = -x * sin(angle) + z * cos(angle);
      } else if (axis == 'z') {
        m->vertexArray[i] = x * cos(angle) - y * sin(angle);
        m->vertexArray[i + 1] = x * sin(angle) + y * cos(angle);
      }
  }
  for (size_t j = 0; j < m->lineIndex * 3; j += 3) {
    float x = m->linesArray[j];
    float y = m->linesArray[j + 1];
    float z = m->linesArray[j + 2];
      if (axis == 'x') {
        m->linesArray[j + 1] = y * cos(angle) - z * sin(angle);
        m->linesArray[j + 2] = y * sin(angle) + z * cos(angle);
      } else if (axis == 'y') {
        m->linesArray[j] = x * cos(angle) + z * sin(angle);
        m->linesArray[j + 2] = -x * sin(angle) + z * cos(angle);
      } else if (axis == 'z') {
        m->linesArray[j] = x * cos(angle) - y * sin(angle);
        m->linesArray[j + 1] = x * sin(angle) + y * cos(angle);
      }
  }
}

// if need scale only in one axis --> other coeff = 1
void scale(model *m, double xs, double ys, double zs) {
  for(size_t i = 0; i < m->allIndex * 3; i += 3) {
    m->vertexArray[i] *= xs;
    m->vertexArray[i + 1] *= ys;
    m->vertexArray[i + 2] *= zs;
  }
  for (size_t j = 0; j < m->lineIndex * 3; j += 3) {
    m->linesArray[j] *= xs;
    m->linesArray[j + 1] *= ys;
    m->linesArray[j + 2] *= zs;
  }
}
