#ifndef _SRC_AFFINITY_H
#define _SRC_AFFINITY_H

#include "../parseobj/parseobj.h"

void move(model *m, double x, double y, double z);
void rotate(model  *m, double angle, char axis);
void scale(model *m, double xs, double ys, double zs);

#endif /*  _SRC_AFFINITY_H */
