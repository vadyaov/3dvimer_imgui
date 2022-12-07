#include "parseobj.h"

void parseobj(const char *filename, model *m) {
  initModel(m);
  FILE *file = fopen(filename, "r");

  count(file, &m->vertexNumber, &m->indexNumber);

  printf("v:%zu\tf:%zu\n", m->vertexNumber, m->indexNumber);

  size_t v_size = 3 * m->vertexNumber;
  size_t f_size = 0;
  
  printf("v_size:%zu\tf_size:%zu\n", v_size, f_size);

  float *vertices = (float *)calloc(v_size, sizeof(float));
  int *indexes = NULL;

  indexes = parse(file, vertices, indexes, &f_size);

  for (size_t i = 0; i < m->vertexNumber * 3; i++)
    printf("%f ", vertices[i]);

  printf("\n");
  for (size_t j = 0; j < f_size; j++)
    printf("%d ", indexes[j]);

  printf("\n");
    
  m->vertexArray = (float *)calloc(f_size * 3, sizeof(float));
  for (size_t i = 0, k = 0; k < f_size; i += 3, k++) {
    for (size_t j = 0; j < 3; j++) {
      m->vertexArray[i + j] = vertices[(indexes[k] - 1) * 3 + j];
    }
  }

  for (size_t i = 0, j = 0; i < f_size * 3; i++) {
    printf("%f ", m->vertexArray[i]);
    j++;
    if (j == 3) {
      printf("\n");
      j = 0;
    }
  }

  free(vertices);
  free(indexes);
  fclose(file);
}

void initModel(model *m) {
  m->vertexArray = NULL;
  m->name = NULL;
  m->vertexNumber = 0;
  m->indexNumber = 0;
}

void count(FILE *file, size_t *vertexNumber, size_t *indexNumber) {
  char *line = NULL;
  size_t len = 0;
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
int* parse(FILE *file, float *v, int *f, size_t *f_size) {
  char *line = NULL;
  size_t len = 0;
  size_t i = 0, j = 0;

  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line + 2, "%f %f %f", v + i, v + i + 1, v + i + 2);
      printf("Hello!\n");
      i += 3;
    } else if (line[0] == 'f' && line[1] == ' ') {
      size_t k = 2;
      size_t spcs = spaceNum(line + k);

      *f_size += spcs == 3 ? spcs * 2 : spcs + 1;
      printf("index_size = %zu\n", *f_size);
      f = (int *)realloc(f, *f_size * sizeof(int));

      while (line[k] != '\0') {
        f[j++] = toInt(line + k, &k);
        while (line[k] != ' ' && line[k] != '\0')
          k++;
        if (line[k] == '\0') k--;
        k++;
      }

      if (spcs == 3) {
        /* f[j] = f[j - 4]; */
        /* j++; */
        /* f[j] = f[j - 3]; */
        /* j++; */
        for(int l = 4; l > 2; l--, j++)
          f[j] = f[j - l];
      }

    }
  }
  if (line) free(line);
  return f;
}

size_t spaceNum(char *line) {
  size_t sp = 0;
  while (*line != '\0') {
    if (*line++ == ' ') sp++;
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
/*   model m; */
/*   parseobj("../models/cube.obj", &m); */
/*   free(m.vertexArray); */
/*   return 0; */
/* } */
