#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926535
#define NB_POINTS 64

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

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
	int i;
	float teta = 0;
	GLenum primitiveType = full ? GL_POLYGON : GL_LINE_LOOP;

	glBegin(primitiveType);
		for (i = 0; i < NB_POINTS; ++i)
		{
			teta = i * (2 * PI) / NB_POINTS;
			glVertex2f(cos(teta)/2, sin(teta)/2);
		}
	glEnd();
}

void resize(); //Redimensionne le viewport et rafraichit le repère

int main(int argc, char** argv) {

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

	glScalef(.1, .1, 1);
	glColor3ub(255, 255, 255); //Initialise la couleur à blanc
	

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Système Solaire", NULL);

	/* Boucle d'affichage */
	int loop = 1;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Dessin du soleil
		glPushMatrix();
			glColor3ub(255, 255, 0);
			glScalef(1.392, 1.392, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Mercure
		glPushMatrix();
			glColor3ub(120, 120, 120);
			glTranslatef(57.90, 0, 0);
			glScalef(.004878, .004878, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Venus
		glPushMatrix();
			glColor3ub(190, 190, 190);
			glTranslatef(108.21, 0, 0);
			glScalef(.0121, .0121, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Earth
		glPushMatrix();
			glColor3ub(20, 40, 250);
			glTranslatef(149.6, 0, 0);
			glScalef(.012756, .012756, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Mars
		glPushMatrix();
			glColor3ub(200, 40, 40);
			glTranslatef(227.90, 0, 0);
			glScalef(.006794, .006794, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Jupiter
		glPushMatrix();
			glColor3ub(255, 255, 100);
			glTranslatef(778.34, 0, 0);
			glScalef(.1428, .1428, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Saturne
		glPushMatrix();
			glColor3ub(255, 255, 150);
			glTranslatef(1427, 0, 0);
			glScalef(.1206, .1206, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Uranus
		glPushMatrix();
			glColor3ub(50, 70, 255);
			glTranslatef(2869, 0, 0);
			glScalef(.0513, .0513, 1);
			drawCircle(1);
		glPopMatrix();

		// Dessin de Neptune
		glPushMatrix();
			glColor3ub(10, 20, 255);
			glTranslatef(4490, 0, 0);
			glScalef(.0491, .0491, 1);
			drawCircle(1);
		glPopMatrix();




		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();
		
		/* Boucle traitant les événements */
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

	/* Liberation des ressources associées à la SDL */ 
	SDL_Quit();

	return EXIT_SUCCESS;
}

void resize() {
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE | SDL_GL_DOUBLEBUFFER);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4., 4., -3., 3.);
}