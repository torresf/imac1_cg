#include <math.h>
#include <stdio.h>

#include "geometry.h"
#include "color.h"
#include "shape.h"
#include "raytracer.h"

Ray createRay(Point3D origin, Vector3D direction) {
	Ray r;
	r.origin = origin;
	r.direction = normalize(direction);
	return r;
}

int intersectsSphere(Ray r, Sphere s, Intersection* i) {
	float a, b, c, delta, t, t1, t2;

	a = 1;
	b = norm(vector(r.origin, r.direction));
	c = (norm(r.origin) * norm(r.origin)) - (s.rayon * s.rayon);

	delta = (b*b)/(4*a*c);
	t = 0;

	if (delta < 0) return 0;

	if (delta > 0){
		// Deux intersections
		t1 = (-b - sqrt(delta)) / (2*a);
		t2 = (-b + sqrt(delta)) / (2*a);
		if (t1 > 0 && t2 > 0) {
			t = (t1 > t2) ? t2 : t1;
		} else if (t1 > 0 && t2 < 0) {
			t = t1;
		} else if (t1 < 0 && t2 > 0) {
			t = t2;
		} else {
			return 0;
		}
	} else if (delta == 0) {
		// Une intersection
		t = -b / (2*a);
	}
	Point3D p;
	p.x = r.origin.x + t*r.direction.x;
	p.y = r.origin.y + t*r.direction.y;
	p.z = r.origin.z + t*r.direction.z;
	i->position = p;
	return 1;
}

int intersectsCube(Ray r, Cube c, Intersection* i) {
	printf("Cube\n");
	return 1;
}