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

	glScalef(4, 4, 1);
	glColor3ub(255, 255, 255); //Initialise la couleur à blanc

	time_t rawtime;
	struct tm * timeinfo;
	
	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Photoshop assez moyen", NULL);

	/* Boucle d'affichage */
	int loop = 1;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Dessin de l'horloge
		glColor3ub(255, 255, 255);
		drawCircle(1);
		glColor3ub(0, 0, 0);
		int i;
		for (i = 1; i <= 60; ++i)
		{
			glPushMatrix();
				glRotatef((360 / 60)*i, 0, 0, 1);
				glTranslatef(.48, 0, 0);
				if (i%5 != 0) {
					glScalef(.05, .005, 1);
				} else {
					glScalef(.05, .01, 1);
				}
				drawSquare(1);
			glPopMatrix();
		}


		// Dessin des aiguilles
		time (&rawtime);
		timeinfo = localtime (&rawtime);

		glColor3ub(255, 0, 0);
		glPushMatrix();
			glRotatef((360 / 60)*(15 - timeinfo->tm_sec), 0, 0, 1);
			glTranslatef(.5, 0, 0);
			glScalef(.08, .01, 1);
			drawSquare(1);
		glPopMatrix();

		glColor3ub(0, 255, 0);
		glPushMatrix();
			glRotatef((360 / 60)*(15 - timeinfo->tm_min), 0, 0, 1);
			glScalef(.3, .01, 1);
			glTranslatef(.55, 0, 0);
			drawSquare(1);
		glPopMatrix();

		glColor3ub(0, 0, 255);
		glPushMatrix();
			glRotatef((360 / 12)*(3 - timeinfo->tm_hour), 0, 0, 1);
			glScalef(.15, .02, 1);
			glTranslatef(.55, 0, 0);
			drawSquare(1);
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