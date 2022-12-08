#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct {
  float *vertexArray;
  char *name;
  size_t vertexNumber;
  size_t indexNumber;
} model;

size_t parseobj(const char *filename, model *m);
void initModel(model *m);
void count(FILE *file, size_t * vertexNumber, size_t *indexNumber);
int* parse(FILE *file, float *, int *, size_t *, char **);
int toInt(char *src, size_t *i);
size_t spaceNum(char *line);
void loadObjectName(char *line, char **name);
size_t nameSize(char *line);
