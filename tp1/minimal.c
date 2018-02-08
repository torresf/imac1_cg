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

typedef struct Point{
	float x, y; // Position 2D du point
	unsigned char r, g, b; // Couleur du point
	struct Point* next; // Point suivant à dessiner
} Point, *PointList;

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
	printf("In allocPoint\n");
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

void addPointToList(Point* point, PointList* list) {
	PointList tmp_list;
	tmp_list = malloc(sizeof(PointList));
	if (tmp_list) {
		tmp_list = *list;
	}
	printf("In addPointToList\n");
	printf("X du point : %f\n", point->x);
	if (*list == NULL) {
		*list = point;
	} else {
		while (tmp_list->next != NULL) {
			tmp_list = tmp_list->next;
		}
		tmp_list->next = point;
	}
}

void drawPoints(PointList list) {
	while (list != NULL) {
		glColor3ub(list->r, list->g, list->b);
		glBegin(GL_POINTS);
			glVertex2f(list->x, list->y);
		glEnd();
		list = list->next;
	}
}

void deletePoints(PointList* list) { //Pas sûr
	if (*list) {
		while (list != NULL) {
			PointList next = (*list)->next;
			free(list);
			*list = next;
		}
	}
}

void resize() {
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

void displayPalette(){
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

void selectColor(int x, int *r, int *g, int *b){
	int nb_colors, num_bande;
	nb_colors = 8;

	num_bande = x / (WINDOW_WIDTH / nb_colors);
	printf("Clic Sur %d\n", num_bande);
	*r = (255 / nb_colors) * num_bande;
	*g = 0;
	*b = 255;
}

int main(int argc, char** argv) {
	int primitive, mode;
	float x1 = 0,
		y1 = 0, 
		x2 = 0, 
		y2 = 0, 
		x3 = 0, 
		y3 = 0;
	int r, g, b;
	PointList list;

	list = NULL;
	primitive = 0;
	mode = 0;
	r = 255;
	g = 255;
	b = 255;


	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

	/* Ouverture d'une fenÃªtre et crÃ©ation d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	resize();

	glColor3ub(255, 255, 255);

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Photoshop assez moyen", NULL);

	/* Boucle d'affichage */
	int loop = 1;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		// SDL_GL_SwapBuffers();

		if (mode == 1) {
			displayPalette();
		}

		
		/* Boucle traitant les evenements */
		SDL_Event e;
		while(SDL_PollEvent(&e)) {

			/* L'utilisateur ferme la fenêtre : */
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}

			/* Quelques exemples de traitement d'evenements : */
			switch(e.type) {

				/* Clic souris */
				case SDL_MOUSEBUTTONDOWN:
					printf("clic en (%d, %d)\n", e.button.x, e.button.y);
					switch (mode) {
						case 0:
							printf("Couleurs avant dessin : %d, %d, %d \n", r, g, b);
							glColor3ub(r, g, b);
							if (primitive == 0) {
								printf("DESSIN DE POINT\n");
								x1 = -1 + 2. * e.button.x / WINDOW_WIDTH;
								y1 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								glBegin(GL_POINTS);
									glVertex2f(x1, y1);
								glEnd();
								addPointToList(allocPoint(x1, y1, r, g, b), &list);
								x1 = 0;
								y1 = 0;
							} else if (primitive == 1) {
								printf("DESSIN DE LIGNE\n");
								if (!x1 || !y1) {
									x1 = -1 + 2. * e.button.x / WINDOW_WIDTH;
									y1 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								} else if (x1 && y1 && (!x2 || !y2)) {
									x2 = -1 + 2. * e.button.x / WINDOW_WIDTH;
									y2 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								}
								glBegin(GL_POINTS);
									glVertex2f(x1, y1);
								glEnd();
								if (x1 && y1 && x2 && y2) {
									glBegin(GL_LINES);
										glVertex2f(x1, y1);
										glVertex2f(x2, y2);
									glEnd();
									x1 = 0;
									y1 = 0;
									x2 = 0;
									y2 = 0;
								} else {
									printf("Cliquez à nouveau.\n");
								}
							} else if (primitive == 2) {
								printf("DESSIN DE LIGNE\n");
								if (!x1 || !y1) {
									x1 = -1 + 2. * e.button.x / WINDOW_WIDTH;
									y1 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								} else if (x1 && y1 && (!x2 || !y2)) {
									x2 = -1 + 2. * e.button.x / WINDOW_WIDTH;
									y2 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								} else if (x1 && y1 && x2 && y2 && (!x3 || !y3)) {
									x3 = -1 + 2. * e.button.x / WINDOW_WIDTH;
									y3 = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
								}
								glBegin(GL_POINTS);
									glVertex2f(x1, y1);
								glEnd();
								if (x2 && y2)
								{
									glBegin(GL_LINES);
										glVertex2f(x1, y1);
										glVertex2f(x2, y2);
									glEnd();
								}
								if (x1 && y1 && x2 && y2 && x3 && y3) {
									glBegin(GL_TRIANGLES);
										glVertex2f(x1, y1);
										glVertex2f(x2, y2);
										glVertex2f(x3, y3);
									glEnd();
									x1 = 0;
									y1 = 0;
									x2 = 0;
									y2 = 0;
									x3 = 0;
									y3 = 0;
								} else {
									printf("Cliquez à nouveau.\n");
								}
							}
							break;
						case 1:
							printf("Sélection de couleur\n");
							selectColor(e.button.x, &r, &g, &b);
							break;
						default:
							break;
					}
					break;

				/* Touche clavier appuyée */
				case SDL_KEYDOWN:
					printf("touche pressée (code = %d)\n", e.key.keysym.sym);
					switch (e.key.keysym.sym) {
						case SDLK_SPACE:
							mode = 1;
							break;
						case SDLK_p:
							primitive = 0;
							break;
						case SDLK_l:
							primitive = 1;
							break;
						case SDLK_t:
							primitive = 2;
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
					printf("touche relachée (code = %d)\n", e.key.keysym.sym);
					switch (e.key.keysym.sym) {
						case SDLK_SPACE:
							mode = 0;
							glClear(GL_COLOR_BUFFER_BIT);
							drawPoints(list);
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

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}
