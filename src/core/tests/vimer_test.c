#include "test.h"

START_TEST(parser_t1) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);

  ck_assert_uint_eq(m.allIndex, 24);
  ck_assert_uint_eq(m.lineIndex, 36);
  ck_assert_str_eq(m.name, "Prism");
  for (size_t i = 0; i < m.allIndex * 3; i++)
    ck_assert_float_eq_tol(waitingTriangles[i], m.trianglesArray[i], 1e-6);
  for (size_t i = 0; i < m.lineIndex * 3; i++)
    ck_assert_float_eq_tol(waitingLines[i], m.linesArray[i], 1e-6);

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(parser_t2) {
  model m;
  const char path[] = "../models/pyramid.obj";

  const float waitingTriangles[] = {
      0, 1, 0,   0,   0,   0,   1,   0, 0, 1, 1, 0,   0,   1,
      0, 1, 0,   0,   0.5, 0.5, 1.6, 1, 0, 0, 0, 0,   0,   0,
      1, 0, 0.5, 0.5, 1.6, 0,   0,   0, 1, 1, 0, 0.5, 0.5, 1.6,
      0, 1, 0,   0.5, 0.5, 1.6, 1,   1, 0, 1, 0, 0};

  const float waitingLines[] = {
      0,   1, 0, 0,   0,   0,   0,   0, 0, 1,   0,   0,   1,   0,   0,   0,
      1,   0, 1, 1,   0,   0,   1,   0, 0, 1,   0,   1,   0,   0,   1,   0,
      0,   1, 1, 0,   0.5, 0.5, 1.6, 1, 0, 0,   1,   0,   0,   0,   0,   0,
      0,   0, 0, 0.5, 0.5, 1.6, 0,   1, 0, 0.5, 0.5, 1.6, 0.5, 0.5, 1.6, 0,
      0,   0, 0, 0,   0,   0,   1,   0, 1, 1,   0,   0.5, 0.5, 1.6, 0.5, 0.5,
      1.6, 0, 1, 0,   0,   1,   0,   1, 1, 0,   0.5, 0.5, 1.6, 1,   1,   0,
      1,   1, 0, 1,   0,   0,   1,   0, 0, 0.5, 0.5, 1.6};

  parseobj(path, &m);

  ck_assert_uint_eq(m.allIndex, 18);
  ck_assert_uint_eq(m.lineIndex, 36);
  ck_assert_str_eq(m.name, "Pyramid");
  for (size_t i = 0; i < m.allIndex * 3; i++)
    ck_assert_float_eq_tol(waitingTriangles[i], m.trianglesArray[i], 1e-6);
  for (size_t i = 0; i < m.lineIndex * 3; i++)
    ck_assert_float_eq_tol(waitingLines[i], m.linesArray[i], 1e-6);

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(affinity_move) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);
  move(&m, 0.5f, 1.0f, -0.5f);

  for (size_t i = 0; i < m.allIndex * 3; i += 3) {
    ck_assert_float_eq_tol(waitingTriangles[i], m.trianglesArray[i] - 0.5f,
                           1e-6);
    ck_assert_float_eq_tol(waitingTriangles[i + 1],
                           m.trianglesArray[i + 1] - 1.0f, 1e-6);
    ck_assert_float_eq_tol(waitingTriangles[i + 2],
                           m.trianglesArray[i + 2] + 0.5f, 1e-6);
  }
  for (size_t i = 0; i < m.lineIndex * 3; i += 3) {
    ck_assert_float_eq_tol(waitingLines[i], m.linesArray[i] - 0.5f, 1e-6);
    ck_assert_float_eq_tol(waitingLines[i + 1], m.linesArray[i + 1] - 1.0f,
                           1e-6);
    ck_assert_float_eq_tol(waitingLines[i + 2], m.linesArray[i + 2] + 0.5f,
                           1e-6);
  }

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(affinity_rotateX) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);
  float angle = 30.0f;
  rotate(&m, angle, 'x');

  for (size_t i = 0; i < m.allIndex * 3; i += 3) {
    // float x = waitingTriangles[i];
    float y = waitingTriangles[i + 1];
    float z = waitingTriangles[i + 2];
    float waitValue1 = y * cos(angle) - z * sin(angle);
    float waitValue2 = y * sin(angle) + z * cos(angle);

    ck_assert_float_eq_tol(m.trianglesArray[i + 1], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.trianglesArray[i + 2], waitValue2, 1e-6);
  }

  for (size_t i = 0; i < m.lineIndex * 3; i += 3) {
    float y = waitingLines[i + 1];
    float z = waitingLines[i + 2];
    float waitValue1 = y * cos(angle) - z * sin(angle);
    float waitValue2 = y * sin(angle) + z * cos(angle);

    ck_assert_float_eq_tol(m.linesArray[i + 1], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.linesArray[i + 2], waitValue2, 1e-6);
  }

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(affinity_rotateY) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);
  float angle = 30.0f;
  rotate(&m, angle, 'y');

  for (size_t i = 0; i < m.allIndex * 3; i += 3) {
    float x = waitingTriangles[i];
    // float y = waitingTriangles[i + 1];
    float z = waitingTriangles[i + 2];

    float waitValue1 = x * cos(angle) + z * sin(angle);
    float waitValue2 = -x * sin(angle) + z * cos(angle);

    ck_assert_float_eq_tol(m.trianglesArray[i], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.trianglesArray[i + 2], waitValue2, 1e-6);
  }

  for (size_t i = 0; i < m.lineIndex * 3; i += 3) {
    float x = waitingLines[i];
    // float y = waitingTriangles[i + 1];
    float z = waitingLines[i + 2];

    float waitValue1 = x * cos(angle) + z * sin(angle);
    float waitValue2 = -x * sin(angle) + z * cos(angle);

    ck_assert_float_eq_tol(m.linesArray[i], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.linesArray[i + 2], waitValue2, 1e-6);
  }

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(affinity_rotateZ) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);
  float angle = 30.0f;
  rotate(&m, angle, 'z');

  for (size_t i = 0; i < m.allIndex * 3; i += 3) {
    float x = waitingTriangles[i];
    float y = waitingTriangles[i + 1];
    // float z = waitingTriangles[i + 2];

    float waitValue1 = x * cos(angle) - y * sin(angle);
    float waitValue2 = x * sin(angle) + y * cos(angle);

    ck_assert_float_eq_tol(m.trianglesArray[i], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.trianglesArray[i + 1], waitValue2, 1e-6);
  }

  for (size_t i = 0; i < m.lineIndex * 3; i += 3) {
    float x = waitingLines[i];
    float y = waitingLines[i + 1];
    // float z = waitingLines[i + 2];

    float waitValue1 = x * cos(angle) - y * sin(angle);
    float waitValue2 = x * sin(angle) + y * cos(angle);

    ck_assert_float_eq_tol(m.linesArray[i], waitValue1, 1e-6);
    ck_assert_float_eq_tol(m.linesArray[i + 1], waitValue2, 1e-6);
  }

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(affinity_Scale) {
  model m;
  const char path[] = "../models/prism.obj";

  const float waitingTriangles[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -4.5973, -4.8484,
      -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  -0.0000,
      4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  -4.5973, 4.8484,  -0.0000,
      -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, -0.0000,
      4.8484,  7.9628,  0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  -4.8484, -0.0000, 4.5973,
      4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628};

  const float waitingLines[] = {
      4.5973,  -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  0.0000,  -4.8484,
      7.9628,  -4.5973, -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 4.5973,
      -4.8484, -0.0000, -4.5973, -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,
      0.0000,  -4.8484, 7.9628,  -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973,
      -4.8484, -0.0000, 0.0000,  -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000,
      4.5973,  -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,
      -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,  7.9628,  0.0000,
      -4.8484, 7.9628,  4.5973,  -4.8484, -0.0000, -4.5973, -4.8484, -0.0000,
      -4.5973, -4.8484, -0.0000, -4.5973, 4.8484,  -0.0000, -4.5973, 4.8484,
      -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,  4.8484,  -0.0000, 4.5973,
      -4.8484, -0.0000, 4.5973,  4.8484,  -0.0000, -4.5973, 4.8484,  -0.0000,
      -4.5973, 4.8484,  -0.0000, -0.0000, 4.8484,  7.9628,  -0.0000, 4.8484,
      7.9628,  4.5973,  4.8484,  -0.0000};

  parseobj(path, &m);
  float xs = 0.5f;
  float ys = 1.2f;
  float zs = 1.0f;
  scale(&m, xs, ys, zs);

  for (size_t i = 0; i < m.allIndex * 3; i += 3) {
    ck_assert_float_eq_tol(m.trianglesArray[i], waitingTriangles[i] * xs, 1e-6);
    ck_assert_float_eq_tol(m.trianglesArray[i + 1],
                           waitingTriangles[i + 1] * ys, 1e-6);
    ck_assert_float_eq_tol(m.trianglesArray[i + 2],
                           waitingTriangles[i + 2] * zs, 1e-6);
  }

  for (size_t i = 0; i < m.lineIndex * 3; i += 3) {
    ck_assert_float_eq_tol(m.linesArray[i], waitingLines[i] * xs, 1e-6);
    ck_assert_float_eq_tol(m.linesArray[i + 1], waitingLines[i + 1] * ys, 1e-6);
    ck_assert_float_eq_tol(m.linesArray[i + 2], waitingLines[i + 2] * zs, 1e-6);
  }

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(dragon) {
  model m;
  const char path[] = "../models/dragon.obj";

  parseobj(path, &m);

  ck_assert_str_eq(m.name, "Dragon");

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

START_TEST(lamp) {
  model m;
  const char path[] = "../models/lamp.obj";

  parseobj(path, &m);

  ck_assert_str_eq(m.name, "LAMP");

  free(m.vertexArray);
  free(m.trianglesArray);
  free(m.linesArray);
  if (m.name) free(m.name);
}
END_TEST

Suite *VimerSuite() {
  Suite *vimer = suite_create("[Vimer Unit Test]");
  TCase *tc = tcase_create("vimer");

  tcase_add_test(tc, parser_t1);
  tcase_add_test(tc, parser_t2);

  tcase_add_test(tc, affinity_move);
  tcase_add_test(tc, affinity_rotateX);
  tcase_add_test(tc, affinity_rotateY);
  tcase_add_test(tc, affinity_rotateZ);
  tcase_add_test(tc, affinity_Scale);

  tcase_add_test(tc, dragon);
  tcase_add_test(tc, lamp);

  suite_add_tcase(vimer, tc);

  return vimer;
}
