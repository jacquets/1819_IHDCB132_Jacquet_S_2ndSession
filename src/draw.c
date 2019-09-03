/**
 *      Filename : draw.c
 *      Description:  simple display functions.
 *      Created:  18-07-19 | Revision: 20-07-19.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifdef _WIN32
#include <windows.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#elif __linux__
#include <GL/glut.h> /* GLUT, includes glu.h and gl.h */
#else
#error "OS not supported!"
#endif

#ifdef _WIN32
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#else
#include <SOIL/SOIL.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "game.h"
#include "write.h"
#include "map.h"
#include "render.h"
#include "score.h"

// ----------------------------------------------------
//  Jump and collision demonstration.
// ----------------------------------------------------

#include "event.h"

#define v_jump_cste -4
#define v_air_cste 1.5

bool jumping;
bool colliding;
double vx=v_air_cste;
double vy=v_jump_cste;
double v_gravity_cste=0.08;

typ_position a1={.posX=140,.posY=260,.sizeX=20,.sizeY=20};
typ_position b1={.posX=50,.posY=100,.sizeX=20,.sizeY=20};
typ_position c1={.posX=0,.posY=280,.sizeX=300,.sizeY=20};

// ----------------------------------------------------
//  Global variables environement.
// ----------------------------------------------------

GLuint *texID=NULL;

// ----------------------------------------------------
//  Texture.
// ----------------------------------------------------

GLuint *loadTex(void)
{
    GLuint* texID=NULL;
    texID=malloc(texNB*sizeof(GLuint));
    if(texID==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    texID[0]=ImageLoad(MONKEY1);
    texID[1]=ImageLoad(MONKEY2);
    texID[2]=ImageLoad(MONKEY3);
    texID[3]=ImageLoad(MONKEY4);
    texID[4]=ImageLoad(MONKEY5);
    texID[5]=ImageLoad(MONKEY6);
    texID[6]=ImageLoad(MONKEY7);
    texID[7]=ImageLoad(MONKEY8);
    texID[8]=ImageLoad(MONKEY9);
    texID[9]=ImageLoad(MONKEY10);

    texID[10]=ImageLoad(SNAKE1);
    texID[11]=ImageLoad(SNAKE2);
    texID[12]=ImageLoad(SNAKE3);
    texID[13]=ImageLoad(BACKGROUND);

    texID[14]=ImageLoad(COIN1);
    texID[15]=ImageLoad(COIN2);
    texID[16]=ImageLoad(COIN3);
    texID[17]=ImageLoad(COIN4);

    texID[18]=ImageLoad(TRELLIS);

    texID[19]=ImageLoad(JUMP1);
    texID[20]=ImageLoad(JUMP2);

    texID[21]=ImageLoad(LIVE);

    return texID;
}

// ----------------------------------------------------
//  Drawing functions.
// ----------------------------------------------------

void drawRect(float x, float y, float width, float height)
{
    //int a = ilGetInteger(IL_IMAGE_WIDTH);
    //int b = ilGetInteger(IL_IMAGE_HEIGHT);
    glBegin(GL_QUADS);
        glTexCoord2f (0.0f, 1.0f); glVertex2f(x,y); // vertex z default to 0.
        glTexCoord2f (1.0f, 1.0f); glVertex2f(x+width,y);
        glTexCoord2f (1.0f, 0.0f); glVertex2f(x+width,y+height);
        glTexCoord2f (0.0f, 0.0f); glVertex2f(x,y+height);
    glEnd();
}

void drawGame(float x, float y, float width, float height, int score)
{
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f (0.0f, 1.0f); glVertex2f(x,y); // vertex z default to 0.
            glTexCoord2f (1.0f, 1.0f); glVertex2f(x+width,y);
            glTexCoord2f (1.0f, 0.0f); glVertex2f(x+width,y+height);
            glTexCoord2f (0.0f, 0.0f); glVertex2f(x,y+height);
        glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
    vBitmapOutput(Square_size,HEIGHT-1,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*20,HEIGHT-1, buffer,GLUT_BITMAP_HELVETICA_12);
}

void drawTrellis(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
        glTexCoord2f (1.0f, 1.0f); glVertex2f(x,y); // vertex z default to 0.
        glTexCoord2f (0.0f, 1.0f); glVertex2f(x+width,y);
        glTexCoord2f (0.0f, 0.0f); glVertex2f(x+width,y+height);
        glTexCoord2f (1.0f, 0.0f); glVertex2f(x,y+height);
    glEnd();
}

void drawChar(float x, float y, float width, float height, typ_action action)
{
    glBegin(GL_QUADS);
    if(action==LEFT)
    {
        glTexCoord2f (0.0f, 1.0f); glVertex2f(x,y); // vertex z default to 0.
        glTexCoord2f (1.0f, 1.0f); glVertex2f(x+width,y);
        glTexCoord2f (1.0f, 0.0f); glVertex2f(x+width,y+height);
        glTexCoord2f (0.0f, 0.0f); glVertex2f(x,y+height);
    }
    else
    {
        glTexCoord2f (1.0f, 1.0f); glVertex2f(x,y); // vertex z default to 0.
        glTexCoord2f (0.0f, 1.0f); glVertex2f(x+width,y);
        glTexCoord2f (0.0f, 0.0f); glVertex2f(x+width,y+height);
        glTexCoord2f (1.0f, 0.0f); glVertex2f(x,y+height);
    }

    glEnd();
}

void background(void)
{
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	    glTranslatef(0.0,0.0,0.0f);
        glBindTexture(GL_TEXTURE_2D, texID[13]); // bind our texture.
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(Square_size*MaxX,0);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(Square_size*MaxX,Square_size*MaxY);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0,Square_size*MaxY);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
}

void menuDisplay(void)
{
    glBegin(GL_LINES);
        glColor3f(0.1f, 0.4f, 0.6f);
        glVertex2d(Square_size, HEIGHT-5*Square_size); // first point.
        glVertex2d(WIDTH-5*Square_size, HEIGHT-5*Square_size); // second point.
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2d(Square_size, Square_size); // upper/top right corner.
        glVertex2d(WIDTH-Square_size, Square_size); // lower right corner.
        glVertex2d(WIDTH-Square_size, HEIGHT-Square_size); // lower right corner.
        glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*30,Square_size*28,"EXIT",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*25,Square_size*23,"SAVE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*20,Square_size*18,"SCORE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*15,Square_size*13,"RULES",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*8,"GAME",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*28,"DIAPORAMA",GLUT_BITMAP_HELVETICA_18);
}

// Just for the presentation.
void keyDiaporama(int key)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
        diaporama++;
        break;
        case GLUT_KEY_LEFT:
        diaporama--;
        break;
        default:
            break;
    }
}

void diaporamaDisplay(int diaporama)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);
        glColor3f(0.2f,0.3f,0.8f);
            glVertex2i(0,0);
            glVertex2i(Square_size*MaxX,0);
            glVertex2i(Square_size*MaxX,Square_size*MaxY);
            glVertex2i(0,Square_size*MaxY);
        glColor3f(1.0f,1.0f,1.0f);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2d(Square_size, 5*Square_size); // first point.
            glVertex2d(WIDTH-Square_size, 5*Square_size); // second point.
        glColor3f(1.0f,1.0f,1.0f);
    glEnd();
    vBitmapOutput(Square_size*12,3*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_18);
    switch(diaporama)
    {
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
        case 3:
        break;
    }
}

void scoreDisplay(typ_data *lptr)
{
  	glClear(GL_COLOR_BUFFER_BIT);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

    glColor3f(0.5f, 0.5f, 0.5f);
      drawRect(0.0, 0.0, WIDTH, HEIGHT);
    glColor3f(1.0f, 1.0f, 1.0f);

    char buffer[255];
    int res;
    /*buffer=malloc(255*sizeof(char*));
    if(buffer==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
        exit(EXIT_FAILURE);
    }*/

    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_18);

    lptr=readData(SCORES);

    res=sprintf(buffer, "%s score: %d (%d-%d-%d %d:%d:%d)\n",lptr->data->username,lptr->data->score,lptr->data->time[0],lptr->data->time[1],lptr->data->time[2],
    lptr->data->time[3],lptr->data->time[4],lptr->data->time[5]);
    printf("%d : %s\n",res,buffer);
    vBitmapOutput(120,70, buffer,GLUT_BITMAP_HELVETICA_12);
    memset(buffer, 0, sizeof(*buffer) * 255);
    lptr = lptr->nextptr;

    res=sprintf(buffer, "%s score: %d\n",lptr->data->username,lptr->data->score);
    printf("%d : %s\n",res,buffer);
    vBitmapOutput(120,100, buffer,GLUT_BITMAP_HELVETICA_12);
    memset(buffer, 0, sizeof(*buffer) * 255);
    lptr = lptr->nextptr;

    res=sprintf(buffer, "%s score: %d\n",lptr->data->username,lptr->data->score);
    printf("%d : %s\n",res,buffer);
    vBitmapOutput(120,130, buffer,GLUT_BITMAP_HELVETICA_12);
    memset(buffer, 0, sizeof(*buffer) * 255);

    return;
}

void saveDisplay(typ_data *L)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		glBegin(GL_QUADS);
            glColor3f(0.2f,0.5f,1.0f);
            glVertex2i(0,0);
            glVertex2i(Square_size*MaxX,0);
            glVertex2i(Square_size*MaxX,Square_size*MaxY);
            glVertex2i(0,Square_size*MaxY);
            glColor3f(1.0f,1.0f,1.0f);
		glEnd();
    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*28,"EXIT",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*23,"SAVE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*18,"SCORE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*13,"RULES",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*8,"GAME",GLUT_BITMAP_HELVETICA_18);
}

void rulesDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		glBegin(GL_QUADS);
            glColor3f(0.5f,0.5f,1.0f);
            glVertex2i(0,0);
            glVertex2i(Square_size*MaxX,0);
            glVertex2i(Square_size*MaxX,Square_size*MaxY);
            glVertex2i(0,Square_size*MaxY);
            glColor3f(1.0f,1.0f,1.0f);
		glEnd();
	vBitmapOutput(Square_size*4,Square_size*4,"GAMES RULES for Jumping Banana",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*4,Square_size*6,"GAME KEYBOARD",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*8,"Q q : Go left",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*10,"D d : Go right",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*12,"Z z : Jump",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*14,"E e : Shot",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*6,"MENU",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*8,"PgUp : Go up in the menu",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*10,"PgDn : Go down in the menu",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*12,"Enter : enter",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*14,"ESC : leave",GLUT_BITMAP_HELVETICA_12);
}

void coordDisplay(void)
{
    vBitmapOutput(Square_size*2,Square_size*2,"(0,0)",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*2,HEIGHT - Square_size*2,"(0,Height)",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(WIDTH - Square_size*8,Square_size*2,"(Width,0)",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(WIDTH - Square_size*10,HEIGHT - Square_size*2,"(Width,Height)",GLUT_BITMAP_HELVETICA_12);
}

void winDisplay(void)
{
	glBegin(GL_QUADS);
      glColor3f(0.9f,0.5f,1.0f);
      glVertex2i(0,0);
      glVertex2i(Square_size*MaxX,0);
      glVertex2i(Square_size*MaxX,Square_size*MaxY);
      glVertex2i(0,Square_size*MaxY);
      glColor3f(1.0f,1.0f,1.0f);
	glEnd();

	vBitmapOutput(130,Square_size*MaxX/2,"Great, you win",GLUT_BITMAP_HELVETICA_18);
}

void loseDisplay(void)
{
	glBegin(GL_QUADS);
      glColor3f(0.9f,0.5f,1.0f);
      glVertex2i(0,0);
      glVertex2i(Square_size*MaxX,0);
      glVertex2i(Square_size*MaxX,Square_size*MaxY);
      glVertex2i(0,Square_size*MaxY);
      glColor3f(1.0f,1.0f,1.0f);
	glEnd();

	vBitmapOutput(130,Square_size*MaxX/2,"Game Over",GLUT_BITMAP_HELVETICA_18);
}

// ----------------------------------------------------
//  main.
// ----------------------------------------------------

#ifdef _DRAW
void display(void){
    menuDisplay();
    vBitmapOutput(Square_size*30,Square_size*25,"GAME",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*25,Square_size*20,"RULES",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*20,Square_size*15,"SCORE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*15,Square_size*10,"SAVE",GLUT_BITMAP_HELVETICA_18);
    vBitmapOutput(Square_size*10,Square_size*5,"EXIT",GLUT_BITMAP_HELVETICA_18);
    glutSwapBuffers();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
    glutInit(&argc,argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE); // Enable double buffered mode.
    glutInitWindowSize(MaxX*Square_size, MaxY*Square_size);
    glutCreateWindow("TEST"); // Create window with the given title.
    glutDisplayFunc(display); // Register callback handler for window re-paint event.
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
#endif

#ifdef _RANDOM
int main(int argc, char *argv[])
{
    int a = newRandomNumber(5,20);
    printf("Test random : %d",a);
    return 0;
}
#endif
