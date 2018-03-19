#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926535

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

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


void drawLandmark() {
	int i;
	// Axe des abscisses rouge
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-8, 0);
		glVertex2f( 8, 0);
	glEnd();
	for (i = -8; i < 8; ++i)
	{
		glBegin(GL_LINES);
			glVertex2f(i, -.05);
			glVertex2f(i, .05);
		glEnd();
	}

	// Axe des ordonnées vert
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -6);
		glVertex2f(0,  6);
	glEnd();
	for (i = -6; i < 6; ++i)
	{
		glBegin(GL_LINES);
			glVertex2f(-.05, i);
			glVertex2f( .05, i);
		glEnd();
	}
}

/* Fonctions dessins objets canoniques */
void drawSquare(int full) {
	GLenum primitiveType = full ? GL_QUADS : GL_LINE_LOOP;

	glBegin(primitiveType);
		glVertex2f(-.5, -.5);
		glVertex2f( .5, -.5);
		glVertex2f( .5,  .5);
		glVertex2f(-.5,  .5);
	glEnd();
}

void drawCircle(int full) {
	int nbPoints = 64;
	int i;
	float teta = 0;
	GLenum primitiveType = full ? GL_POLYGON : GL_LINE_LOOP;

	glBegin(primitiveType);
		for (i = 0; i < nbPoints; ++i)
		{
			teta = i * (2 * PI) / nbPoints;
			glVertex2f(cos(teta)/2, sin(teta)/2);
		}
	glEnd();
}

void drawRoundedSquare() {
	float radius = .5;
	glPushMatrix();
		glPushMatrix();
			glTranslatef(-.5, -.5, 0);
			glScalef(1-radius, 1-radius, 1);
			glTranslatef(.5, .5, 0);
			drawCircle(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-.5, .5, 0);
			glScalef(1-radius, 1-radius, 1);
			glTranslatef(.5, -.5, 0);
			drawCircle(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(.5, -.5, 0);
			glScalef(1-radius, 1-radius, 1);
			glTranslatef(-.5, .5, 0);
			drawCircle(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(.5, .5, 0);
			glScalef(1-radius, 1-radius, 1);
			glTranslatef(-.5, -.5, 0);
			drawCircle(1);
		glPopMatrix();
		glPushMatrix();
			glScalef(radius, 1, 1);
			drawSquare(1);
		glPopMatrix();
		glPushMatrix();
			glScalef(1, radius, 1);
			drawSquare(1);
		glPopMatrix();
	glPopMatrix();
}

GLuint createFirstArmIDList() {
	GLuint id = glGenLists(1);
	glNewList(id, GL_COMPILE);

	glPushMatrix();
		glScalef(4, 4, 1);
		drawCircle(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(6, 0, 0);
		glScalef(2, 2, 1);
		drawCircle(1);
	glPopMatrix();

	glBegin(GL_QUADS);
		glVertex2f(0, 2);
		glVertex2f(0, -2);
		glVertex2f(6, -1);
		glVertex2f(6, 1);
	glEnd();

	glEndList();
	return id;
}

GLuint createSecondArmIDList() {
	GLuint id = glGenLists(1);
	glNewList(id, GL_COMPILE);

	glColor3ub(180, 180, 180);
	glPushMatrix();
		drawRoundedSquare();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(4, 0, 0);
		drawRoundedSquare();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(2, 0, 0);
		glScalef(4.6, .6, 1);
		drawSquare(1);
	glPopMatrix();

	glEndList();
	return id;
}

GLuint createThirdArmIDList() {
	GLuint id = glGenLists(1);
	glNewList(id, GL_COMPILE);

	glColor3ub(255, 180, 100);
	glPushMatrix();
		glScalef(.6, .6, 1);
		drawSquare(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.5, 0, 0);
		glScalef(3, .4, 1);
		drawSquare(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3, 0, 0);
		glScalef(.8, .8, 1);
		drawCircle(1);
	glPopMatrix();

	glEndList();
	return id;
}

void resize(); //Redimensionne le viewport et rafraichit le repère

int main(int argc, char** argv) {
	float x, y; // Coordonnées du curseur au clic
	int r, g, b; // Valeurs rgb de la couleur courante
	float point_size; //Largeur du point

	PrimitiveList primitives = NULL;
	r = g = b = 255; //Couleur blanche
	x = y = 0;
	point_size = 1.0f;

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


	glScalef(.25, .25, 1);
	glColor3ub(r, g, b); //Initialise la couleur à blanc
	glLineWidth(point_size); 
	glPointSize(point_size); 	


	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Photoshop assez moyen", NULL);

	/* Boucle d'affichage */
	int loop = 1;
	int alpha, beta, gamma;
	alpha = 45;
	beta = -10;
	gamma = 35;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		drawLandmark(); // Dessin du repère
		glColor3ub(255, 255, 255);
		glPushMatrix();
			glRotatef(alpha, 0, 0, 1);
			glCallList(createFirstArmIDList());

			glTranslatef(6, 0, 0);
			glRotatef(beta, 0, 0, 1);
			glCallList(createSecondArmIDList());

			glTranslatef(4, 0, 0);
			glRotatef(gamma, 0, 0, 1);
			glCallList(createThirdArmIDList());
			glRotatef(gamma*2, 0, 0, 1);
			glCallList(createThirdArmIDList());
			glRotatef(gamma/2, 0, 0, 1);
			glCallList(createThirdArmIDList());
		glPopMatrix();

		alpha++;
		beta++;
		gamma++;

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

				/* Touche clavier appuyée */
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
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
	gluOrtho2D(-4., 4., -3., 3.);
}