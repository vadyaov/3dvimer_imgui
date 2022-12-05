#include "parseobj.h"

void parseobj(const char *filename, model *m) {
  initModel(m);
  FILE *file = fopen(filename, "r");

  count(file, &m->vertexNumber, &m->indexNumber);

  printf("v:%zu\tf:%zu\n", m->vertexNumber, m->indexNumber);

  size_t v_size = m->vertexNumber * 3;
  size_t f_size = m->indexNumber * 3;
  
  float vertices[v_size];
  int indexes[f_size];

  parse(file, vertices, indexes);

  for (size_t i = 0; i < m->vertexNumber * 3; i++)
    printf("%f ", vertices[i]);

  printf("\n");
  for (size_t j = 0; j < m->indexNumber * 3; j++)
    printf("%d ", indexes[j]);
    
  m->vertexArray = calloc(f_size * 3, sizeof(float));
  for (size_t i = 0, k = 0; k < f_size; i += 3, k++) {
    for (size_t j = 0; j < 3; j++) {
      m->vertexArray[i + j] = vertices[(indexes[k] - 1) * 3 + j];
    }
  }
  /* for (size_t i = 0, j = 0; i < f_size * 3; i++) { */
  /*   printf("%f ", m->vertexArray[i]); */
  /*   j++; */
  /*   if (j == 3) { */
  /*     printf("\n"); */
  /*     j = 0; */
  /*   } */
  /* } */
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
  fseek(file, 0, SEEK_SET);
}

void parse(FILE *file, float *v, int *f) {
  char *line = NULL;
  size_t len = 0;
  size_t i = 0, j = 0;

  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line + 2, "%f %f %f", &v[i], &v[i + 1], &v[i + 2]);
      i += 3;
    } else if (line[0] == 'f' && line[1] == ' ') {
      size_t k = 2;
      while (line[k] != '\0') {
        f[j++] = toInt(line + k, &k);
        while (line[k] != ' ' && line[k] != '\0')
          k++;
        if (line[k] == '\0') k--;
        k++;
      }
    }
  }
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
/*   return 0; */
/* } */
