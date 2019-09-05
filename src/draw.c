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

    texID[22]=ImageLoad("../../data/dia/str_map.PNG");
    texID[23]=ImageLoad("../../data/dia/str_tile.PNG");
    texID[24]=ImageLoad("../../data/dia/InitAudio.PNG");
    texID[25]=ImageLoad("../../data/dia/InitGame.PNG");
    texID[26]=ImageLoad("../../data/dia/freeAll.PNG");

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
    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*30,Square_size*28,"EXIT",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*25,Square_size*23,"SAVE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*20,Square_size*18,"SCORE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*15,Square_size*13,"RULES",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*10,"GAME",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*28,"DIAPORAMA",GLUT_BITMAP_HELVETICA_12);
}

void diaporamaDisplay(typ_diaporama diaporama)
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
            glVertex2d(Square_size, 5*Square_size-Square_size*diaporama.scroll); // first point.
            glVertex2d(WIDTH-Square_size, 5*Square_size-Square_size*diaporama.scroll); // second point.
        glColor3f(1.0f,1.0f,1.0f);
    glEnd();
    vBitmapOutput(Square_size*12,3*Square_size-Square_size*diaporama.scroll,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_18);
    
    int i=10;
    switch(diaporama.slide)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,"Structure de fichier",GLUT_BITMAP_HELVETICA_18);
            if(diaporama.slide==0)
                glColor3f(1.0f, 0.0f, 0.0f);           
            i+=2;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,"/repository",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);
            if(diaporama.slide==1)
                glColor3f(1.0f, 0.0f, 0.0f);
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"build",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);
            if(diaporama.slide==1){
                i+=2; 
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"*.o",GLUT_BITMAP_HELVETICA_12);
                i+=2;                
            }
            if(diaporama.slide==2)
                glColor3f(1.0f, 0.0f, 0.0f);
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"include",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);
            if(diaporama.slide==2){
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"GL",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"IL",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"FMOD",GLUT_BITMAP_HELVETICA_12);                
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"*.h",GLUT_BITMAP_HELVETICA_12);
            }
            if(diaporama.slide==3)
                glColor3f(1.0f, 0.0f, 0.0f);           
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"lib",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);
            if(diaporama.slide==3){
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"linux",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*14,Square_size*i-Square_size*diaporama.scroll,"*.so",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"win32",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*14,Square_size*i-Square_size*diaporama.scroll,"*.a",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*14,Square_size*i-Square_size*diaporama.scroll,"*.lib",GLUT_BITMAP_HELVETICA_12);
            }
            if(diaporama.slide==4)
                glColor3f(1.0f, 0.0f, 0.0f);  
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"src",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);
            if(diaporama.slide==4){
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"*.c",GLUT_BITMAP_HELVETICA_12);
            }
            if(diaporama.slide==5)
                glColor3f(1.0f, 0.0f, 0.0f);
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"release",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f);       
            if(diaporama.slide==5){
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"linux",GLUT_BITMAP_HELVETICA_12);
                i+=2;
                vBitmapOutput(Square_size*10,Square_size*i-Square_size*diaporama.scroll,"win32",GLUT_BITMAP_HELVETICA_12);
            }
            if(diaporama.slide==6)
                glColor3f(1.0f, 0.0f, 0.0f);      
            i+=2;
            vBitmapOutput(Square_size*6,Square_size*i-Square_size*diaporama.scroll,"Makefile",GLUT_BITMAP_HELVETICA_12);
            glColor3f(1.0f,1.0f,1.0f); 
            vBitmapOutput(Square_size*4,HEIGHT-Square_size,"Note: Linux command : [~]$ tree | Latex : \\usepackage{dirtree}",GLUT_BITMAP_HELVETICA_12);
            i=10;
        break;
        case 7:
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            ".h = header file, a source file containing declarations.",GLUT_BITMAP_HELVETICA_12);
            i+=4;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            ".lib (win) or .a (linux, win) = static library.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            "May contain code or just links to a dynamic library.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            "The static library is included in the .exe.",GLUT_BITMAP_HELVETICA_12);
            i+=4;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            ".dll (win) or .so (linux) = dynamic library.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*4,Square_size*i-Square_size*diaporama.scroll,
            "Just like a static one but it's loaded at run time.",GLUT_BITMAP_HELVETICA_12);
            i=10;
        break;
        case 8:
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Tile Mapping",GLUT_BITMAP_HELVETICA_18);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Une structure Map, definie dans le fichier map.h.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Un fichier .lvl, donne le nombre de bloc en largeur et hauteur.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Un tableau a plus d'une dimension, ou est stockee la map.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Un tableau aux dimensions identiques, ou est stockee les bananes.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Deux entiers pour reafficher la map au depart de xscroll & yscroll.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            glBindTexture(GL_TEXTURE_2D, texID[22]); // bind our texture.
            float x=368*SCALE, y=208*SCALE;
            //if(WIDTH>glutGet(GLUT_WINDOW_WIDTH))
            //https://github.com/jacquets/1819_IHDCB132_Jacquet_S_2ndSession/blame/master/include/map.h
            glTranslatef(Square_size*4,Square_size*i-Square_size*diaporama.scroll,0.0f);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0,0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x,0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x,y);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0,y);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            //i+=y/Square_size-Square_size*diaporama.scroll;
            //vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Note: .",GLUT_BITMAP_HELVETICA_12);
            i=10;
        break;
        case 9:
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Tile Mapping",GLUT_BITMAP_HELVETICA_18);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Une image (sprite) contenue à l'adresse du pointeur texID.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"L autre champ, un entier dit si on peux traverser ou pas.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            x=389*SCALE, y=152*SCALE;
            glTranslatef(Square_size*4,Square_size*i-Square_size*diaporama.scroll,0.0f);
            glBindTexture(GL_TEXTURE_2D, texID[23]); // bind our texture.
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0,0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x,0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x,y);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0,y);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            //i+=y/Square_size-Square_size*diaporama.scroll;
            //vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Note: .",GLUT_BITMAP_HELVETICA_12);
            //i+=2;
        break;
        case 10:
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (1/10) : Main (main.c).",GLUT_BITMAP_HELVETICA_18);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Definition de la taille et la position de l'écran.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Charger les images et les sons une seule fois.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Mais aussi initialiser les variables du jeu, champs de structure.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Afficher (et reafficher) l ecran de menu.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
        break;
        case 11:
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (2/10) : Audio_init (audio.c).",GLUT_BITMAP_HELVETICA_18);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"Audio_init(); = charge la musique.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"playAudio(0,1); = fonction qui joue la piste 0 si le second arg. est sur 1.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"playAudio(0,1); = fonction qui joue la piste 0 si le second arg. est sur 1.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"stopAudio(0); = fonction qui stoppe la piste '0' en cours.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            vBitmapOutput(Square_size*2,Square_size*i-Square_size*diaporama.scroll,"stopAudio(0); = fonction qui stoppe la piste '0' en cours.",GLUT_BITMAP_HELVETICA_12);
            i+=2;
            x=881*SCALE*0.8, y=252*SCALE*0.8;
            glTranslatef(Square_size*4-Square_size*diaporama.scroll,Square_size*i,0.0f);
            glBindTexture(GL_TEXTURE_2D, texID[24]); // bind our texture.
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0,0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x,0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x,y);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0,y);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        break;
        case 12:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les prototypes utilises de Audio_init (audio.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_Create(FMOD_SYSTEM **system);.",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_Init(FMOD_SYSTEM *system",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",int maxchannels",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_INITFLAGS flags",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",void *extradriverdata);.",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_CreateSound(FMOD_SYSTEM *system",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",const char *name_or_data",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_MODE mode",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_CREATESOUNDEXINFO *exinfo",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_SOUND **sound);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_Sound_SetLoopCount(FMOD_SOUND *sound",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",int loopcount);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_PlaySound(FMOD_SYSTEM *system",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_SOUND *sound",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_CHANNELGROUP *channelgroup",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_BOOL paused",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,",FMOD_CHANNEL **channel);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_Channel_IsPlaying(FMOD_CHANNEL *channel,FMOD_BOOL *isplaying);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_Sound_Release(FMOD_SOUND *sound);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_Close(FMOD_SYSTEM *system);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_Release(FMOD_SYSTEM *system);",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"FMOD_RESULT FMOD_System_GetVersion(FMOD_SYSTEM *system,unsigned int *version);",GLUT_BITMAP_HELVETICA_10);
            i=0;
        break;
        case 13:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Fonction appelée une fois par la main.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Elle appelle les fonctions de chargement d'images, de maps, ect.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            x=497*SCALE*0.8, y=555*SCALE*0.8;
            glTranslatef(Square_size*4,Square_size*i-Square_size*diaporama.scroll,0.0f);
            glBindTexture(GL_TEXTURE_2D, texID[25]); // bind our texture.
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0,0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x,0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x,y);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0,y);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        break;
        case 14:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Necessite que des variables soient definies au prealable:",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"GLuint window, window2;",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_state state=MENU; // to init game state.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_state selectedMenu=GAME;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_action action=RIGHT; // to init game menu.",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_data *scoreList=NULL; // list to manage Game Data.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_map *map=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_game *game=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_character *player=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_character *ennemy=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_object *object=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_decor *decor=NULL;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"GLboolean lauded=0;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"GLboolean cleaned=0;",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"GLboolean pause=0;",GLUT_BITMAP_HELVETICA_10);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"typ_diaporama diaporama={.slide=0,.scroll=0};",GLUT_BITMAP_HELVETICA_10);
            i+=2;
        break;
        case 15:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Fonction appelée une fois par la main.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
            x=567*SCALE*0.8, y=374*SCALE*0.8;
            glTranslatef(Square_size*4,Square_size*i-Square_size*diaporama.scroll,0.0f);
            glBindTexture(GL_TEXTURE_2D, texID[25]); // bind our texture.
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0,0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x,0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x,y);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0,y);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        break;
        case 16:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Fonction appelée une fois par la main.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
        break;
        case 17:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Fonction appelée une fois par la main.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
        break;
        case 18:
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Les fonctions (3/10) : gameInit (game.c).",GLUT_BITMAP_HELVETICA_18);
            i+=4;
            vBitmapOutput(Square_size,Square_size*i-Square_size*diaporama.scroll,"Fonction appelée une fois par la main.",GLUT_BITMAP_HELVETICA_10);
            i+=2;
        break;
        default:
            break;
    }
}

void scoreDisplay(typ_data *lptr)
{
  	glClear(GL_COLOR_BUFFER_BIT);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

    glColor3f(0.2f, 0.7f, 0.1f);
      drawRect(0.0, 0.0, WIDTH, HEIGHT);
    glColor3f(1.0f, 1.0f, 1.0f);

    char buffer[255];
    //int res;
    /*buffer=malloc(255*sizeof(char*));
    if(buffer==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
        exit(EXIT_FAILURE);
    }*/

    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_12);

    lptr=readData(SCORES);

    /*res=*/sprintf(buffer, "%s score: %d (%d-%d-%d %d:%d:%d)\n",lptr->data->username,lptr->data->score,lptr->data->time[0],lptr->data->time[1],lptr->data->time[2],
    lptr->data->time[3],lptr->data->time[4],lptr->data->time[5]);
    vBitmapOutput(120,70, buffer,GLUT_BITMAP_HELVETICA_12);
    memset(buffer, 0, sizeof(*buffer) * 255);
    lptr = lptr->nextptr;

    sprintf(buffer, "%s score: %d\n",lptr->data->username,lptr->data->score);
    vBitmapOutput(120,100, buffer,GLUT_BITMAP_HELVETICA_12);
    memset(buffer, 0, sizeof(*buffer) * 255);
    lptr = lptr->nextptr;

    sprintf(buffer, "%s score: %d\n",lptr->data->username,lptr->data->score);
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
    vBitmapOutput(Square_size*10,HEIGHT-2*Square_size,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*28,"EXIT",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*23,"SAVE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*18,"SCORE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*13,"RULES",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*10,"GAME",GLUT_BITMAP_HELVETICA_12);
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
	vBitmapOutput(Square_size*4,Square_size*4,"GAMES RULES for Jumping Banana",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*6,"GAME KEYBOARD",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*10,"Q q : Go left",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*10,"D d : Go right",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*12,"Z z : Jump",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*4,Square_size*14,"E e : Shot",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*6,"MENU",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*10,"PgUp : Go up in the menu",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*10,"PgDn : Go down in the menu",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*12,"Enter : enter",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*MaxX/2,Square_size*14,"ESC : leave",GLUT_BITMAP_HELVETICA_12);
}

void coordDisplay(void)
{
    vBitmapOutput(Square_size*2,Square_size*2,"(0,0)",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*2,HEIGHT - Square_size*2,"(0,Height)",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(WIDTH - Square_size*10,Square_size*2,"(Width,0)",GLUT_BITMAP_HELVETICA_12);
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

	vBitmapOutput(130,Square_size*MaxX/2,"Great, you win",GLUT_BITMAP_HELVETICA_12);
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

	vBitmapOutput(130,Square_size*MaxX/2,"Game Over",GLUT_BITMAP_HELVETICA_12);
}

// ----------------------------------------------------
//  main.
// ----------------------------------------------------

#ifdef _DRAW
void display(void){
    menuDisplay();
    vBitmapOutput(Square_size*30,Square_size*25,"GAME",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*25,Square_size*20,"RULES",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*20,Square_size*15,"SCORE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*15,Square_size*10,"SAVE",GLUT_BITMAP_HELVETICA_12);
    vBitmapOutput(Square_size*10,Square_size*5,"EXIT",GLUT_BITMAP_HELVETICA_12);
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
