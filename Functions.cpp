#include "stdafx.h"
#include <math.h>
#include "Operator.h"

Vertex Lighting(LightCoe para, PointLight pl, Vertex n, Vertex E)
{
	//float ka[3] = { 0.1, 0, 0 };
	//float kd[3] = { 0.7, 0, 0 };
	//float ks[3] = { 0.5, 0.5, 0.5 };
	//float A[3] = { 0.5, 0.5, 0.5 };
	//float C[3] = { 1, 1, 1 };
	//Vertex L(1, 1, -1);
	//Vertex E(0, 0, -1);
	Vertex R;
	int exp = 5;
	float temp;
	float Diffuse;
	float Specular;
	Vertex rvalue;

	pl.L = Normalize(pl.L);
	n = Normalize(n);
	E = Normalize(E);

	//Calculate the vector R
	temp = 2.0 * DotProduct(pl.L, n);
	R = Minus(Scale(temp, n), pl.L);

	Diffuse = DotProduct(pl.L, n);
	Specular = pow((double)DotProduct(R, E), (double)exp);

	if (Diffuse < 0)
	{
		Diffuse = 0;
		Specular = 0;
	}

	if (Specular < 0)
		Specular = 0;

	//Calculate the Lighting value
	rvalue.x = para.ka[0] * pl.A[0] + pl.C[0] * (para.kd[0] * Diffuse + para.ks[0] * Specular);
	rvalue.y = para.ka[1] * pl.A[1] + pl.C[1] * (para.kd[1] * Diffuse + para.ks[1] * Specular);
	rvalue.z = para.ka[2] * pl.A[2] + pl.C[2] * (para.kd[2] * Diffuse + para.ks[2] * Specular);

	return rvalue;
}

/*Calculate the intersection point of plane*/
IntersecPoint IntersecPlane(ObjectPara para, Vertex p, Vertex v)
{
	IntersecPoint rvalue;
	float temp;

	temp = para.a*v.x + para.b*v.y + para.c*v.z;
	
	if (temp == 0)
		rvalue.sym = 0;
	else
	{
		rvalue.t = -1.0*(para.a*p.x + para.b*p.y + para.c*p.z) / temp;
		rvalue.point = Plus(p, Scale(rvalue.t, v));
		rvalue.sym = 1;
	}

	return rvalue;
}

/*Calculate the intersection point of cylinder*/
IntersecPoint IntersecCylinder(ObjectPara para, Vertex p, Vertex v)
{
	IntersecPoint rvalue;
	float a, b, c;
	float temp;

	a = pow(v.x, 2) / pow(para.a, 2) + pow(v.y, 2) / pow(para.b, 2);
	b = 2.0*p.x*v.x / pow(para.a, 2) + 2.0*p.y*v.y / pow(para.b, 2);
	c = pow(p.x, 2) / pow(para.a, 2) + pow(p.y, 2) / pow(para.b, 2) - 1;

	temp = pow(b, 2) - 4 * a*c;

	if (temp < 0)
		rvalue.sym = 0;
	else if (temp == 0)
	{
		rvalue.t = -1.0*b / (2.0*a);
		rvalue.point = rvalue.point = Plus(p, Scale(rvalue.t, v));
		rvalue.sym = 1;
	}
	else if (temp >0)
	{
		float t1, t2;

		t1 = (-1.0*b + sqrt(temp)) / (2.0*a);
		t2 = (-1.0*b - sqrt(temp)) / (2.0*a);

		if (abs(t1) < abs(t2))
			rvalue.t = t1;
		else rvalue.t = t2;

		rvalue.point = rvalue.point = Plus(p, Scale(rvalue.t, v));
		rvalue.sym = 1;
	}

	return rvalue;
}

/*Calculate the intersection point of sphere and ellipsoid*/
IntersecPoint IntersecEllipsoid(ObjectPara para, Vertex p, Vertex v)
{
	IntersecPoint rvalue;
	float a, b, c;
	float temp;

	a = pow(v.x, 2) / pow(para.l, 2) + pow(v.y, 2) / pow(para.m, 2) + pow(v.z, 2) / pow(para.n, 2);
	b = 2.0*((p.x - para.a)*v.x / pow(para.l, 2) + (p.y - para.b)*v.y / pow(para.m, 2) + (p.z - para.c)*v.z / pow(para.n, 2));
	c = pow(p.x - para.a, 2) / pow(para.l, 2) + pow(p.y - para.b, 2) / pow(para.m, 2) + pow(p.z - para.c, 2) / pow(para.n, 2) - 1;

	temp = pow(b, 2) - 4 * a*c;

	if (temp < 0)
		rvalue.sym = 0;
	else if (temp == 0)
	{
		rvalue.t = -1.0*b / (2.0*a);
		rvalue.point = rvalue.point = Plus(p, Scale(rvalue.t, v));
		rvalue.sym = 1;
	}
	else if (temp >0)
	{
		float t1, t2;

		t1 = (-1.0*b + sqrt(temp)) / (2.0*a);
		t2 = (-1.0*b - sqrt(temp)) / (2.0*a);

		if (abs(t1) < abs(t2))
			rvalue.t = t1;
		else rvalue.t = t2;

		rvalue.point = rvalue.point = Plus(p, Scale(rvalue.t, v));
		rvalue.sym = 1;
	}

	return rvalue;
}

/*Calculate the reflect ray*/
Vertex Reflection(Vertex v, Vertex n)
{
	float temp;
	Vertex R;

	v = Normalize(v);
	n = Normalize(n);

	temp = 2.0 * DotProduct(v, n);
	R = Minus(Scale(temp, n), v);

	return R;
}

/*Calcualte the plane normal*/
Vertex PlaneNormal(ObjectPara para, Vertex p)
{
	Vertex rvalue;

	rvalue.x = para.a;
	rvalue.y = para.b;
	rvalue.z = para.c;

	rvalue = Normalize(rvalue);

	return rvalue;
}

/*Calculate the cylinder normal*/
Vertex CylinderNormal(ObjectPara para, Vertex p)
{
	Vertex rvalue;

	rvalue.x = 2.0*p.x / pow(para.a, 2);
	rvalue.y = 2.0*p.y / pow(para.b, 2);
	rvalue.z = 0;

	rvalue = Normalize(rvalue);

	return rvalue;
}

/*Calculate the sphere or ellipsoid normal*/
Vertex EllipsoidNormal(ObjectPara para, Vertex p)
{
	Vertex rvalue;

	rvalue.x = 2.0*(p.x - para.a) / pow(para.l, 2);
	rvalue.y = 2.0*(p.y - para.b) / pow(para.m, 2);
	rvalue.z = 2.0*(p.z - para.c) / pow(para.n, 2);

	rvalue = Normalize(rvalue);

	return rvalue;
}