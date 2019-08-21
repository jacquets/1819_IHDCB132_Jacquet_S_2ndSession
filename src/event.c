/**
 *      Filename : event.h
 *      Description:  updating functions.
 *      Created:  09-08-19 | Revision: --.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "event.h"
#include "map.h"
#include "game.h"
#include "draw.h"


// ----------------------------------------------------
//  Update functions.
// ----------------------------------------------------

typ_position *adjustment(typ_position *test, typ_map *m)
{
    while(collision(test, m))
    {
        test->posY-=0.1;
    }
    test->posY+=0.1;
    return test;
}

void bananaCollision(typ_character *c, typ_map *m, typ_game *g)
{
    int minX = (c->posChar->posX + c->posChar->sizeX/2) / Square_size;
    int minY = (c->posChar->posY + c->posChar->sizeY/2) / Square_size;
    int maxX = Square_size;
    int maxY = Square_size;

    if(m->banana[minX][minY]==1)
    {
        if((minX + maxX) < c->posChar->posX
        || (c->posChar->posX + c->posChar->sizeX) < minX
        || (minY + maxY) < c->posChar->posY
        || (c->posChar->posY + c->posChar->sizeY) < minY)
        {
            m->banana[minX][minY]=0;
            g->score++;
        }
    }
    return;
}

void updatePosition(typ_character *c, typ_map *m, typ_decor *d, typ_game *g)
{

    if(collision(c->posChar,m))
    {
        if(c->isJumping==true)
        {
            c->posChar=adjustment(c->posChar,m);
            c->isJumping=false;
            c->action=NONE;
        }
    }
    else if(isColliding(c->posChar, d->spiderWeb))
    {
        if(c->isJumping==true)
        {
            c->isJumping=false;
            c->action=DOWN;
        }
    }
    else
    {
        c->action=DOWN;
        c->v_y += v_grav; // incrementation of the speed.
        c->posChar->posY += c->v_y; // incrementation of the position.
    }
    bananaCollision(c, m, g);
}

int collision(typ_position* test, typ_map* m)
{
    int collision_down/*, collision_up, collision_left, collision_right*/;
    int i=0, j=0, tileIndice;
    int minX, maxX;
    //int minY, maxY;
  	minX = test->posX / Square_size;
  	//minY = test->posY / Square_size;
  	maxX = (test->posX + test->sizeX -1) / Square_size;
  	//maxY = (test->posY + test->sizeY -1) / Square_size;

if(test->sizeY + test->posY < m->height * Square_size - 10)
{
    // If we hit a block below.
    j = (test->posY + test->sizeY) / Square_size;
    for(i=minX;i<=maxX;i++)
    {
        tileIndice=m->matrice[i][j];
        collision_down=(m->tiles[tileIndice].solid);
    }
    /*
    // If we hit a block to the left.
    i=(test->posX -1) / Square_size;
    for(j=minY;j<=maxY;j++)
    {
        tileIndice = m->matrice[i][j];
        collision_left=(m->tiles[tileIndice].solid);
        j=maxY+1;
    }
    // If we hit a block to the right.
    i=(test->posX + test->sizeX) / Square_size;
    for(j=minY;j<=maxY;j++)
    {
        tileIndice = m->matrice[i][j];
        collision_right=(m->tiles[tileIndice].solid);
        j=maxY+1;
    }
    // If we hit a block above.
    j = (test->posY - 1) / Square_size;
    for(i=minX;i<=maxX;i++) //Si il y a un bloc au dessus
    {
        tileIndice = m->matrice[i][j];
        collision_up=(m->tiles[tileIndice].solid);
    }
    */
}
return collision_down /*|| collision_left || collision_right || collision_up*/;
}

void updateTests(typ_character* c, typ_map* m)
{
    // bord gauche
    if (c->posChar->posX-1<0) //Si on touche le bord gauche de la map
    {
        c->posChar->posX--;
    }
    // bord droit
    if (c->posChar->posX + c->posChar->sizeY >= m->width*Square_size-2)
    {
        c->posChar->posX--;
    }
    // bord haut
    if (c->posChar->posY<0)
    {
        c->isJumping=false;
        c->posChar->posY+=10;
        c->v_y+=10;
    }
/*
    if(c->action==JUMP)
    {
        if(c->action==JUMP)
        {
            if(c->jumptime<=1)
            {
                playAudio(4,1);
            }
            c->jumptime+=10;
            c->posChar->posY-=10;
            if(c->jumptime>1000)
            {
//Ne pas sautez tout le temps si on laisse la touche haut activée
            }
        }
    }
*/
                //winDisplay();
                //loseDisplay();
}

void updateSnake(typ_character *e, typ_character *c, typ_map *m)
{
    // testing limits.
    if(e->action==RIGHT)
    {
      if(e->posChar->posX < c->posChar->posX)
          e->posChar->posX+=1;
      else
          e->action=LEFT;
    }
    if(e->action==LEFT)
    {
      if(e->posChar->posX > c->posChar->posX)
          e->posChar->posX-=1;
      else
        e->action=RIGHT;
    }


    // Ground collision else gravity.
    int minX=(e->posChar->posX + e->posChar->sizeX/2) / Square_size;
    int minY=(e->posChar->posY + e->posChar->sizeY) / Square_size;
    if(m->matrice[minX][minY]==1)
    {
        adjustment(e->posChar, m);
    }
    else
    {
        // Gravity.
        e->v_y += v_grav;
        e->posChar->posY += e->v_y;
    }
}

GLboolean CheckCollision(typ_position *one, typ_position *two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one->posX + one->sizeX >= two->posX &&
        two->posX + two->sizeX >= one->posX;

    // Collision y-axis?
    bool collisionY = one->posY + one->sizeY >= two->posY &&
        two->posY + two->sizeY >= one->posY;

    // Collision only if on both axes
    return collisionX && collisionY;

}

bool isColliding(typ_position *one, typ_position *two) // AABB - AABB collision
{
    return !((one->posX + one->sizeX) < two->posX
    || (two->posX + two->sizeX) < one->posX
    || (one->posY + one->sizeY) < two->posY
    || (two->posY + two->sizeY) < one->posY);
}

// ----------------------------------------------------
//  main _EVENT (training environnement).
// ----------------------------------------------------

#ifdef _EVENT

typ_position one={.posX=140,.posY=260,.sizeX=20,.sizeY=20};
typ_position two={.posX=50,.posY=100,.sizeX=20,.sizeY=20};
typ_position three={.posX=0,.posY=280,.sizeX=300,.sizeY=20};

bool isJumping;
bool isColl;

    #define v_jump -4
    #define v_air 1.5
    /* initialisation des vitesses */
    double v_grav = 0.08;
    double v_x = v_air;
    double v_y = v_jump;

int adjust(void)
{
    while(isColliding(&one,&two) || isColliding(&one,&three))
    {
        one.posY-=0.1;
    }
    return one.posY+=0.1;
}

void updateTest()
{
    if(isColliding(&one,&two) || isColliding(&one,&three))
    {
        if(isJumping==true)
        {
            adjust();
            isJumping=false;
        }
    }
    else
    {
        v_y += v_grav; /* evolution de la vitesse */
        one.posY += v_y; /* evolution de la position */
    }
}

void loopTest();
void timerTest(int value);
void reshapeTest(int NewWidth,int NewHeight);
void displayTest(void);
void keyTest(int key, int x, int y);

int main(int argc, char *argv[]){
    glutInit(&argc,argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE); // Enable double buffered mode.
    glutInitWindowSize(300, 300);
    glutCreateWindow("TEST"); // Create window with the given title.
    glutDisplayFunc(displayTest); // Register callback handler for window re-paint event.
    glutSpecialFunc(keyTest);
    glutIdleFunc(displayTest);	// callback activiting.
	glutTimerFunc(30, timerTest, 0);
    glutReshapeFunc(reshapeTest);
    glutMainLoop();
    return 0;
}

void loopTest()
{
    updateTest();
    //rendering();
}

void timerTest(int value)
{
    loopTest();
    glutPostRedisplay();
    glutTimerFunc(30, timerTest, 0);
}

void keyTest(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_UP:
            isJumping=true;
            v_y = v_jump;
            one.posY--;
            break;
        case GLUT_KEY_RIGHT:
            one.posX++;
            /* direction en vol */
            if(isJumping==true)
            {
                one.posX += v_x;
            }
            break;
        case GLUT_KEY_LEFT:
            one.posX--;
            /* direction en vol */
            if(isJumping==true)
            {
                one.posX -= v_x;
            }
            break;
        default:
            break;
    }
}

void reshapeTest(int NewWidth,int NewHeight)
{
	if (NewHeight==0)
    {
        NewHeight=1; // Prevent A Divide By Zero If The Window Is Too Small.
    }
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, NewWidth, NewHeight);
	gluOrtho2D(0, 300, 300, 0);
}

void displayTest(void)
{
    glClearColor(0.0f,0.0f,0.0f,0.0f); // Clear to black.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the colour buffer.
        glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);
            drawRect(one.posX, one.posY, one.sizeX, one.sizeY);
        glPopMatrix();
        glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);
            drawRect(two.posX, two.posY, two.sizeX, two.sizeY);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.0f, 1.0f, 0.0f);
            drawRect(three.posX, three.posY, three.sizeX, three.sizeY);
            glColor3f(0.0f, 0.0f, 0.0f);
        glPopMatrix();
    glutSwapBuffers();
}

#endif
