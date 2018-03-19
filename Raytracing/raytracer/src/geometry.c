#include <math.h>
#include <stdio.h>
#include "geometry.h"

Point3D pointXYZ(float x, float y, float z) {
	Point3D point3D;
	point3D.x = x;
	point3D.y = y;
	point3D.z = z;
	return point3D;
}

Vector3D vectorXYZ(float x, float y, float z) {
	Vector3D vector3D;
	vector3D.x = x;
	vector3D.y = y;
	vector3D.z = z;
	return vector3D;
}

Vector3D vector(Point3D A, Point3D B) {
	Vector3D vector3D;
	vector3D.x = B.x - A.x;
	vector3D.y = B.x - A.x;
	vector3D.z = B.x - A.x;
	return vector3D;
}

Point3D pointPlusVector(Point3D P, Vector3D V) {
	P.x = P.x + V.x;
	P.y = P.y + V.y;
	P.z = P.z + V.z;
	return P;
}

Vector3D addVectors(Vector3D A, Vector3D B) {
	Vector3D vector3D;
	vector3D.x = A.x + B.x;
	vector3D.y = A.y + B.y;
	vector3D.z = A.z + B.z;
	return vector3D;
}

Vector3D subVectors(Vector3D A, Vector3D B) {
	Vector3D vector3D;
	vector3D.x = A.x - B.x;
	vector3D.y = A.y - B.y;
	vector3D.z = A.z - B.z;
	return vector3D;
}

Vector3D multVector(Vector3D V, float a) {
	V.x = V.x * a;
	V.y = V.y * a;
	V.z = V.z * a;
	return V;
}

Vector3D divVector(Vector3D V, float a) {
	if (a != 0) {
		V.x = V.x / a;
		V.y = V.y / a;
		V.z = V.z / a;
	} else
		printf("Erreur : Division par 0\n");
	return V;
}

float dot(Vector3D A, Vector3D B) {
	return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
}

float norm(Vector3D A) {
	return sqrtf(A.x*A.x + A.y*A.y + A.z*A.z);
}

Vector3D normalize(Vector3D A) {
	float normA = norm(A);
	if (normA != 0) {
		A.x = A.x / normA;
		A.y = A.y / normA;
		A.z = A.z / normA;
	} else
		printf("Erreur : Division par 0\n");
	return A;
}

void printVector3D(Vector3D v) {
	printf("(%.5f, %.5f, %.5f)\n", v.x, v.y, v.z);
}

void printPoint3D(Point3D p) {
	printf("(%.2f, %.2f, %.2f)\n", p.x, p.y, p.z);
}