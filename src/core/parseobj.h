#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  float *vertexArray;
  float *trianglesArray;
  float *linesArray;
  char *name;
  size_t vertexNumber;
  size_t indexNumber;
  size_t allIndex, lineIndex;
  int edges;
} model;

void parseobj(const char *filename, model *m);
void initModel(model *m);
void count(FILE *file, size_t *vertexNumber, size_t *indexNumber);
int parse(FILE*, int**, int**, model*);
int toInt(char *src, size_t *i);
size_t spaceNum(char *line);
void loadObjectName(char *line, char **name);
size_t nameSize(char *line);
