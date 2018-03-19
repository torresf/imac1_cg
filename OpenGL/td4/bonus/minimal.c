#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

const char* filename = "logo_imac_400x400.png";
const char* image_colon_file = "numbers/colon.png";
const char* image0_file = "numbers/0.png";
const char* image1_file = "numbers/1.png";
const char* image2_file = "numbers/2.png";
const char* image3_file = "numbers/3.png";
const char* image4_file = "numbers/4.png";
const char* image5_file = "numbers/5.png";
const char* image6_file = "numbers/6.png";
const char* image7_file = "numbers/7.png";
const char* image8_file = "numbers/8.png";
const char* image9_file = "numbers/9.png";

GLuint createTexture(const char* filename) {
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
        GL_RGBA,
        image->w,
        image->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image->pixels);

    SDL_FreeSurface(image);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

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
    GLuint image_colon = createTexture(image_colon_file);
    GLuint image0 = createTexture(image0_file);
    GLuint image1 = createTexture(image1_file);
    GLuint image2 = createTexture(image2_file);
    GLuint image3 = createTexture(image3_file);
    
    //Boucle de dessin (à décommenter pour l'exercice 3)
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    time_t now;
    struct tm *tm;

    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        // TODO: Code de dessin

        glClear(GL_COLOR_BUFFER_BIT);

        now = time(0);
        if ((tm = localtime (&now)) == NULL) {
            printf ("Error extracting time stuff\n");
            return 1;
        }

        printf("%02d:%02d:%02d\n",
            tm->tm_hour, tm->tm_min, tm->tm_sec);

        glEnable(GL_TEXTURE_2D); // On précise qu’on veut activer la fonctionnalité de texturing

        printf("%d\n", tm->tm_sec);
        switch(tm->tm_sec){
            case 0:
                glBindTexture(GL_TEXTURE_2D, image0); // On bind la texture pour pouvoir l’utiliser
                break;
            case 1:
                glBindTexture(GL_TEXTURE_2D, image1); // On bind la texture pour pouvoir l’utiliser
                break;
            case 2:
                glBindTexture(GL_TEXTURE_2D, image2); // On bind la texture pour pouvoir l’utiliser
                break;
            case 3:
                glBindTexture(GL_TEXTURE_2D, image3); // On bind la texture pour pouvoir l’utiliser
                break;
            default: 
                glBindTexture(GL_TEXTURE_2D, image_colon); // On bind la texture pour pouvoir l’utiliser
        }

        glPushMatrix();
            // glTranslatef(-.5, -.5, 0); // Translate pour vérifier que la texture suit bien le quad
            glScalef(.3, .6, 1); // Translate pour vérifier que la texture suit bien le quad

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
    glDeleteTextures(1, &image_colon);
    glDeleteTextures(1, &image0);
    glDeleteTextures(1, &image1);
    glDeleteTextures(1, &image2);
    glDeleteTextures(1, &image3);

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
