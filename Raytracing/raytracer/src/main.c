#include <stdlib.h>
#include <stdio.h>
#include "geometry.h"

int main(int argc, char** argv) {
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
	return EXIT_SUCCESS;
}