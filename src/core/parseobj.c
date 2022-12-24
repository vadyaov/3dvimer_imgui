#include "parseobj.h"

void parseobj(const char *filename, model *m) {
  initModel(m);
  /* printf("path=%s\nstrlen=%zu\n", filename, strlen(filename)); */
  FILE *file = fopen(filename, "r");

  if (file) {
    count(file, &m->vertexNumber, &m->indexNumber);

    /* printf("v:%zu\tf:%zu\n", m->vertexNumber, m->indexNumber); */

    size_t v_size = 3 * m->vertexNumber;
    
    float *vertices = (float *)calloc(v_size, sizeof(float));
    /* CHECK MEMORY */

    int *mainIndexArray = NULL;
    int *edgeIndexArray = NULL;

    int e = parse(file, vertices, &mainIndexArray, &edgeIndexArray, m);

    /* printf("lines:%zu\ttriangles:%zu\n-----VERTICES-----\n", m->lineIndex, m->allIndex); */

    /* for (size_t i = 0; i < m->vertexNumber * 3; i++) */
    /*   printf("%f ", vertices[i]); */

    /* printf("\n-----FACES TRIAN-----\n"); */
    /* for (size_t j = 0; j < m->allIndex; j++) */
    /*   printf("%d ", mainIndexArray[j]); */

    /* printf("\n-----FACES LINES-----\n"); */
    /* for (size_t j = 0; j < m->lineIndex; j++) */
    /*   printf("%d ", edgeIndexArray[j]); */

    if (!e) {
      m->vertexArray = (float *)calloc(m->allIndex * 3, sizeof(float));
      if (m->vertexArray)
      for (size_t i = 0, k = 0; k < m->allIndex; i += 3, k++)
        for (size_t j = 0; j < 3; j++)
          m->vertexArray[i + j] = vertices[(mainIndexArray[k] - 1) * 3 + j];
    }

    /* printf("m->lineIndex = %zu\n", m->lineIndex); */
    m->linesArray = (float *)calloc(m->lineIndex * 3, sizeof(float));
    if (m->linesArray)
    for (size_t i = 0, k = 0; k < m->lineIndex; i += 3, k++)
      for (size_t j = 0; j < 3; j++)
        m->linesArray[i + j] = vertices[(edgeIndexArray[k] - 1) * 3 + j];

  /*   printf("\n-----AFTER INDEXING-----\n"); */
  /*   for (size_t i = 0, j = 0; i < m->allIndex * 3; i++) { */
  /*     printf("%f ", m->vertexArray[i]); */
  /*     j++; */
  /*     if (j == 3) { */
  /*       printf("\n"); */
  /*       j = 0; */
  /*     } */
  /*   } */

  /*   printf("\n-----AFTER INDEXING-----\n"); */
  /*   for (size_t i = 0, j = 0; i < m->lineIndex * 3; i++) { */
  /*     printf("%f ", m->linesArray[i]); */
  /*     j++; */
  /*     if (j == 3) { */
  /*       printf("\n"); */
  /*       j = 0; */
  /*     } */
  /*   } */

    free(vertices);
    free(mainIndexArray);
    free(edgeIndexArray);
    fclose(file);
  } else {
    printf("AAAAA");
  }
}

void initModel(model *m) {
  m->vertexArray = NULL;
  m->linesArray = NULL;
  m->name = NULL;
  m->vertexNumber = 0;
  m->indexNumber = 0;
  m->allIndex = 0;
  m->lineIndex = 0;
}

void count(FILE *file, size_t *vertexNumber, size_t *indexNumber) {
  char *line = NULL;
  size_t len = 0;
  getline(&line, &len, file);
  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ')
      *vertexNumber += 1;
    else if (line[0] == 'f' && line[1] == ' ')
      *indexNumber += 1;
  }
  if (line) free(line);
  fseek(file, 0, SEEK_SET);
}

 /* ONLY 3 or 4 idx in one face! */
int parse(FILE *file, float *v, int **f, int **fl, model *m) {
  int errMark = 0;
  char *line = NULL;
  size_t len = 0;
  size_t i = 0, j = 0, p = 0;

  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line + 2, "%f %f %f", v + i, v + i + 1, v + i + 2);
      i += 3;
    } else if (line[0] == 'f' && line[1] == ' ') {
      size_t k = 2, startj = j, startp = p;
      size_t spcs = spaceNum(line + k);
      size_t mm = 0;

      /* printf("spaces = %zu\nstart = %zu\n", spcs, start); */

      if (!errMark && spcs > 1) {
        m->allIndex += (spcs - 1) * 3;
        m->lineIndex += (spcs + 1) * 2;
      } else {
        m->lineIndex += 2;
        errMark = 1;
      }
      /* printf("all:%zu\tline%zu\n", m->allIndex, m->lineIndex); */
      if (!errMark)
        *f = (int *)realloc(*f, m->allIndex * sizeof(int));
      *fl = (int *)realloc(*fl, m->lineIndex * sizeof(int));

      // DONT FORGET TO CHECK MEMORY!!! //

      while (line[k] != '\0') {
        int cur = toInt(line + k, &k);
        /* printf("J = %zu\t j - start = %zu\n", j, j - start); */
        /* printf("Line:%s\n", line); */

        /* printf("P = %zu\t p - start = %zu\n", p, p - startp); */
        /* printf("Line:%s\n", line); */

        if (!errMark) {
          if ((j - startj) == 3) {
            (*f)[j] = (*f)[j - 1];
            j++;
          }
          if ((j - startj) % 3 == 0 && (j - startj) > 5) {
            (*f)[j] = (*f)[j - 2];
            j++;
          }
          (*f)[j++] = cur;
          if ((j - startj) == 5 + 3 * mm) {
            mm++;
            (*f)[j] = (*f)[startj];
            j++;
          }
        }

        (*fl)[p++] = cur;
        if (!errMark) {
          if ((p - startp) % 2 == 0 && p > 0) {
            (*fl)[p] = (*fl)[p - 1];
            p++;
          }
          if ((p - startp) == (spcs + 1) * 2 - 1) {
            (*fl)[p] = (*fl)[startp];
            p++;
          }
        }

        while (line[k] != ' ' && line[k] != '\0')
          k++;
        if (line[k] == '\0') k--;
        k++;
      }
    } else if (line[0] == 'o' && line[1] == ' ') {
      loadObjectName(line, &m->name);
    }
  }
  if (line) free(line);
  return errMark;
}

void loadObjectName(char *line, char **name) {
  if (NULL == *name) {
    size_t length = nameSize(line + 2);
    *name = (char *)calloc(length + 1, sizeof(char));
    if (*name) {
      *name = strncpy(*name, line + 2, length);
      (*name)[length] = '\0';
    }
  }
}

size_t nameSize(char *line) {
  size_t l = 0;
  while (isalpha(*line++)) l++;
  return l;
}

size_t spaceNum(char *line) {
  size_t sp = 0, i = 0;
  while (line[i] != '\0') {
    if (line[i] == ' ' && line[i + 1] != '\0')
      sp++;
    i++;
  }
  return sp;
}

int toInt(char *src, size_t *i) {
  char *start = src, *end = NULL;
  char *buf = start;
  int res = 0;
  int j = 0;
  while (*buf != '\0' && isdigit(*buf)) buf++;
  end = buf--;
  while (buf >= start) {
    res += (*buf-- - '0') * (size_t)pow(10, j++);
  }
  *i += end - start;
  return res;
}

/* int main() { */
/*    model m; */
/*    parseobj("../models/dragon.obj", &m); */
/*    free(m.vertexArray); */
/*    free(m.linesArray); */
/*    free(m.name); */
/*    return 0; */
/*  } */