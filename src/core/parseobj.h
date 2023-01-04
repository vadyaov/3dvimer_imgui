#ifndef CORE_PARSEOBJ_H
#define CORE_PARSEOBJ_H

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

void parseobj(const char *, model *);
void initModel(model *);
void countVF(FILE *, size_t *, size_t *);
int parse(FILE *, int **, int **, model *);
int toInt(char *, size_t *);
size_t spaceNum(const char *);
void loadObjectName(char *, char **);
size_t nameSize(char *);

#endif /* CORE_PARSEOBJ_H */
