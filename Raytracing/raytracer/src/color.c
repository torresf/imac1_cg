#include <stdio.h>
#include "color.h"

Color3f addColors(Color3f c1, Color3f c2) {
	Color3f color;
	color.r = c1.r + c2.r;
	color.g = c1.g + c2.g;
	color.b = c1.b + c2.b;
	return color;
}

Color3f subColors(Color3f c1, Color3f c2) {
	Color3f color;
	color.r = c1.r - c2.r;
	color.g = c1.g - c2.g;
	color.b = c1.b - c2.b;
	return color;
}

Color3f multColors(Color3f c1, Color3f c2) {
	Color3f color;
	color.r = c1.r * c2.r;
	color.g = c1.g * c2.g;
	color.b = c1.b * c2.b;
	return color;
}

Color3f divColors(Color3f c1, Color3f c2) {
	if (c2.r != 0 && c2.g != 0 && c2.b != 0)
	{
		c1.r = c1.r / c2.r;
		c1.g = c1.g / c2.g;
		c1.b = c1.b / c2.b;
	} else
		printf("Erreur : Division par 0\n");
	return c1;
}

Color3f multColor(Color3f c, float a) {
	c.r = c.r * a;
	c.g = c.g * a;
	c.b = c.b * a;
	return c;
}

Color3f divColor(Color3f c, float a) {
	if (a != 0) {
		c.r = c.r / a;
		c.g = c.g / a;
		c.b = c.b / a;
	} else
		printf("Erreur : Division par 0\n");
	return c;
}