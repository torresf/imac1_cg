#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

const char* filename = "logo_imac_400x400.jpg";

int main(int argc, char** argv) {

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("td04", NULL);
    resizeViewport();

    // TODO: Chargement et traitement de la texture
    SDL_Surface* image;
    image = IMG_Load(filename);
    if (image == NULL)
    {
        printf("Erreur de chargement de l'image.\n");
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        image->w,
        image->h,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);


    // TODO: Libération des données CPU
    SDL_FreeSurface(image);
    
    //Boucle de dessin (à décommenter pour l'exercice 3)
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        // TODO: Code de dessin

        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D); // On précise qu’on veut activer la fonctionnalité de texturing
        glBindTexture(GL_TEXTURE_2D, textureID); // On bind la texture pour pouvoir l’utiliser

        glPushMatrix();
            glTranslatef(-.5, -.5, 0); // Translate pour vérifier que la texture suit bien le quad
            glScalef(.7, .7, 1); // Translate pour vérifier que la texture suit bien le quad

            glBegin(GL_QUADS);
                glTexCoord2f(0,0);
                glVertex2f(-.5,.5);
                glTexCoord2f(1,0);
                glVertex2f(.5,.5);
                glTexCoord2f(1,1);
                glVertex2f(.5,-.5);
                glTexCoord2f(0,1);
                glVertex2f(-.5,-.5);
            glEnd();
        
        glPopMatrix();

        // Fin du code de dessin, on désactive la texture
        glDisable(GL_TEXTURE_2D); // On désactive le sampling de texture
        glBindTexture(GL_TEXTURE_2D, 0); // On débind la texture

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    //Fin de la boucle de dessin / rendu

    // TODO: Libération des données GPU
    glDeleteTextures(1, &textureID);

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
