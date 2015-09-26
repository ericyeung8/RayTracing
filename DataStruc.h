#ifndef _DATASTRUC_H_
#define _DATASTRUC_H_

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define LEVEL_MAX 3
#define GAMMA 0.1

class Vertex
{
public:
	float x, y, z;
	Vertex(float newX, float newY, float newZ)
	{
		x = newX;
		y = newY;
		z = newZ;
	}
	Vertex(void)
	{
		x = y = z = 0;
	}
};

class LightCoe
{
public:
	float ka[3];
	float kd[3];
	float ks[3];
	LightCoe(float newa[3], float newd[3], float news[3])
	{
		for (int i = 0; i <= 2; i++)
		{
			ka[i] = newa[i];
			kd[i] = newd[i];
			ks[i] = news[i];
		}
	}
	LightCoe(void)
	{
		for (int i = 0; i <= 2; i++)
		{
			ka[i] = kd[i] = ks[i] = 0;
		}
	}
};

class PointLight
{
public:
	float A[3];
	float C[3];
	Vertex L;
	PointLight(float newa[3], float newc[3], Vertex l)
	{
		for (int i = 0; i <= 2; i++)
		{
			A[i] = newa[i];
			C[i] = newc[i];
		}
		L = l;
	}
	PointLight(void)
	{
		for (int i = 0; i <= 2; i++)
		{
			A[i] = C[i] = 0;
		}
	}

};

class IntersecPoint
{
public:
	Vertex point;
	float t;
	int sym;
	IntersecPoint(void)
	{
		point.x = point.y = point.z = 0;
		t = 0;
		sym = 0;
	}
};

class ObjectPara
{
public:
	float a, b, c, d, l, m, n;
	int type;
	ObjectPara(float newa, float newb, float newc, float newd, int newt)
	{
		a = newa;
		b = newb;
		c = newc;
		d = newd;
		l = m = n = 0;
		type = newt;
	}
	ObjectPara(float newa, float newb, float newc, float newl, float newm, float newn, int newt)
	{
		a = newa;
		b = newb;
		c = newc;
		l = newl;
		m = newm;
		n = newn;
		d = -1;
		type = newt;
	}
	ObjectPara(float newa, float newb, int newt)
	{
		a = newa;
		b = newb;
		c = 0;
		d = -1;
		l = m = n = 0;
		type = newt;
	}
	ObjectPara(void)
	{
		a = b = c = d = l = m = n = type = 0;
	}
};

struct color {
	float r, g, b;		// Color (R,G,B values)
};

#endif