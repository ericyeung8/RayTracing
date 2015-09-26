// RayTracing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Operator.h"
#include "Functions.h"
#include <vector>
#include <gl/glut.h>
#include <gl/gl.h>

float framebuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
int flag = 0;

void clearFramebuffer()
{
	int i, j;

	for (i = 0; i<WINDOW_HEIGHT; i++) {
		for (j = 0; j<WINDOW_WIDTH; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;
	if (R <= 1.0)
	if (R >= 0.0)
		framebuffer[y][x][0] = R;
	else
		framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
	if (G >= 0.0)
		framebuffer[y][x][1] = G;
	else
		framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
	if (B >= 0.0)
		framebuffer[y][x][2] = B;
	else
		framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}

void drawit(void)
{
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, framebuffer);
}

color RayTrace(Vertex point, Vertex dir, int level, std::vector <ObjectPara> objects, std::vector <LightCoe> lightpara, std::vector <PointLight> light, Vertex E)
{
	color rvalue;
	color next;
	std::vector <IntersecPoint> InPoints;
	int i, j;
	int empty = 1;
	float maxt = -1;
	Vertex normal;
	Vertex newpoint;
	Vertex newdir;
	LightCoe lcoe;

	rvalue.r = rvalue.g = rvalue.b = 0;
	next.r = next.g = next.b = 0;

	if (level > LEVEL_MAX)
		return rvalue;

	/*Calculate the intersection point*/
	for (i = 0; i < objects.size(); i++)
	{
		IntersecPoint temp;


		if (objects[i].type == 1)
			InPoints.push_back(IntersecPlane(objects[i], point, dir));
		else if (objects[i].type == 2)
			InPoints.push_back(IntersecEllipsoid(objects[i], point, dir));
		else if (objects[i].type == 3)
			InPoints.push_back(IntersecCylinder(objects[i], point, dir));
	}

	/*Find the normal of the intersection point*/
	for (i = 0; i < InPoints.size(); i++)
	{
		if (InPoints[i].t > maxt && InPoints[i].sym != 0)
		{
			maxt = InPoints[i].t;
			newpoint = InPoints[i].point;
			lcoe = lightpara[i];

			if (objects[i].type == 1)
				normal = PlaneNormal(objects[i], newpoint);
			else if (objects[i].type == 2)
				normal = EllipsoidNormal(objects[i], newpoint);
			else if (objects[i].type == 3)
				normal = CylinderNormal(objects[i], newpoint);
		}
		if (InPoints[i].sym != 0)
			empty = 0;
	}

	if (empty)
		return rvalue;

	/*Calculate the reflective ray*/
	newdir = Reflection(dir, normal);

	int shadow = 0;
	for (i = 0; i < light.size(); i++)
	{
		Vertex temp;
		Vertex view;
		IntersecPoint temppoint;

		/*Determine whether the intersection point in the shadow*/
		for (j = 0; j < objects.size(); j++)
		{
			if (objects[j].type == 1)
				temppoint = IntersecPlane(objects[j], light[i].L, Normalize(Minus(newpoint, light[i].L)));
			else if (objects[j].type == 2)
				temppoint = IntersecEllipsoid(objects[j], light[i].L, Normalize(Minus(newpoint, light[i].L)));
			else if (objects[j].type == 3)
				temppoint = IntersecCylinder(objects[j], light[i].L, Normalize(Minus(newpoint, light[i].L)));

			if (temppoint.t>0 && temppoint.t < 1)
			{
				shadow = 1;
				break;
			}
		}

		if (shadow)
			break;

		/*Calculate the local shading*/
		light[i].L = Minus(light[i].L, newpoint);
		view = Minus(E, newpoint);

		temp = Lighting(lcoe, light[i], normal, view);

		rvalue.r += temp.x;
		rvalue.g += temp.y;
		rvalue.b += temp.z;
	}

	//next = RayTrace(newpoint, newdir, level + 1, objects, lightpara, light, E);
	rvalue.r += GAMMA*next.r;
	rvalue.g += GAMMA*next.g;
	rvalue.b += GAMMA*next.b;

	return rvalue;
}

void Scene1(void)
{
	std::vector <ObjectPara> objects;
	std::vector <LightCoe> lightpara;
	std::vector <PointLight> light;

	int x, y;
	Vertex E(0, 0, 100);

	float A[3] = { 0.5, 0.5, 0.5 };
	float C[3] = { 1, 1, 1 };

	float ka1[3] = { 0.1, 0, 0 };
	float kd1[3] = { 0.7, 0, 0 };
	float ks1[3] = { 0.5, 0.5, 0.5 };

	float ka2[3] = { 0, 0, 0.1 };
	float kd2[3] = { 0, 0, 0.7 };
	float ks2[3] = { 0.5, 0.5, 0.2 };

	float ka3[3] = { 0, 0.1, 0 };
	float kd3[3] = { 0, 0.7, 0 };
	float ks3[3] = { 0.5, 0.2, 0.5 };

	objects.push_back(ObjectPara(0, 1, 0, 0, 1));
	objects.push_back(ObjectPara(100, 500, -100, 50, 50, 50, 2));
	objects.push_back(ObjectPara(200, 50, -100, 100, 50, 50, 2));

	light.push_back(PointLight(A, C, Vertex(500, 500, 50)));
	light.push_back(PointLight(A, C, Vertex(-200, 500, 50)));

	lightpara.push_back(LightCoe(ka1, kd1, ks1));
	lightpara.push_back(LightCoe(ka2, kd2, ks2));
	lightpara.push_back(LightCoe(ka3, kd3, ks3));

	for (x = 0; x < WINDOW_WIDTH; x++)
	for (y = 0; y < WINDOW_HEIGHT; y++)
	{
		Vertex dir;
		color rgb;

		dir = Minus(Vertex(x, y, 0), E);
		dir = Normalize(dir);

		rgb = RayTrace(E, dir, 0, objects, lightpara, light, E);
		setFramebuffer(x, y, rgb.r, rgb.g, rgb.b);
	}
}

void Scene2(void)
{
	std::vector <ObjectPara> objects;
	std::vector <LightCoe> lightpara;
	std::vector <PointLight> light;

	int x, y;
	Vertex E(0, 0, 100);

	float A[3] = { 0.5, 0.5, 0.5 };
	float C[3] = { 1, 1, 1 };

	float ka1[3] = { 0.1, 0, 0 };
	float kd1[3] = { 0.7, 0, 0 };
	float ks1[3] = { 0.5, 0.5, 0.5 };

	float ka2[3] = { 0, 0, 0.1 };
	float kd2[3] = { 0, 0, 0.7 };
	float ks2[3] = { 0.5, 0.5, 0.2 };

	float ka3[3] = { 0, 0.1, 0 };
	float kd3[3] = { 0, 0.7, 0 };
	float ks3[3] = { 0.5, 0.2, 0.5 };

	objects.push_back(ObjectPara(0, 1, 0, 0, 1));
	objects.push_back(ObjectPara(100, 500, -100, 50, 50, 50, 2));
	objects.push_back(ObjectPara(20, 20, 3));

	light.push_back(PointLight(A, C, Vertex(500, 500, 50)));
	light.push_back(PointLight(A, C, Vertex(-200, 500, 50)));

	lightpara.push_back(LightCoe(ka1, kd1, ks1));
	lightpara.push_back(LightCoe(ka2, kd2, ks2));
	lightpara.push_back(LightCoe(ka3, kd3, ks3));

	for (x = 0; x < WINDOW_WIDTH; x++)
	for (y = 0; y < WINDOW_HEIGHT; y++)
	{
		Vertex dir;
		color rgb;

		dir = Minus(E, Vertex(x, y, 0));
		dir = Normalize(dir);

		rgb = RayTrace(E, dir, 0, objects, lightpara, light, E);
		setFramebuffer(x, y, rgb.r, rgb.g, rgb.b);
	}
}

void Scene3(void)
{
	std::vector <ObjectPara> objects;
	std::vector <LightCoe> lightpara;
	std::vector <PointLight> light;

	int x, y;
	Vertex E(0, 0, 100);

	float A[3] = { 0.5, 0.5, 0.5 };
	float C[3] = { 1, 1, 1 };

	float ka1[3] = { 0.1, 0, 0 };
	float kd1[3] = { 0.7, 0, 0 };
	float ks1[3] = { 0.5, 0.5, 0.5 };

	float ka2[3] = { 0, 0, 0.1 };
	float kd2[3] = { 0, 0, 0.7 };
	float ks2[3] = { 0.5, 0.5, 0.2 };

	float ka3[3] = { 0, 0.1, 0 };
	float kd3[3] = { 0, 0.7, 0 };
	float ks3[3] = { 0.5, 0.2, 0.5 };

	objects.push_back(ObjectPara(0, 1, 0, 0, 1));
	objects.push_back(ObjectPara(100, 500, -100, 50, 50, 50, 2));
	objects.push_back(ObjectPara(200, 50, -100, 100, 50, 50, 2));

	light.push_back(PointLight(A, C, Vertex(500, 500, 50)));
	light.push_back(PointLight(A, C, Vertex(-200, 500, 50)));

	lightpara.push_back(LightCoe(ka1, kd1, ks1));
	lightpara.push_back(LightCoe(ka2, kd2, ks2));
	lightpara.push_back(LightCoe(ka3, kd3, ks3));

	for (x = 0; x < WINDOW_WIDTH; x++)
	for (y = 0; y < WINDOW_HEIGHT; y++)
	{
		Vertex dir;
		color rgb;

		dir = Minus(E, Vertex(x, y, 0));
		dir = Normalize(dir);

		rgb = RayTrace(E, dir, 0, objects, lightpara, light, E);
		setFramebuffer(x, y, rgb.r, rgb.g, rgb.b);
	}
}

void Scene4(void)
{
	std::vector <ObjectPara> objects;
	std::vector <LightCoe> lightpara;
	std::vector <PointLight> light;

	int x, y;
	Vertex E(0, 0, 100);

	float A[3] = { 0.5, 0.5, 0.5 };
	float C[3] = { 1, 1, 1 };

	float ka1[3] = { 0.1, 0, 0 };
	float kd1[3] = { 0.7, 0, 0 };
	float ks1[3] = { 0.5, 0.5, 0.5 };

	float ka2[3] = { 0, 0, 0.1 };
	float kd2[3] = { 0, 0, 0.7 };
	float ks2[3] = { 0.5, 0.5, 0.2 };

	float ka3[3] = { 0, 0.1, 0 };
	float kd3[3] = { 0, 0.7, 0 };
	float ks3[3] = { 0.5, 0.2, 0.5 };

	objects.push_back(ObjectPara(0, 1, 0, 0, 1));
	objects.push_back(ObjectPara(100, 500, -100, 50, 50, 50, 2));
	objects.push_back(ObjectPara(200, 50, -100, 100, 50, 50, 2));

	light.push_back(PointLight(A, C, Vertex(500, 500, 50)));
	light.push_back(PointLight(A, C, Vertex(-200, 500, 50)));

	lightpara.push_back(LightCoe(ka1, kd1, ks1));
	lightpara.push_back(LightCoe(ka2, kd2, ks2));
	lightpara.push_back(LightCoe(ka3, kd3, ks3));

	for (x = 0; x < WINDOW_WIDTH; x++)
	for (y = 0; y < WINDOW_HEIGHT; y++)
	{
		Vertex dir;
		color rgb;

		dir = Minus(E, Vertex(x, y, 0));
		dir = Normalize(dir);

		rgb = RayTrace(E, dir, 0, objects, lightpara, light, E);
		setFramebuffer(x, y, rgb.r, rgb.g, rgb.b);
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/*clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	/*clear the buffer*/
	clearFramebuffer();
}

void display(void)
{
	switch (flag)
	{
	case 1:
		Scene1();
		break;
	case 2:
		Scene2();
	case 3:
		Scene3();
	case 4:
		Scene4();
	}


	drawit();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		flag = 1;
		break;
	case '2':
		flag = 2;
		break;
	case '3':
		flag = 3;
		break;
	case '4':
		flag = 4;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Jiajun Yang - Assignment 5");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}