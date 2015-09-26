#include "stdafx.h"
#include <math.h>
#include "DataStruc.h"

Vertex Normalize(Vertex a)
{
	Vertex rvalue;
	float scale;

	scale = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	rvalue.x = a.x / scale;
	rvalue.y = a.y / scale;
	rvalue.z = a.z / scale;

	return rvalue;
}

Vertex CrossProduct(Vertex a, Vertex b)
{
	Vertex rvalue;

	rvalue.x = a.y*b.z - a.z*b.y;
	rvalue.y = a.z*b.x - a.x*b.z;
	rvalue.z = a.x*b.y - a.y*b.x;

	return rvalue;
}

Vertex Minus(Vertex a, Vertex b)
{
	Vertex rvalue;

	rvalue.x = a.x - b.x;
	rvalue.y = a.y - b.y;
	rvalue.z = a.z - b.z;

	return rvalue;
}

Vertex Plus(Vertex a, Vertex b)
{
	Vertex rvalue;

	rvalue.x = a.x + b.x;
	rvalue.y = a.y + b.y;
	rvalue.z = a.z + b.z;

	return rvalue;
}

float DotProduct(Vertex a, Vertex b)
{
	float rvalue;

	rvalue = a.x*b.x + a.y*b.y + a.z*b.z;

	return rvalue;
}

Vertex Scale(float a, Vertex b)
{
	Vertex rvalue;

	rvalue.x = a*b.x;
	rvalue.y = a*b.y;
	rvalue.z = a*b.z;

	return rvalue;
}