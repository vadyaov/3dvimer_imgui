#pragma once

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

void parseobj(const char *filename, model *m);
void initModel(model *m);
void count(FILE *file, size_t * vertexNumber, size_t *indexNumber);
void parse(FILE *file, float *, int *);
int toInt(char *src, size_t *i);
