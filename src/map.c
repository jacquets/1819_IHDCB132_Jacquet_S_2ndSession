/**
 *      Filename : map.c
 *      Description:  chargement de la map.
 *      Created:  06-07-19 | Revision: 27-07-19.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */
// https://www.codeproject.com/Tips/881397/Csharp-Simple-D-Game-Physics-Gravity-Jumping-Movem
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
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "map.h"
#include "draw.h"
#include "render.h"

// ----------------------------------------------------
//  Global variable.
// ----------------------------------------------------

bool call_srand=0;

// ----------------------------------------------------
//  Random number.
// ----------------------------------------------------

void InitRandom(unsigned int n)
{
    srand(n) ;
    call_srand=1;
}

int newRandomNumber(int min, int max)
{
    InitRandom((unsigned)time(NULL));
    if(call_srand != 1)
        InitRandom((unsigned)time(NULL));
    return rand()%(max-min+1) + min;
}

// ----------------------------------------------------
//  Memory allocation.
// ----------------------------------------------------

void HandleErrors(){
    fprintf(stderr, "Error : dynamic allocation problem.\n");
    exit(EXIT_FAILURE); /* program left. */
}

typ_map *createMap(int w, int h)
{
    typ_map* m;

    m=malloc(sizeof(typ_map));

    if(m==NULL){
		HandleErrors();
    }

    //memset((m),0,sizeof(typ_map));
    m->width=w;
    m->height=h;
    m->matrice=malloc(w*sizeof(unsigned int*)); //allocation mémoire pour le tableau
    m->banana=malloc(w*sizeof(unsigned int*));

	if(m->matrice==NULL || m->banana==NULL){
		free(m);
		HandleErrors();
    }

    int i;

    for(i=0;i<w;i++)
    {
        m->matrice[i]=malloc(h*sizeof(unsigned int));
        m->banana[i]=malloc(h*sizeof(unsigned int));
		if(m->matrice[i]==NULL || m->banana[i]==NULL){
			free(m);
			HandleErrors();
		}
    }

	m->tiles=malloc(TILE_NUMBER*sizeof(typ_tile));
	if(m->tiles==NULL){
		free(m->matrice); free(m->banana); free(m);
		HandleErrors();
    }

	m->tiles[0].texID=ImageLoad(MAP1);
	m->tiles[1].texID=ImageLoad(MAP2);
	m->tiles[2].texID=ImageLoad(MAP3);
	m->tiles[3].texID=ImageLoad(MAP4);
	m->tiles[4].texID=ImageLoad(BANANA);

	m->tiles[0].solid=0; // to determine if Monkey can go though the block in front of him.
	m->tiles[1].solid=1;
	m->tiles[2].solid=1;
	m->tiles[3].solid=0;
	m->tiles[4].solid=0;

    return m;
}

void initBanana(typ_map *m)
{
	int x,y;
	int a=m->width;
	int b=m->height;
	int c=0;
	int d=0;
	m->banana[5][36]=1;
    for(int i=0;i<bananaNB;i++)
    {
        a-=2; b-=3; c+=2; d+=3;
		x=newRandomNumber(c, a);
        y=newRandomNumber(d, b);
        m->banana[x][y]=1;
    }
}

// ----------------------------------------------------
//  Laod map.
// ----------------------------------------------------

typ_map* loadMap(char *level)
{
    typ_map* m;
	FILE* F;
	F=fopen(level,"r");
    if(F==NULL){
        fprintf(stderr,"Error : No such file or directory : %s\n",level);
        fprintf(stderr, "Error : %s\n",strerror(errno)); /* On affiche sur le canal d'erreur standard, "stderr". */
        perror("Error ");
        exit(EXIT_FAILURE); /* program left. */
    }

    int i,j;
    int w,h;
    int tmp;
    char *bufferName=malloc(sizeof(char)*50);
    if(bufferName==NULL){
        perror("Error ");
      	exit(EXIT_FAILURE);
    }

    fscanf(F,"%s",bufferName); // level name.
	fscanf(F,"%d %d",&w,&h); // number of square on the width/height.

    m=createMap(w,h);
	m->xscroll=0;
	m->yscroll=0;

    for(i=0;i<m->height;i++)
    {
        for(j=0;j<m->width;j++)
        {
            fscanf(F,"%d ",&tmp);
            m->matrice[j][i]=tmp;
            m->banana[j][i]=0;
        }
    }
	fclose(F);
	return m;
}

// ----------------------------------------------------
//  Display map.
// ----------------------------------------------------

void printMap(typ_map *m)
{
    int i,j;
    for(i=0; i<m->height; i++)
    {
        for(j=0; j<m->width; j++)
        {
            //printf("%d",m->banana[j][i]);
			if(m->banana[j][i]==1)
			{
				printf("%d,%d\n",j,i);
			}
        }
        //printf("\n");
    }
}

// fonction qui affiche les murs et les plateformes
void drawMap(typ_map *m)
{
	int i,j;
	int minX, maxX, minY, maxY;
	int tileNumber=0;
	minX=m->xscroll/Square_size-1;
	minY=m->yscroll/Square_size-1;
    maxX=(m->xscroll+WIDTH)/Square_size;
    maxY=(m->yscroll+HEIGHT)/Square_size;

	for(i=minX; i<=maxX; i++)
	{
		for(j=minY; j<=maxY; j++)
		{
			if (i<0 || i>=m->width || j<0 || j>=m->height)
			{

			}
			else
			{
				glPushMatrix();
					glTranslatef(i*Square_size-m->xscroll, j*Square_size-m->yscroll,0.0f);
					tileNumber=m->matrice[i][j];
					glBindTexture(GL_TEXTURE_2D, m->tiles[tileNumber].texID); // bind our texture.
					if(tileNumber)
						drawRect(0,0,Square_size,Square_size);
					glBindTexture(GL_TEXTURE_2D, 0);
				glPopMatrix();
			}
		}
	}
}


void drawBanana(typ_map *m)
{
	int i,j;
	int minX, maxX, minY, maxY;
	minX=m->xscroll/Square_size-1;
	minY=m->yscroll/Square_size-1;
    maxX=(m->xscroll+WIDTH)/Square_size;
    maxY=(m->yscroll+HEIGHT)/Square_size;

	for(i=minX; i<=maxX; i++)
	{
		for(j=minY; j<=maxY; j++)
		{
			if (i>0 && i<m->width && j>0 && j<m->height)
			{
				if(m->banana[i][j]==1)
				{
					glPushMatrix();
						glTranslatef(i*Square_size-m->xscroll, j*Square_size-m->yscroll,0.0f);
						glBindTexture(GL_TEXTURE_2D, m->tiles[4].texID); // bind our texture.
						drawRect(0,0,Square_size,Square_size);
						glBindTexture(GL_TEXTURE_2D, 0);
					glPopMatrix();
				}
			}
		}
	}
}

// ----------------------------------------------------
//  Free map.
// ----------------------------------------------------

void freeMap(typ_map *m)
{

	if(m->tiles!=NULL){
			glDeleteTextures(1,&m->tiles[0].texID);  // Delete The Shader Texture.
			glDeleteTextures(1,&m->tiles[1].texID);
			glDeleteTextures(1,&m->tiles[2].texID);
			glDeleteTextures(1,&m->tiles[3].texID);
			glDeleteTextures(1,&m->tiles[4].texID);

			free(m->tiles);
	}
	if(m!=NULL){
        for(int i = 0; i < m->width; i++){
            free(m->matrice[i]);
            free(m->banana[i]);
        }
        free(m->matrice);
        free(m->banana);
        free(m);
    }
}

// ----------------------------------------------------
//  Main.
// ----------------------------------------------------

#ifdef _MAP

    typ_map *m;

void display(void){
	drawMap(m);
	glutSwapBuffers();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, width, 0, height);
	gluOrtho2D( width,0, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
    glutInit(&argc,argv); // Initialize GLUT.
  	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE); // Enable double buffered mode.
  	glutCreateWindow("TEST"); // Create window with the given title.

#ifdef _WIN32
  InitImage(); // Our own DevIL initialization
#endif
    m=loadMap("../../data/matrice/niveau3.lvl");
	glutInitWindowSize(m->width, m->height);

	glutDisplayFunc(display); // Register callback handler for window re-paint event.
	glutReshapeFunc(reshape);
	glEnable(GL_TEXTURE_2D);
	glutMainLoop();
    return 0;
}
#endif
#ifdef _MAP_bis
int main(int argc, char *argv[])
{
	typ_map *m=loadMap("../../data/matrice/niveau3.lvl");

	printMap(m);
	printf("width = %d\n",m->width);
	printf("height = %d\n",m->height);
	printf("address de map[2][5] = %p\n",&m->matrice[2][5]);
	printf("map[2][5] = %d\n",m->matrice[2][5]);

	freeMap(m);

    return 0;
}
#endif
