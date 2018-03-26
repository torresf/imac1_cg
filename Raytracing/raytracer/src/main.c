#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"
#include "color.h"
#include "shape.h"
#include "raytracer.h"

int main(int argc, char** argv) {
	/*
	Point3D P = pointXYZ(0, 0, 0);
	Vector3D V = vectorXYZ(1, 2, 0);
	printPoint3D(pointPlusVector(P, V));
	printVector3D(addVectors(vectorXYZ(.5, 1, -2), vectorXYZ(.2, -1, 0)));
	printVector3D(subVectors(vectorXYZ(.5, 1, -2), vectorXYZ(.2, -1, 0)));
	printVector3D(multVector(vectorXYZ(.5, 1, -2), 2));
	printVector3D(multVector(vectorXYZ(.5, 1, -2), 0));
	printVector3D(divVector(vectorXYZ(.5, 1, -2), 2));
	printVector3D(divVector(vectorXYZ(.5, 1, -2), 0));
	printf("Dot : %.1f\n", dot(vectorXYZ(1,0,0), vectorXYZ(2,0,0)));
	printf("Dot : %.1f\n", dot(vectorXYZ(1,0,0), vectorXYZ(0,1,0)));
	printf("Norm : %.1f\n", norm(vectorXYZ(2,0,0)));
	printf("Norm : %.1f\n", norm(vectorXYZ(0,0,0)));
	printVector3D(normalize(vectorXYZ(1,1,1)));
	printVector3D(normalize(vectorXYZ(0,0,0)));
	*/

	Color3f color;
	color.r = 255;
	color.g = 255;
	color.b = 255;

	Ray r = createRay(pointXYZ(0, 0, 0), vectorXYZ(1, 1, 1));
	Sphere s = createSphere(pointXYZ(2, 2, 2), 3, color);
	Intersection *i;

	printf("Intersect sphere : %d\n", intersectsSphere(r, s, &i));
	return EXIT_SUCCESS;
}