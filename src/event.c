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

typ_position *adjustment(typ_position *test, typ_map *map)
{
    while(collision(test, map))
    {
        test->posY-=0.1;
    }
    test->posY+=0.1;
    return test;
}

void bananaCollision(typ_character *player, typ_map *map, typ_game *game)
{
    int minX = (player->posChar->posX + player->posChar->sizeX/2) / Square_size;
    int minY = (player->posChar->posY + player->posChar->sizeY/2) / Square_size;
    int maxX = Square_size;
    int maxY = Square_size;

    if(map->banana[minX][minY]==1)
    {
        if((minX + maxX) < player->posChar->posX
        || (player->posChar->posX + player->posChar->sizeX) < minX
        || (minY + maxY) < player->posChar->posY
        || (player->posChar->posY + player->posChar->sizeY) < minY)
        {
            map->banana[minX][minY]=0;
            game->score++;
        }
    }
    return;
}

void snakeCollision(typ_character *player, typ_character *ennemy)
{
    int res=CheckCollision(player->posChar, ennemy->posChar);
    if(res==1)
    {
        player->live--;
    }
    return;
}

void updatePosition(typ_character *player, typ_character *ennemy, typ_map *map, typ_decor *decor, typ_game *game)
{
    if(collision(player->posChar,map))
    {
        if(player->isJumping==true)
        {
            player->posChar=adjustment(player->posChar,map);
            player->isJumping=false;
            player->action=NONE;
        }
    }
    else if(isColliding(player->posChar, decor->spiderWeb))
    {
        if(player->isJumping==true)
        {
            player->isJumping=false;
            player->action=DOWN;
        }
    }
    else
    {
        player->action=DOWN;
        player->v_y += v_grav; // incrementation of the speed.
        player->posChar->posY += player->v_y; // incrementation of the position.
    }
    bananaCollision(player, map, game);
    snakeCollision(player, ennemy);
}

int collision(typ_position* test, typ_map* map)
{
    int collision_down/*, collision_up, collision_left, collision_right*/;
    int i=0, j=0, tileIndice;
    int minX, maxX, minY, maxY;
  	minX = test->posX / Square_size;
  	minY = test->posY / Square_size;
  	maxX = (test->posX + test->sizeX -1) / Square_size;
  	maxY = (test->posY + test->sizeY -1) / Square_size;

if(test->sizeY + test->posY < map->height * Square_size - 10)
{
    // If we hit a block below.
    j = (test->posY + test->sizeY) / Square_size;
    for(i=minX;i<=maxX;i++)
    {
        tileIndice=map->matrice[i][j];
        collision_down=(map->tiles[tileIndice].solid);
    }
    // If we hit a block to the left.
    i=(test->posX -1) / Square_size;
    for(j=minY;j<=maxY;j++)
    {
        tileIndice = map->matrice[i][j];
        //collision_left=(map->tiles[tileIndice].solid);
        j=maxY+1;
    }
    // If we hit a block to the right.
    i=(test->posX + test->sizeX) / Square_size;
    for(j=minY;j<=maxY;j++)
    {
        tileIndice = map->matrice[i][j];
        //collision_right=(map->tiles[tileIndice].solid);
        j=maxY+1;
    }
    // If we hit a block above.
    j = (test->posY - 1) / Square_size;
    for(i=minX;i<=maxX;i++) //Si il y a un bloc au dessus
    {
        tileIndice = map->matrice[i][j];
        //collision_up=(map->tiles[tileIndice].solid);
    }
}
return collision_down /*|| collision_left || collision_right || collision_up*/;
}

void updateLimits(typ_character* player, typ_map* map)
{
    // bord gauche
    if (player->posChar->posX < Square_size) //Si on touche le bord gauche de la map
    {
        player->posChar->posX = Square_size + 1;
    }
    // bord droit
    if (player->posChar->posX >= map->width*Square_size - player->posChar->sizeX - Square_size)
    {
        player->posChar->posX = map->width*Square_size - player->posChar->sizeX - Square_size;
    }
    // bord haut
    if (player->posChar->posY<0)
    {
        player->isJumping=false;
        player->posChar->posY+=10;
        player->v_y+=10;
    }
/*
    if(player->action==JUMP)
    {
        if(player->action==JUMP)
        {
            if(player->jumptime<=1)
            {
                playAudio(4,1);
            }
            player->jumptime+=10;
            player->posChar->posY-=10;
            if(player->jumptime>1000)
            {
//Ne pas sautez tout le temps si on laisse la touche haut activée
            }
        }
    }
*/
                //winDisplay();
                //loseDisplay();
}

void updateSnake(typ_character *ennemy, typ_character *player, typ_map *map)
{
    int res=CheckCollision(player->posChar, ennemy->posChar);
    if(res==1)
    {
        if(ennemy->action==RIGHT)
        {
            ennemy->posChar->posX-=15;
            ennemy->action=LEFT;
        }
        else if(ennemy->action==LEFT)
        {
            ennemy->posChar->posX+=15;
            ennemy->action=RIGHT;
        }
    }
    else
    {
        // testing limits.
        if(ennemy->action==RIGHT)
        {
          if(ennemy->posChar->posX < map->width * Square_size - ennemy->posChar->sizeX - 1) // don't reach the border right side.
              ennemy->posChar->posX+=1;
          else
              ennemy->action=LEFT;
        }
        if(ennemy->action==LEFT)
        {
          if(ennemy->posChar->posX > 0)
              ennemy->posChar->posX-=1;
          else
            ennemy->action=RIGHT;
        }
    }

    // Ground collision else gravity.
    int minX=(ennemy->posChar->posX + ennemy->posChar->sizeX/2) / Square_size;
    int minY=(ennemy->posChar->posY + ennemy->posChar->sizeY) / Square_size;
    if(map->matrice[minX][minY]==1)
    {
        adjustment(ennemy->posChar, map);
    }
    else
    {
        // Gravity.
        ennemy->v_y += v_grav;
        ennemy->posChar->posY += ennemy->v_y;
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
    double v_gravity = 0.08;
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
        v_y += v_gravity; /* evolution de la vitesse */
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
