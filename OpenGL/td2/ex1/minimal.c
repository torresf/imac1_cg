#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* Structures */
typedef struct Point {
	float x, y; // Position 2D du point
	unsigned char r, g, b; // Couleur du point
	int last_point;
	struct Point* next; // Point suivant à dessiner
} Point, *PointList;

typedef struct Primitive {
	GLenum primitiveType; //Type de primitive (GL_POINTS, GL_LINES, GL_TRIANGLES)
	PointList points;	//Liste des points de la primitive
	struct Primitive* next;	 //Primitive suivante
} Primitive, *PrimitiveList;

/* Fonctions concernant les points */
Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b);
void addPointToList(Point* point, PointList* list);
void drawPoints(PointList list);
void deletePoints(PointList* list);

/* Fonctions concernant les primitive */
Primitive* allocPrimitive(GLenum primitiveType);
void addPrimitive(Primitive* primitive, PrimitiveList* list);
void drawPrimitives(PrimitiveList list);
void deletePrimitive(PrimitiveList* list);

void resize(); //Redimensionne le viewport et rafraichit le repère 
void displayPalette(); //Affiche les couleurs de la palette
void selectColor(int x, int *r, int *g, int *b); // Modifie la couleur actuelle en fonction du clic


int main(int argc, char** argv) {
	int mode; //0: Mode dessin, 1: Mode palette
	float x, y; // Coordonnées du curseur au clic
	int r, g, b; // Valeurs rgb de la couleur courante
	float point_size; //Largeur du point

	PrimitiveList primitives = NULL;
	mode = 0;
	r = g = b = 255; //Couleur blanche
	x = y = 0;
	point_size = 2.0f;

	addPrimitive(allocPrimitive(GL_LINE_STRIP), &primitives);

	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE | SDL_GL_DOUBLEBUFFER)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	resize();

	glColor3ub(r, g, b); //Initialise la couleur à blanc
	glLineWidth(point_size); 
	glPointSize(point_size); 

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Photoshop assez moyen", NULL);

	/* Boucle d'affichage */
	int loop = 1;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		if (mode == 0) {

			glClear(GL_COLOR_BUFFER_BIT);
			drawPrimitives(primitives); //On dessine les primitives
		} else {
			displayPalette(); //On affiche les couleurs pour la sélection
		}

		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();
		
		/* Boucle traitant les evenements */
		SDL_Event e;
		while(SDL_PollEvent(&e)) {

			/* L'utilisateur ferme la fenêtre */
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}

			/* Traitement d'evenements  */
			switch(e.type) {

				/* Clic souris */
				case SDL_MOUSEBUTTONDOWN:
					switch (mode) {
						case 0:	//Mode dessin : Ajout d'un point
							if (e.button.button == SDL_BUTTON_RIGHT)
							{
								printf("RIGHT CLICK\n");
								primitives->primitiveType = GL_LINE_LOOP;
							} else {

							}
							x = -1 + 2. * e.button.x / WINDOW_WIDTH;
							y = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
							printf("x, y = %f, %f\n", x, y);
							addPointToList(allocPoint(x, y, r, g, b), &primitives->points);
							break;
						case 1: //Mode palette : Sélection de couleur 
							selectColor(e.button.x, &r, &g, &b);
							break;
						default:
							break;
					}
					break;

				/* Touche clavier appuyée */
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						case SDLK_SPACE:
							mode = 1;
							break;
						case SDLK_p:
							addPrimitive(allocPrimitive(GL_POINTS), &primitives);
							break;
						case SDLK_l:
							addPrimitive(allocPrimitive(GL_LINES), &primitives);
							break;
						case SDLK_t:
							addPrimitive(allocPrimitive(GL_TRIANGLES), &primitives);
							break;
						case SDLK_q:
							loop = 0;
							break;
						default:
							break;
					}
					break;

				/* Touche clavier relachée */
				case SDL_KEYUP:
					switch (e.key.keysym.sym) {
						case SDLK_SPACE:
							mode = 0;
							glClear(GL_COLOR_BUFFER_BIT);
						default:
							break;
					}
					break;

				/* Resize */
				case SDL_VIDEORESIZE:
					WINDOW_WIDTH =  e.resize.w;
					WINDOW_HEIGHT =  e.resize.h;
					resize();
					break;

				default:
					break;
			}

		}

		SDL_GL_SwapBuffers();

		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;

		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}

	}

	deletePrimitive(&primitives);

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}


Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
	Point* point;
	point = malloc(sizeof(Point));
	if (point == NULL)
	{
		exit(EXIT_FAILURE);
	}
	point->x = x;
	point->y = y;
	point->r = r;
	point->g = g;
	point->b = b;
	point->next = NULL;
	return point;
}

Primitive* allocPrimitive(GLenum primitiveType) {
	Primitive* primitive;
	primitive = malloc(sizeof(Primitive));
	if (primitive == NULL)
	{
		exit(EXIT_FAILURE);
	}
	primitive->primitiveType = primitiveType;
	primitive->points = NULL;
	primitive->next = NULL;
	return primitive;
}

void addPointToList(Point* point, PointList* list) {
	PointList* tmp_list;
	tmp_list = malloc(sizeof(PointList));
	if (tmp_list) {
		tmp_list = list;
		if (*tmp_list == NULL) {
			*tmp_list = point;
		} else {
			point->next = *tmp_list;
			*tmp_list = point;
		}
		*list = *tmp_list;
	} else {
		exit(EXIT_FAILURE);
	}
}

void drawPoints(PointList list) {
	while (list) {
		glColor3ub(list->r, list->g, list->b);
		glVertex2f(list->x, list->y);
		list = list->next;
	}
}

void deletePoints(PointList* list) {
	while (*list) {
		PointList next = (*list)->next;
		free(*list);
		*list = next;
	}
}

void addPrimitive(Primitive* primitive, PrimitiveList* list) {
	PrimitiveList tmp_list;
	tmp_list = malloc(sizeof(PrimitiveList));
	if (tmp_list) {
		tmp_list = *list;
	}
	if (*list == NULL) {
		*list = primitive;
	} else {
		primitive->next = *list;
		*list = primitive;
	}
}

void drawPrimitives(PrimitiveList list) {
	while (list) {
		glBegin(list->primitiveType);
			drawPoints(list->points);
		glEnd();
		list = list->next;
	}
}

void deletePrimitive(PrimitiveList* list) {
	while (*list) {
		deletePoints(&((*list)->points));
		PrimitiveList next = (*list)->next;
		free(*list);
		*list = next;
	}
}

void resize() {
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE | SDL_GL_DOUBLEBUFFER);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

void displayPalette() {
	int nb_colors, i;
	float color_width;
	nb_colors = 8;
	color_width = 2 * WINDOW_WIDTH / (float)nb_colors * 100 / WINDOW_WIDTH / 100 ;
	glBegin(GL_QUADS);
		for(i = 0; i < nb_colors; ++i) {
			glColor3ub((255 / nb_colors) * i, 0, 255);
			glVertex2f(-1 + i * color_width, -1);
			glVertex2f(-1 + (i + 1) * color_width, -1);
			glVertex2f(-1 + (i + 1) * color_width, 1);
			glVertex2f(-1 + i * color_width, 1);
		}
	glEnd();
}

void selectColor(int x, int *r, int *g, int *b) {
	int nb_colors, num_bande;
	nb_colors = 8;
	num_bande = x / (WINDOW_WIDTH / nb_colors);
	*r = (255 / nb_colors) * num_bande;
	*g = 0;
	*b = 255;
}