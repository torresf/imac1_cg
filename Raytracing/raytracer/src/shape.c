#include "geometry.h"
#include "color.h"
#include "shape.h"

Sphere createSphere(Point3D center, float rayon, Color3f color) {
	Sphere s;
	s.center = center;
	s.rayon = rayon;
	s.color = color;
	return s;
}

Cube createCube(Point3D max, Point3D min, Color3f color) {
	Cube c;
	c.max = max;
	c.min = min;
	c.color = color;
	return c;
}