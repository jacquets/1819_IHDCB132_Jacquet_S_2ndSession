/**
 *      Filename : game.c
 *      Description:  game functions.
 *      Created:  06-07-19 | Revision: --.
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
#include <stdbool.h>
#include <time.h>
//#include <limits.h>
//#include <errno.h>

#include "game.h"
#include "draw.h"
#include "map.h"
#include "render.h"
#include "write.h"
#include "audio.h"
#include "score.h"
#include "timeTM.h"
#include "event.h"

// ----------------------------------------------------
//  global variables.
// ----------------------------------------------------
GLuint window, window2;

typ_state state=MENU; // to init game state.
typ_state selectedMenu=GAME;
typ_action action=RIGHT; // to init game menu.

typ_data *scoreList=NULL; // list to manage Game Data.
typ_map *m=NULL;
typ_game *g=NULL;
typ_character *c=NULL;
typ_character *e=NULL;
typ_object *o=NULL;
typ_decor *d=NULL;
GLboolean lauded=0;
GLboolean cleaned=0;
GLboolean pause=0;

typ_diaporama diaporama={.slide=0,.scroll=0};


// ----------------------------------------------------
//  Initialisation.
// ----------------------------------------------------

// Setup OpenGL to use our image.
bool Setup()
{
	glEnable(GL_TEXTURE_2D); // Enable texturing.
	glMatrixMode(GL_PROJECTION);  // We want to use the projection matrix.
	glLoadIdentity();  // Loads the identity matrix into the current matrix.
  #ifdef _WIN32
	ilutRenderer(ILUT_OPENGL); // Lets ILUT know to use its OpenGL functions.
  #endif
	//glOrtho(0, MaxX*Square_size, MaxY*Square_size, 0, 1, -1);  // Set up an orthographic projection with OpenGL.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    return GL_TRUE;
}

// Standard glut resize function.
void vReshape(int NewWidth,int NewHeight)
{
	if (NewHeight==0)
    	NewHeight=1; // Prevent A Divide By Zero If The Window Is Too Small.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, NewWidth, NewHeight); // Reset The Current Viewport And Perspective Transformation.
	gluOrtho2D(0, MaxX*Square_size, MaxY*Square_size, 0); // orthographic view (objects appear the same size, no matter the distance).
	//glOrtho(0, MaxX*Square_size, MaxY*Square_size, 0, -1, 1);  // Set up an orthographic projection with OpenGL.
    //gluOrtho2D(0, width, 0, height);
}

// ----------------------------------------------------
//  game and structure allocation.
// ----------------------------------------------------

typ_game *createGame(char *playerName)
{
    typ_game *g;
    int length=strlen(playerName);
    //printf("str game : %d octets\n", sizeof(typ_game));
    g=malloc(sizeof(typ_game)); // sizeof(*typ_game) = 8 bits.
    if(g==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    g->username=malloc(sizeof(*g->username)*length+1); // sizeof(char)*length //1 bit * length.
    if(g->username==NULL){
        free(g);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    strcpy(g->username,playerName);
    g->score=0;

    return g;
}
typ_character *createCharacter(int a)
{
    typ_character *c; // pointer initialised to NULL.
    c=malloc(sizeof(typ_character));
    if(c==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    //memset((c), 0, sizeof(typ_character));

    c->posChar=malloc(sizeof(*c->posChar)); // or sizeof(typ_position) = 12 bits.
    if(c->posChar==NULL){
        free(c);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    //memset((c->posChar), 0, sizeof(*c->posChar));

    return c;
}
typ_object *createObject(void)
{
    typ_object *o;
    o=malloc(sizeof(typ_object));
    if(o==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    for(int i=0;i<objectNB;i++)
    {
        m->matrice[i]=malloc(sizeof(int));
    }

    return 0;
}
typ_character *createEnnemy(void)
{
    typ_character *e; // pointer initialised to NULL.
    e=malloc(sizeof(typ_character));
    if(e==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    e->posChar=malloc(sizeof(*e->posChar)); // or sizeof(typ_position) = 12 bits.
    if(e->posChar==NULL){
        free(e);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    return e;
}

typ_decor *createDecor(void)
{
    typ_decor *d;
    d=malloc(sizeof(typ_decor));
    if(e==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    d->spiderWeb=malloc(sizeof(*e->posChar));
    if(e->posChar==NULL){
        free(d);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    return d;
}

// ----------------------------------------------------
//  Game initialisation.
// ----------------------------------------------------

bool ok=0;

void InitGame(void)
{
    texID=loadTex(); // load texture.
    char *level = "../../data/matrice/niveau6.lvl"; // new map.
    m=loadMap(level); //load the map.

    scoreList=readData(SCORES);
    printList(scoreList);

    initBanana(m);
    //printMap(m); // to display location of some bananas.

    g=createGame("Monkey"); // new game.
    c=createCharacter(5);
    //o=createObject();
    e=createEnnemy();
    d=createDecor();
    //printf("Bonjour %s, votre score est actuellement de %d.",g->username, g->score);

    g->score=0;
    // init player.
    c->live=10;
    c->posChar->sizeX=15;
    c->posChar->sizeY=20;
    c->posChar->posX=WIDTH/2;
    c->posChar->posY=m->height*Square_size - c->posChar->sizeX - 2*Square_size - 15;
    //c->posChar->posY=m->height - 2*Square_size - c->posChar->sizeY;
    c->texSelected=1;
    c->textureDelay=0;
    c->keyTextureDelay=0;
    c->action=LEFT;
    c->control=CHARACTER;
    /*
    // init object.
    for(int i=1; i<=o->objNB; i++){
        o->posObj[i].w=Square_size;
        o->posObj[i].h=Square_size;
        o->posObj[i].posX=o->posObj[i].posX - m->xscroll;
        o->posObj[i].posX=o->posObj[i].posY - m->yscroll;
    }
    */
    // init ennemy.
    e->posChar->sizeX=30;
    e->posChar->sizeY=30;
    e->posChar->posX=WIDTH/2 - 20;
    //e->posChar->posY=m->height - c->posChar->sizeX - 2*Square_size;
    e->posChar->posY=HEIGHT/2;
    e->texSelected=10;
    e->textureDelay=0;
    e->action=LEFT;
    e->live=10;

    c->v_x=v_air;
    c->v_y=v_jump;
    c->isJumping=false;

    d->spiderWeb->posX=20;
    d->spiderWeb->posY=150;
    d->spiderWeb->sizeX=100;
    d->spiderWeb->sizeY=100;
}

// ----------------------------------------------------
//  Display the game.
// ----------------------------------------------------

void vDisplay(void)
{
    //glClearColor(0.0f,0.0f,0.0f,0.0f); // Clear to black.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the colour buffer.
		switch(state)
		{
			case MENU:
                menuDisplay();
                coordDisplay();
                selectedMenu==GAME?
                    drawRect(70, 70, Square_size, Square_size) :
                    drawRect(70, 70, 2, 2);
                selectedMenu==RULES?
                    drawRect(120, 120, Square_size, Square_size) :
                    drawRect(120, 120, 2, 2);
                selectedMenu==SCORE?
                    drawRect(170, 170, Square_size, Square_size) :
                    drawRect(170, 170, 2, 2);
                selectedMenu==SAVE?
                    drawRect(220, 220, Square_size, Square_size) :
                    drawRect(220, 220, 2, 2);
                selectedMenu==EXIT?
                    drawRect(270, 270, Square_size, Square_size) :
                    drawRect(270, 270, 2, 2);
                selectedMenu==DIAPORAMA?
                    drawRect(50, 270, Square_size, Square_size) :
                    drawRect(70, 270, 2, 2);
                break;

            case GAME:
                background();
                if(c->live>1 && g->score<2)
                {
                    drawMap(m);
                    drawBanana(m);
                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, texID[18]);
                            drawTrellis(d->spiderWeb->posX - m->xscroll,d->spiderWeb->posY - m->yscroll,d->spiderWeb->sizeX,d->spiderWeb->sizeY);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, 0);
                        glBindTexture(GL_TEXTURE_2D, texID[c->texSelected]);
                            drawChar(c->posChar->posX-m->xscroll, c->posChar->posY-m->yscroll, c->posChar->sizeX, c->posChar->sizeY, c->action);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, texID[e->texSelected]); // snake.
                            drawChar(e->posChar->posX-m->xscroll, e->posChar->posY-m->yscroll, e->posChar->sizeX, e->posChar->sizeY, e->action);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, 0);
                            drawGame(0.0, HEIGHT-Square_size, WIDTH, Square_size, g->score);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();
                    glPushMatrix();
                    int i=10;
                    while(i>=10-c->live)
                    {
                        glBindTexture(GL_TEXTURE_2D, texID[21]);
                            drawRect(WIDTH-i*Square_size-2*Square_size, HEIGHT-0.8*Square_size, 73/10, 66/10);
                        glBindTexture(GL_TEXTURE_2D, 0);
                        i--;
                    }
                    glPopMatrix();
                }
                else if(g->score==2)
                {
                    //freeAll(); // freeing all global variables.
                    glPushMatrix();
                      winDisplay();
                    glPopMatrix();
                }
                else if(c->live==1)
                {
                    glPushMatrix();
                      loseDisplay();
                    glPopMatrix();
                }
				break;
			case RULES:
          rulesDisplay();
          break;
      case SCORE:
          scoreDisplay(scoreList);
          break;
      case SAVE:
          saveDisplay(scoreList);
          break;
      case EXIT:
          //printList(scoreList);
          keyPressed(ESCAPE,0,0);
          break;
      case DIAPORAMA:
          diaporamaDisplay(diaporama);
          break;
			default :
          printf("Error : no case.");
          break;
		}
    //glFlush();
    glBindTexture(GL_TEXTURE_2D,0);
    glutSwapBuffers();  // We use double buffering, so swap the buffers.
    /*

    if(c->live==0)
    {
        saveSortedListGamePlays(g);
        saveGamePlay(g);
        printf("Game saved.\n");
        loseDisplay();
        //freeAll(); // freeing all global variables.
        //changeGameState(EXIT);
    }
    if(g->score==5)
    {
        saveSortedListGamePlays(g);
        saveGamePlay(g);
        printf("Game saved.\n");
        winDisplay();
        //freeAll(); // freeing all global variables.
        //changeGameState(EXIT);
    }
    */
}

void vDisplay2(void)
{
    //glClearColor(0.0f,0.0f,0.0f,0.0f); // Clear to black.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the colour buffer.
    char *buffer[bufferNB];
    int i;
    switch(state)
		{
			case MENU:
                break;
            case GAME:
                for(i=0;i<bufferNB;i++)
                {
                    buffer[i]=malloc(50*sizeof(char*));
                    if(buffer[i]==NULL){
                        fprintf(stderr, "Error : dynamic allocation problem.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                glColor3f(0.5f, 0.5f, 0.5f);
                drawRect(0.0, 0.0, WIDTH2, HEIGHT2);
                glColor3f(1.0f, 1.0f, 1.0f);

                sprintf(buffer[0], "SCORE: %d", g->score);
                sprintf(buffer[1], "pos X: %f", c->posChar->posX);
                sprintf(buffer[2], "pos Y: %f", c->posChar->posX);
                sprintf(buffer[3], "size X: %f", c->posChar->sizeX);
                sprintf(buffer[4], "size Y: %f", c->posChar->sizeY);
                sprintf(buffer[5], "Collision: %d", c->isColliding);

                vBitmapOutput(10,40,"JUMPING BANANA",GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,70, buffer[0],GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,100, buffer[1],GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,130, buffer[2],GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,160, buffer[3],GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,190, buffer[4],GLUT_BITMAP_HELVETICA_12);
                vBitmapOutput(10,220, buffer[5],GLUT_BITMAP_HELVETICA_12);
                break;
            case RULES:
                break;
            case SCORE:
                break;
            case SAVE:
                break;
            default:
                break;
        }
    glutSwapBuffers();  // We use double buffering, so swap the buffers.
}

// ----------------------------------------------------
//  Save game.
// ----------------------------------------------------

typ_data *readData(char* filename)
{
    typ_data *L=newlist();
    FILE *F = NULL;
    F = fopen(filename, "r");
    if(F==NULL){
        //fprintf(stderr, "Error : %s\n",strerror(errno));
        perror("Error ");
        exit(EXIT_FAILURE);
    }
    char string[BUFFER];
    int tabInt[6]={0};
    char username[BUFFER];
    int score=0;
    int res=0;
    while(!feof(F))
    {
        fgets(string, sizeof(string),F);
        res=sscanf(string,"%d;%d;%d;%d;%d;%d;%[^;];%d\n",&tabInt[0],&tabInt[1],&tabInt[2],
        &tabInt[3],&tabInt[4],&tabInt[5],username,&score);
        if(res!=-1)
        {
            insertSorted(&L, tabInt, username, score);
            //printf("sscanf: %d, fgets: %s\n",res,string);
            memset(string, 0, sizeof(*string) * BUFFER);
        }
    };
    fclose(F);
    //printList(L);
    return L;
}

void writeData(char* filename, typ_data *lptr)
{
    FILE *F = fopen(filename,"w");
    if(F==NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }
    while (lptr != NULL) {
        fprintf(F, "%d;%d;%d;%d;%d;%d;%s;%d\n",lptr->data->time[0],lptr->data->time[1],lptr->data->time[2],
        lptr->data->time[3],lptr->data->time[4],lptr->data->time[5],lptr->data->username,lptr->data->score);
		    lptr = lptr->nextptr;
    }
    fclose(F);
}

void saveGamePlay(typ_game *g)
{
    typ_data *lptr=newlist();
    char filename[40]; // we create a new file.
    int now[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};

    lptr=readData(filename);
    insertSorted(&lptr, now, g->username, g->score);
    sprintf(filename,"../../data/score/%d-%d-%d_score.csv",now[0],now[1],now[2]);
    writeData(filename, lptr);
    //printList(L);  // Simple verification.
    writeData(BACKUP1, lptr);
}

void saveGame(char* filename, typ_game *g, typ_data *lptr)
{
    int now[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};
    insertSorted(&lptr, now, g->username, g->score);
    writeData(BACKUP2, lptr);
    //printList(lptr);  // Simple verification.
}

// ----------------------------------------------------
//  Key management.
// ----------------------------------------------------

void changeGameState(typ_state newState)
{
    state = newState;
    if (state == GAME)
    {
        //game();
    }
    //stopAudio();
}

void keyPressed(unsigned char key, int x, int y)
{
//usleep(100); // avoid thrashing this procedure. If escape is pressed, kill everything.
  switch (key)
  {
    case ESCAPE: // kill everything.
        //deinitialize();
        /*
        changeGameState(EXIT);
        saveSortedListGamePlays(g);
        saveGamePlay(g);
        printf("Game saved.\n");
        */
        //freeAll(); // freeing all global variables.
        exit(1); // exit the program...normal termination.
        break;
    case 'p' :
    case 'P' :
        if(pause==1)
            pause=0;
        else if(pause==0)
            pause=1;
        break;
    default:
        break;
  }
}

void specialKeyPressed(int key, int x, int y)
{
    switch (state)
	  {
        case MENU:
          switch (key)
	        {
                case GLUT_KEY_DOWN:
                    if (selectedMenu<menuOptionNB)
                        selectedMenu++;
                    else
                        selectedMenu=menuOptionNB;
                    break;
                case GLUT_KEY_UP:
                    if (selectedMenu<1) //  always return to 1 (game).
                        selectedMenu--;
                    else
                        selectedMenu=1;
                    break;
                case GLUT_KEY_RIGHT:
                    changeGameState(selectedMenu);
                    break;
                case GLUT_KEY_LEFT:
                    changeGameState(state);
                    break;
                default:
                    break;
          }
          break;
        case GAME:
            c->control=CHARACTER;
            switch (key)
	          {
                case GLUT_KEY_DOWN:
                    c->action=DOWN;
                    c->posChar->posY++;
                    break;
                case GLUT_KEY_UP:
                    if(!c->isJumping) // jump only once time.
                    {
                        c->action=UP;
                        c->isJumping=true;
                        c->posChar->posY--;
                        c->v_y = v_jump;
                    }
                    break;
                case GLUT_KEY_RIGHT:
                    c->action=RIGHT;
                    c->posChar->posX++;
                    c->keyTextureDelay=0; // changes enabled while 30ms aren't flying.
                    if(c->isJumping==true)
                        c->posChar->posX += c->v_x; // Direction while the jump.
                    break;
                case GLUT_KEY_LEFT:
                    c->action=LEFT;
                    c->posChar->posX--;
                    c->keyTextureDelay=0;
                    if(c->isJumping==true)
                        c->posChar->posX-=c->v_x; // Direction while the jump.
                    break;
                default:
                    //c->action=NONE;
                    break;
            }
            break;
        case RULES:
            break;
        case SCORE:
            break;
        case SAVE:
            break;
        case EXIT:
            //freeAll();
            keyPressed(ESCAPE,0,0);
            break;
        case DIAPORAMA:
            switch(key)
            {
                case GLUT_KEY_RIGHT:
                diaporama.slide++;
                break;
                case GLUT_KEY_LEFT:
                diaporama.slide--;
                break;
                case GLUT_KEY_UP:
                diaporama.scroll++;
                break;
                case GLUT_KEY_DOWN:
                diaporama.scroll--;
                break;
                default:
                    break;
            }
            break;
        default:
            printf("Key %d pressed. No action there yet.",key);
            break;
    }
}

// ----------------------------------------------------
//  Updating the game.
// ----------------------------------------------------


void updateCharacter()
{
    //int temp;
    // testing limits.

    // Going to the other side (c.f. PacMan).
    if(c->posChar->posX<0){
        c->posChar->posX=WIDTH;
    }
    if(c->posChar->posX>WIDTH){
        c->posChar->posX=0;
    }
    if(c->posChar->posY>HEIGHT-Square_size){
        c->posChar->posY=HEIGHT-Square_size;
    }
    if(c->posChar->posY<0){
        c->posChar->posY=0;
    }
}

void updateScroll(void)
{
    m->xscroll = c->posChar->posX + c->posChar->sizeX/2 - WIDTH/2;
    m->yscroll = c->posChar->posY + c->posChar->sizeY/2 - HEIGHT/2;

    if(m->xscroll<0){
        m->xscroll=0;
    }
    if(m->yscroll<0){
        m->yscroll=0;
    }
	  if(m->xscroll>m->width*Square_size-WIDTH-1)
        m->xscroll=m->width*Square_size-WIDTH-1;
	  if(m->yscroll>m->height*Square_size-HEIGHT-1)
        m->yscroll=m->height*Square_size-HEIGHT-1;
}

void updateRender(void)
{
    if(c->textureDelay>20)
    {
        c->textureDelay=0;
    }
    if(c->textureDelay==5 && c->isJumping==0) // every 5 mili-seconds.
    {
        c->texSelected++;
        if(c->texSelected==10)
        {
            c->texSelected=2; // Texture indice for the monkey.
        }
        c->textureDelay=0;
    }
    if(e->textureDelay==5)
    {
        e->texSelected++;
        if(e->texSelected==13)
        {
            e->texSelected=10; // Texture indice for the monkey.
        }
        e->textureDelay=0;
    }

    e->textureDelay++;

    if(c->keyTextureDelay>10)
    {
        c->texSelected=1; // sitting position.
        c->keyTextureDelay=31; // delay enabled.
    }
    else
    {
        c->textureDelay++;
        c->keyTextureDelay++;
    }

    if(c->isJumping==true && c->v_y<0)
    {
      c->texSelected=19; // jumping texture.
    }
    else if(c->isJumping==true && c->v_y>=0)
    {
      c->texSelected=20; // falling texture.
    }
}


// ----------------------------------------------------
//  Timer.
// ----------------------------------------------------

/* Timer function to handle update of the game screen. */
void Loop()
{
    if(!pause){
        updateScroll();
        updateSnake(e,c,m);
        updateRender();
        updatePosition(c,e,m,d,g);
        updateLimits(c,m);
    }
}

/* Pre : integer.
 * Post : screen redisplay every x ms.
 */
void timer(int value)
{
    switch (state)
    {
        case MENU:
            break;
        case GAME:
            Loop();
            break;
        case RULES:
        case SCORE:
        case SAVE:
        case EXIT:
            break;
        case DIAPORAMA:
            if(diaporama.slide<0)
                diaporama.slide=0;
            if(diaporama.slide>40)
                diaporama.slide=40;
            if(diaporama.scroll<0)
                diaporama.scroll=0;
            if(diaporama.scroll>40)
                diaporama.scroll=40;
            break;
        default:
            break;
    }
    glutPostRedisplay();
    //glutPostWindowRedisplay(window);
    //glutPostWindowRedisplay(window2);
    glutTimerFunc(TIMER, timer, 0);
}

// ----------------------------------------------------
//  free game.
// ----------------------------------------------------

void freeTextureGame(GLuint *texID)
{
    if(texID){
        glDeleteTextures(texNB, texID);
        //texID=0;
    }
}

void freeGame(typ_game *g)
{
    if(g){
        free(g);
        //g=0;
    }
}

void freeCharacter(typ_character *c)
{
    if(c){
        free(c);
        //c=0;
    }
    if(e){
        free(e);
        //e=0;
    }
}

void freeDecor(typ_decor *d)
{
    if(d){
        free(d);
        //g=0;
    }
}

void freeAll(void)
{
    freeCharacter(c);
    freeDecor(d);
    freeGame(g);
    // delete textures.
    if (!cleaned){
        freeTextureGame(texID);
		freeMap(m);
    }
	cleaned=GL_TRUE;  // Want to make sure we only delete it once.
    // delete audio system.

    glutDestroyWindow(window);
	glutDestroyWindow(window2);
}

// ----------------------------------------------------
//  main.
// ----------------------------------------------------

#ifdef _SAVEDATA
int main(int argc, char *argv[])
{
    // new game.
    //typ_game *g=createGame("Monkey");
    // new list.
    struct str_data *LI=newlist();
    //int now[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};
    // to insert game into a new list.
    //insertSorted(&L, now, g->username, g->score);
    LI=readData("../../data/score/score.csv");
    writeData("../../data/score/score.csv", LI);

    printList(LI);

    return 0;
}
#endif
