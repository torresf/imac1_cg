#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926535
#define NB_POINTS 64
#define WIDTH 8
#define HEIGHT 6

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60.;

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

	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Système Solaire", NULL);

	resize();

	float scale_x, scale_y, translate_x, translate_y;
	scale_x = 1;
	scale_y = 1;
	translate_x = 0;
	translate_y = 0;
	glColor3ub(255, 255, 255); //Initialise la couleur à blanc

	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	int secondes = timeinfo->tm_sec;
	int old_secondes_i = timeinfo->tm_sec;
	int old_secondes_j = timeinfo->tm_sec;
	int old_secondes_k = timeinfo->tm_sec;
	float i = 0;
	float j = 0;
	float k = 0;

	/* Boucle d'affichage */
	int loop = 1;
	while(loop) {

		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		glClear(GL_COLOR_BUFFER_BIT);

		time (&rawtime);
		timeinfo = localtime (&rawtime);
		secondes = timeinfo->tm_sec;
		
		glPushMatrix();
			glScalef(scale_x, scale_y, 1);
			glTranslatef(translate_x, translate_y, 0);
		
			// Dessin du soleil
			glPushMatrix();
				glColor3ub(255, 255, 0);
				glScalef(1.392, 1.392, 1);
				drawCircle(1);
			glPopMatrix();

			// Dessin de Mercure
			glPushMatrix();
				glColor3ub(120, 120, 120);
				if (old_secondes_i == secondes) {
					i += (float) 1/40;
				} else {
					i = 0;
					old_secondes_i = secondes;
				}
				glRotatef((360 / 60)*(secondes+i), 0, 0, 1);
				glTranslatef(1, 0, 0);
				glScalef(.2, .2, 1);
				drawCircle(1);
			glPopMatrix();

			// Dessin de Venus
			glPushMatrix();
				glColor3ub(190, 190, 190);
				if (old_secondes_j == secondes) {
					j += (float) 1/40;
				} else {
					j = 0;
					old_secondes_j = secondes;
				}
				glRotatef(3*((360 / 60)*(secondes+j)), 0, 0, 1);
				glTranslatef(1.5, 0, 0);
				glScalef(.15, .15, 1);
				drawCircle(1);
			glPopMatrix();

			// Dessin de la Terre
			glPushMatrix();
				glColor3ub(20, 40, 250);
				if (old_secondes_k == secondes) {
					k += (float) 1/40;
				} else {
					k = 0;
					old_secondes_k = secondes;
				}
				glRotatef(6*((360 / 60)*(secondes+k)), 0, 0, 1);
				glTranslatef(2, 0, 0);
				glScalef(.2, .2, 1);
				drawCircle(1);
			glPopMatrix();
			
			// Dessin de Mars
			// glPushMatrix();
			// 	glColor3ub(200, 40, 40);
			// 	glTranslatef(227.90, 0, 0);
			// 	glScalef(.006794, .006794, 1);
			// 	drawCircle(1);
			// glPopMatrix();

			// // Dessin de Jupiter
			// glPushMatrix();
			// 	glColor3ub(255, 255, 100);
			// 	glTranslatef(778.34, 0, 0);
			// 	glScalef(.1428, .1428, 1);
			// 	drawCircle(1);
			// glPopMatrix();

			// // Dessin de Saturne
			// glPushMatrix();
			// 	glColor3ub(255, 255, 150);
			// 	glTranslatef(1427, 0, 0);
			// 	glScalef(.1206, .1206, 1);
			// 	drawCircle(1);
			// glPopMatrix();

			// // Dessin de Uranus
			// glPushMatrix();
			// 	glColor3ub(50, 70, 255);
			// 	glTranslatef(2869, 0, 0);
			// 	glScalef(.0513, .0513, 1);
			// 	drawCircle(1);
			// glPopMatrix();

			// // Dessin de Neptune
			// glPushMatrix();
			// 	glColor3ub(10, 20, 255);
			// 	glTranslatef(4490, 0, 0);
			// 	glScalef(.0491, .0491, 1);
			// 	drawCircle(1);
			// glPopMatrix();
		
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

				case SDL_MOUSEBUTTONDOWN:
					if (e.button.button == SDL_BUTTON_WHEELUP)
					{
						scale_x += .15;
						scale_y += .15;
					} else if (e.button.button == SDL_BUTTON_WHEELDOWN) {
						scale_x -= .15;
						scale_y -= .15;
					}
					break;

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
							break;
						case SDLK_LEFT:
							translate_x += .15;
							break;
						case SDLK_RIGHT:
							translate_x -= .15;
							break;
						case SDLK_UP:
							translate_y -= .15;
							break;
						case SDLK_DOWN:
							translate_y += .15;
							break;
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
	gluOrtho2D(-WIDTH/2., WIDTH/2., -HEIGHT/2., HEIGHT/2.);
}