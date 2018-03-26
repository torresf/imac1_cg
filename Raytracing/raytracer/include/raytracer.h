#ifndef RAYTRACER_H
#define RAYTRACER_H

typedef struct Ray {
	Point3D origin;
	Vector3D direction;
}Ray;

typedef struct Intersection {
	Point3D position;
	Color3f color;
}Intersection;

Ray createRay(Point3D origin, Vector3D direction);

int intersectsSphere(Ray r, Sphere s, Intersection* i);

int intersectsCube(Ray r, Cube c, Intersection* i);

#endif