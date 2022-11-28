#pragma once

//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <string.h>

typedef struct modelinfo {
  std::vector<glm::vec3> vertexArray;
  char *name;
  size_t vertexNumber;
  size_t faceNumber;
} modelData;

void parseObj(const char*, modelData*);
void initData(modelData *m);
size_t countSize(char *line);
const char* makeFormatString(size_t size);
size_t toInt(char *src, size_t *i);
void loadObjectName(char *line, modelData *m);
size_t nameSize(char *line);
