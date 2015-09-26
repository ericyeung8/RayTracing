#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "DataStruc.h"

Vertex Lighting(LightCoe para, PointLight pl, Vertex n, Vertex E);
IntersecPoint IntersecPlane(ObjectPara para, Vertex p, Vertex v);
IntersecPoint IntersecCylinder(ObjectPara para, Vertex p, Vertex v);
IntersecPoint IntersecEllipsoid(ObjectPara para, Vertex p, Vertex v);
Vertex Reflection(Vertex v, Vertex n);
Vertex PlaneNormal(ObjectPara para, Vertex p);
Vertex CylinderNormal(ObjectPara para, Vertex p);
Vertex EllipsoidNormal(ObjectPara para, Vertex p);

#endif