#include "parseobj.h"

void parseobj(const char *filename, model *m) {
  initModel(m);
  FILE *file = fopen(filename, "r");
  if (file) {
    count(file, &m->vertexNumber, &m->indexNumber);
    m->edges = m->vertexNumber + m->indexNumber - 2;

    size_t v_size = 3 * m->vertexNumber;

    m->vertexArray = (float *)calloc(v_size, sizeof(float));
    if (!m->vertexArray) exit(1);

    int *mainIndexArray = NULL;
    int *edgeIndexArray = NULL;

    int e = parse(file, &mainIndexArray, &edgeIndexArray, m);

    /* printf("lines:%zu\ttriangles:%zu\n-----VERTICES-----\n", m->lineIndex,
     * m->allIndex); */

    /* printf("VERTEX\n"); */
    /* for (size_t i = 0; i < m->vertexNumber * 3; i++) */
    /*   printf("%f ", m->vertexArray[i]); */
    /* printf("\n\n"); */

    /* printf("\n-----FACES TRIAN-----\n"); */
    /* for (size_t j = 0; j < m->allIndex; j++) */
    /*   printf("%d ", mainIndexArray[j]); */

    /* printf("\n-----FACES LINES-----\n"); */
    /* for (size_t j = 0; j < m->lineIndex; j++) */
    /*   printf("%d ", edgeIndexArray[j]); */

    if (!e) {
      m->trianglesArray = (float *)calloc(m->allIndex * 3, sizeof(float));
      if (m->trianglesArray)
        for (size_t i = 0, k = 0; k < m->allIndex; i += 3, k++)
          for (size_t j = 0; j < 3; j++)
            m->trianglesArray[i + j] = m->vertexArray[(mainIndexArray[k] - 1) * 3 + j];
    }

    m->linesArray = (float *)calloc(m->lineIndex * 3, sizeof(float));
    if (m->linesArray)
      for (size_t i = 0, k = 0; k < m->lineIndex; i += 3, k++)
        for (size_t j = 0; j < 3; j++)
          m->linesArray[i + j] = m->vertexArray[(edgeIndexArray[k] - 1) * 3 + j];

    /*   printf("\n-----AFTER INDEXING-----\n"); */
    /*   for (size_t i = 0, j = 0; i < m->allIndex * 3; i++) { */
    /*     printf("%f ", m->trianglesArray[i]); */
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

    free(mainIndexArray);
    free(edgeIndexArray);
    fclose(file);
  }
}

void initModel(model *m) {
  m->vertexArray = NULL;
  m->trianglesArray = NULL;
  m->linesArray = NULL;
  m->name = NULL;
  m->vertexNumber = 0;
  m->indexNumber = 0;
  m->allIndex = 0;
  m->lineIndex = 0;
  m->edges = 0;
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

int parse(FILE *file, int **f, int **fl, model *m) {
  int errMark = 0;
  char *line = NULL;
  size_t len = 0;
  size_t i = 0, j = 0, p = 0;

  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line + 2, "%f %f %f", &m->vertexArray[i], &m->vertexArray[i + 1], &m->vertexArray[i + 2]);
      i += 3;
    } else if (line[0] == 'f' && line[1] == ' ') {
      size_t k = 2, startj = j, startp = p;
      size_t spcs = spaceNum(line + k);
      size_t mm = 0;

      if (!errMark && spcs > 1) {
        m->allIndex += (spcs - 1) * 3;
        m->lineIndex += (spcs + 1) * 2;
      } else if (spcs == 1) {
        m->lineIndex += 2;
        errMark = 1;
      }

      if (!errMark) {
        *f = (int *)realloc(*f, m->allIndex * sizeof(int));
        if (NULL == f) {
          if (fl) free(fl);
          exit(1);
        }
      }
      *fl = (int *)realloc(*fl, m->lineIndex * sizeof(int));
      if (NULL == fl) {
        if (f) free(f);
        exit(1);
      }

      while (line[k] != '\0') {
        int cur = toInt(line + k, &k);
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

        while (line[k] != ' ' && line[k] != '\0') k++;
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

size_t spaceNum(const char *line) {
  size_t sp = 0, i = 0;
  while (line[i] != '\0') {
    if (line[i] == ' ' && line[i + 1] != '\0') sp++;
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
