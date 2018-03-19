#ifndef COLOR_H
#define COLOR_H

typedef struct Color3f {
	float r;
	float g;
	float b;
}Color3f;

/* Fonction qui calcule la somme de deux couleurs */
Color3f addColors(Color3f c1, Color3f c2);

/* L’équivalent pour la soustraction, la multiplication et la division */
Color3f subColors(Color3f c1, Color3f c2);
Color3f multColors(Color3f c1, Color3f c2);
Color3f divColors(Color3f c1, Color3f c2);

/* fonctions de multiplication et division d’une couleur par un scalaire */
Color3f multColor(Color3f c, float a);
Color3f divColor(Color3f c, float a);


#endif