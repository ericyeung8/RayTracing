#ifndef OPERATOR_H
#define OPERATOR_H

#include "DataStruc.h"

Vertex Normalize(Vertex a);
Vertex CrossProduct(Vertex a, Vertex b);
Vertex Minus(Vertex a, Vertex b);
Vertex Plus(Vertex a, Vertex b);
float DotProduct(Vertex a, Vertex b);
Vertex Scale(float a, Vertex b);

#endif