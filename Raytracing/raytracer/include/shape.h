#ifndef SHAPE_H
#define SHAPE_H

typedef struct Sphere {
	Point3D center;
	float rayon;
	Color3f color;
}Sphere;

typedef struct Cube {
	Point3D max;
	Point3D min;
	Color3f color;
}Cube;

Sphere createSphere(Point3D center, float rayon, Color3f color);

Cube createCube(Point3D max, Point3D min, Color3f color);

#endif