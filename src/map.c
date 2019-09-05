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
    typ_map* map;

    map=malloc(sizeof(typ_map));

    if(map==NULL){
		HandleErrors();
    }

    //memset((m),0,sizeof(typ_map));
    map->width=w;
    map->height=h;
    map->matrice=malloc(w*sizeof(unsigned int*)); //allocation mémoire pour le tableau
    map->banana=malloc(w*sizeof(unsigned int*));

	if(map->matrice==NULL || map->banana==NULL){
		free(map);
		HandleErrors();
    }

    int i;

    for(i=0;i<w;i++)
    {
        map->matrice[i]=malloc(h*sizeof(unsigned int));
        map->banana[i]=malloc(h*sizeof(unsigned int));
		if(map->matrice[i]==NULL || map->banana[i]==NULL){
			free(map);
			HandleErrors();
		}
    }

	map->tiles=malloc(TILE_NUMBER*sizeof(typ_tile));
	if(map->tiles==NULL){
		free(map->matrice); free(map->banana); free(map);
		HandleErrors();
    }

	map->tiles[0].texID=ImageLoad(MAP1);
	map->tiles[1].texID=ImageLoad(MAP2);
	map->tiles[2].texID=ImageLoad(MAP3);
	map->tiles[3].texID=ImageLoad(MAP4);
	map->tiles[4].texID=ImageLoad(BANANA);

	map->tiles[0].solid=0; // to determine if Monkey can go though the block in front of him.
	map->tiles[1].solid=1;
	map->tiles[2].solid=1;
	map->tiles[3].solid=0;
	map->tiles[4].solid=0;

    return map;
}

void initBanana(typ_map *map)
{
	int x,y;
	int a=map->width;
	int b=map->height;
	int c=0;
	int d=0;
	map->banana[5][36]=1;
    for(int i=0;i<bananaNB;i++)
    {
        a-=2; b-=3; c+=2; d+=3;
		x=newRandomNumber(c, a);
        y=newRandomNumber(d, b);
        map->banana[x][y]=1;
    }
}

// ----------------------------------------------------
//  Laod map.
// ----------------------------------------------------

typ_map* loadMap(char *level)
{
    typ_map* map;
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

    map=createMap(w,h);
	map->xscroll=0;
	map->yscroll=0;

    for(i=0;i<map->height;i++)
    {
        for(j=0;j<map->width;j++)
        {
            fscanf(F,"%d ",&tmp);
            map->matrice[j][i]=tmp;
            map->banana[j][i]=0;
        }
    }
	fclose(F);
	return map;
}

// ----------------------------------------------------
//  Display map.
// ----------------------------------------------------

void printMap(typ_map *map)
{
    int i,j;
    for(i=0; i<map->height; i++)
    {
        for(j=0; j<map->width; j++)
        {
            //printf("%d",map->banana[j][i]);
			if(map->banana[j][i]==1)
			{
				printf("%d,%d\n",j,i);
			}
        }
        //printf("\n");
    }
}

// fonction qui affiche les murs et les plateformes
void drawMap(typ_map *map)
{
	int i,j;
	int minX, maxX, minY, maxY;
	int tileNumber=0;
	minX=map->xscroll/Square_size-1;
	minY=map->yscroll/Square_size-1;
    maxX=(map->xscroll+WIDTH)/Square_size;
    maxY=(map->yscroll+HEIGHT)/Square_size;

	for(i=minX; i<=maxX; i++)
	{
		for(j=minY; j<=maxY; j++)
		{
			if (i<0 || i>=map->width || j<0 || j>=map->height)
			{

			}
			else
			{
				glPushMatrix();
					glTranslatef(i*Square_size-map->xscroll, j*Square_size-map->yscroll,0.0f);
					tileNumber=map->matrice[i][j];
					glBindTexture(GL_TEXTURE_2D, map->tiles[tileNumber].texID); // bind our texture.
					if(tileNumber)
						drawRect(0,0,Square_size,Square_size);
					glBindTexture(GL_TEXTURE_2D, 0);
				glPopMatrix();
			}
		}
	}
}


void drawBanana(typ_map *map)
{
	int i,j;
	int minX, maxX, minY, maxY;
	minX=map->xscroll/Square_size-1;
	minY=map->yscroll/Square_size-1;
    maxX=(map->xscroll+WIDTH)/Square_size;
    maxY=(map->yscroll+HEIGHT)/Square_size;

	for(i=minX; i<=maxX; i++)
	{
		for(j=minY; j<=maxY; j++)
		{
			if (i>0 && i<map->width && j>0 && j<map->height)
			{
				if(map->banana[i][j]==1)
				{
					glPushMatrix();
						glTranslatef(i*Square_size-map->xscroll, j*Square_size-map->yscroll,0.0f);
						glBindTexture(GL_TEXTURE_2D, map->tiles[4].texID); // bind our texture.
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

void freeMap(typ_map *map)
{

	if(map->tiles!=NULL){
			glDeleteTextures(1,&map->tiles[0].texID);  // Delete The Shader Texture.
			glDeleteTextures(1,&map->tiles[1].texID);
			glDeleteTextures(1,&map->tiles[2].texID);
			glDeleteTextures(1,&map->tiles[3].texID);
			glDeleteTextures(1,&map->tiles[4].texID);

			free(map->tiles);
	}
	if(map!=NULL){
        for(int i = 0; i < map->width; i++){
            free(map->matrice[i]);
            free(map->banana[i]);
        }
        free(map->matrice);
        free(map->banana);
        free(map);
    }
}

// ----------------------------------------------------
//  Main.
// ----------------------------------------------------

#ifdef _MAP

    typ_map *map;

void display(void){
	drawMap(map);
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
    map=loadMap("../../data/matrice/niveau3.lvl");
	glutInitWindowSize(map->width, map->height);

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
	typ_map *map=loadMap("../../data/matrice/niveau3.lvl");

	printMap(map);
	printf("width = %d\n",map->width);
	printf("height = %d\n",map->height);
	printf("address de map[2][5] = %p\n",&map->matrice[2][5]);
	printf("map[2][5] = %d\n",map->matrice[2][5]);

	freeMap(m);

    return 0;
}
#endif
