#include "parseobj.hpp"

void parseObj(const char *filename, modelData *m) {
  initData(m);
  FILE *f = fopen(filename, "r");

  std::vector<glm::vec3> noSortVert;
  std::vector<size_t> faces;

  char *line = NULL;
  size_t len = 0;
  ssize_t read = 0;

  while ((read = getline(&line, &len, f)) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      glm::vec3 v;
      sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z);
      noSortVert.push_back(v);
      m->vertexNumber++;

    } else if (line[0] == 'f' && line[1] == ' ') {
      size_t i = 2;
      size_t size = countSize(line + i);
      size_t face = 0;
      while (line[i] != '\0') {
        face = toInt(line + i, &i);
        faces.push_back(face);
        i++;
      }
      m->faceNumber++;
    } else if (line[0] == 'o' && line[1] == ' ') {
      loadObjectName(line, m);
    }
  }

  for (size_t i = 0; i < faces.size(); i++) {
    glm::vec3 v = noSortVert[faces[i] - 1];
    m->vertexArray.push_back(v);
  }

  if (line) free(line);
  fclose(f);
}

void initData(modelData *m) {
  m->name = NULL;
  m->vertexNumber = 0;
  m->faceNumber = 0;
}

size_t countSize(char *line) {
  size_t space = 0;
  while (*line != '\0') {
    if (*line++ == ' ') space++;
  }
  return space + 1;
}

size_t toInt(char *src, size_t *i) {
  char *start = src, *end = NULL;
  char *buf = start;
  size_t res = 0;
  int j = 0;
  while (*buf != '\0' && isdigit(*buf)) buf++;
  end = buf--;
  while (buf >= start) {
    res += (*buf-- - '0') * (size_t)pow(10, j++);
  }
  *i += end - start;
  return res;
}

/*
const char* makeFormatString(size_t size) {
  size_t s = size * 4;
  char *format = (char *)calloc(s, sizeof(char));
  for (size_t i = 0; i < s; i += 4) {
    format[i] = '%';
    format[i + 1] = 'z';
    format[i + 2] = 'u';
    format[i + 3] = (i < s - 4) ? ' ' : '\0';
  }
  return format;
}
*/

void loadObjectName(char *line, modelData *m) {
  if (NULL == m->name) {
    size_t length = nameSize(line + 2);
    m->name = (char *)calloc(length + 1, sizeof(char));
    if (m->name) {
      m->name = strncpy(m->name, line + 2, length);
      m->name[length] = '\0';
    }
  }
}

size_t nameSize(char *line) {
  size_t l = 0;
  while (isalpha(*line++)) l++;
  return l;
}
