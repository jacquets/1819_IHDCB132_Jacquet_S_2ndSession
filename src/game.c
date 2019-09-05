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
typ_map *map=NULL;
typ_game *game=NULL;
typ_character *player=NULL;
typ_character *ennemy=NULL;
typ_object *object=NULL;
typ_decor *decor=NULL;
GLboolean lauded=0;
GLboolean cleaned=0;
GLboolean pause=0;
int soundDelay=0;
int soundIndice=6;

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
    typ_game *game;
    int length=strlen(playerName);
    //printf("str game : %d octets\n", sizeof(typ_game));
    game=malloc(sizeof(typ_game)); // sizeof(*typ_game) = 8 bits.
    if(game==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    game->username=malloc(sizeof(*game->username)*length+1); // sizeof(char)*length //1 bit * length.
    if(game->username==NULL){
        free(game);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    strcpy(game->username,playerName);
    game->score=0;

    return game;
}
typ_character *createCharacter(int a)
{
    typ_character *player; // pointer initialised to NULL.
    player=malloc(sizeof(typ_character));
    if(player==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    //memset((c), 0, sizeof(typ_character));

    player->posChar=malloc(sizeof(*player->posChar)); // or sizeof(typ_position) = 12 bits.
    if(player->posChar==NULL){
        free(player);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    //memset((player->posChar), 0, sizeof(*player->posChar));

    return player;
}
typ_object *createObject(void)
{
    typ_object *object;
    object=malloc(sizeof(typ_object));
    if(object==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    for(int i=0;i<objectNB;i++)
    {
        map->matrice[i]=malloc(sizeof(int));
    }

    return 0;
}
typ_character *createEnnemy(void)
{
    typ_character *ennemy; // pointer initialised to NULL.
    ennemy=malloc(sizeof(typ_character));
    if(ennemy==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    ennemy->posChar=malloc(sizeof(*ennemy->posChar)); // or sizeof(typ_position) = 12 bits.
    if(ennemy->posChar==NULL){
        free(ennemy);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    return ennemy;
}

typ_decor *createDecor(void)
{
    typ_decor *decor;
    decor=malloc(sizeof(typ_decor));
    if(ennemy==NULL){
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    decor->spiderWeb=malloc(sizeof(*ennemy->posChar));
    if(ennemy->posChar==NULL){
        free(decor);
        fprintf(stderr, "Error : dynamic allocation problem.\n");
      	exit(EXIT_FAILURE);
    }
    return decor;
}

// ----------------------------------------------------
//  Game initialisation.
// ----------------------------------------------------

bool ok=0;

void InitGame(void)
{
    texID=loadTex(); // load texture.
    char *level = "../../data/matrice/niveau6.lvl"; // new map.
    map=loadMap(level); //load the map.

    scoreList=readData(SCORES);
    printList(scoreList);

    initBanana(map);
    //printMap(m); // to display location of some bananas.

    game=createGame("Monkey"); // new game.
    player=createCharacter(5);
    //o=createObject();
    ennemy=createEnnemy();
    decor=createDecor();
    //printf("Bonjour %s, votre score est actuellement de %d.",game->username, game->score);

    game->score=0;
    // init player.
    player->live=10;
    player->posChar->sizeX=15;
    player->posChar->sizeY=20;
    player->posChar->posX=WIDTH/2;
    player->posChar->posY=map->height*Square_size - player->posChar->sizeX - 2*Square_size - 15;
    player->texSelected=1;
    player->textureDelay=0;
    player->keyTextureDelay=0;
    player->action=LEFT;
    player->control=CHARACTER;
    /*
    // init object.
    for(int i=1; i<=o->objNB; i++){
        o->posObj[i].w=Square_size;
        o->posObj[i].h=Square_size;
        o->posObj[i].posX=o->posObj[i].posX - map->xscroll;
        o->posObj[i].posX=o->posObj[i].posY - map->yscroll;
    }
    */
    // init ennemy.
    ennemy->posChar->sizeX=30;
    ennemy->posChar->sizeY=30;
    ennemy->posChar->posX=WIDTH/2 - 20;
    ennemy->posChar->posY=HEIGHT/2;
    ennemy->texSelected=10;
    ennemy->textureDelay=0;
    ennemy->action=LEFT;
    ennemy->live=10;

    player->v_x=v_air;
    player->v_y=v_jump;
    player->isJumping=false;

    decor->spiderWeb->posX=20;
    decor->spiderWeb->posY=150;
    decor->spiderWeb->sizeX=100;
    decor->spiderWeb->sizeY=100;
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
                if(player->live>1 && game->score<2)
                {
                    drawMap(map);
                    drawBanana(map);
                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, texID[18]);
                            drawTrellis(decor->spiderWeb->posX - map->xscroll,decor->spiderWeb->posY - map->yscroll,decor->spiderWeb->sizeX,decor->spiderWeb->sizeY);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, 0);
                        glBindTexture(GL_TEXTURE_2D, texID[player->texSelected]);
                            drawChar(player->posChar->posX-map->xscroll, player->posChar->posY-map->yscroll, player->posChar->sizeX, player->posChar->sizeY, player->action);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, texID[ennemy->texSelected]); // snake.
                            drawChar(ennemy->posChar->posX-map->xscroll, ennemy->posChar->posY-map->yscroll, ennemy->posChar->sizeX, ennemy->posChar->sizeY, ennemy->action);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();

                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, 0);
                            drawGame(0.0, HEIGHT-Square_size, WIDTH, Square_size, game->score);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    glPopMatrix();
                    glPushMatrix();
                    int i=10;
                    while(i>=10-player->live)
                    {
                        glBindTexture(GL_TEXTURE_2D, texID[21]);
                            drawRect(WIDTH-i*Square_size-2*Square_size, HEIGHT-0.8*Square_size, 73/10, 66/10);
                        glBindTexture(GL_TEXTURE_2D, 0);
                        i--;
                    }
                    glPopMatrix();
                }
                else if(game->score==2)
                {
                    glPushMatrix();
                      winDisplay();
                    glPopMatrix();
                    //saveGame("../../data/score/score.csv",game,scoreList);
                    saveGamePlay(game);
                    //sleep();
                    //freeAll(); // freeing all global variables.
                    //changeGameState(MENU);
                }
                else if(player->live==1)
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

                sprintf(buffer[0], "SCORE: %d", game->score);
                sprintf(buffer[1], "pos X: %f", player->posChar->posX);
                sprintf(buffer[2], "pos Y: %f", player->posChar->posX);
                sprintf(buffer[3], "size X: %f", player->posChar->sizeX);
                sprintf(buffer[4], "size Y: %f", player->posChar->sizeY);
                sprintf(buffer[5], "Collision: %d", player->isColliding);

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

void saveGamePlay(typ_game *game)
{
    typ_data *lptr=newlist();
    char filename[40]; // we create a new file.
    int now[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};

    lptr=readData(filename);
    insertSorted(&lptr, now, game->username, game->score);
    sprintf(filename,"../../data/score/%d-%d-%d_score.csv",now[0],now[1],now[2]);
    writeData(filename, lptr);
    //printList(L);  // Simple verification.
    writeData(BACKUP1, lptr);
}

void saveGame(char* filename, typ_game *game, typ_data *lptr)
{
    int now[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};
    insertSorted(&lptr, now, game->username, game->score);
    writeData(filename, lptr);
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
        {
            pause=0;
            for(int i=6;i<=10;i++)
            {
                playAudio(i,0);
            }
        }
        else if(pause==0)
        {
            pause=1;
        }
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
            //playAudio(0,1);
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
            player->control=CHARACTER;
            playAudio(0,0);
            switch (key)
	          {
                case GLUT_KEY_DOWN:
                    player->action=DOWN;
                    player->posChar->posY++;
                    break;
                case GLUT_KEY_UP:
                    if(!player->isJumping) // jump only once time.
                    {
                        playAudio(4,1);
                        player->action=UP;
                        player->isJumping=true;
                        player->posChar->posY--;
                        player->v_y = v_jump;
                    }
                    break;
                case GLUT_KEY_RIGHT:
                    player->action=RIGHT;
                    player->posChar->posX++;
                    player->keyTextureDelay=0; // changes enabled while 30ms aren't flying.
                    if(player->isJumping==true)
                        player->posChar->posX += player->v_x; // Direction while the jump.
                    break;
                case GLUT_KEY_LEFT:
                    player->action=LEFT;
                    player->posChar->posX--;
                    player->keyTextureDelay=0;
                    if(player->isJumping==true)
                        player->posChar->posX-=player->v_x; // Direction while the jump.
                    break;
                default:
                    //player->action=NONE;
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
    if(player->posChar->posX<0){
        player->posChar->posX=WIDTH;
    }
    if(player->posChar->posX>WIDTH){
        player->posChar->posX=0;
    }
    if(player->posChar->posY>HEIGHT-Square_size){
        player->posChar->posY=HEIGHT-Square_size;
    }
    if(player->posChar->posY<0){
        player->posChar->posY=0;
    }
}

void updateScroll(void)
{
    map->xscroll = player->posChar->posX + player->posChar->sizeX/2 - WIDTH/2;
    map->yscroll = player->posChar->posY + player->posChar->sizeY/2 - HEIGHT/2;

    if(map->xscroll<0){
        map->xscroll=0;
    }
    if(map->yscroll<0){
        map->yscroll=0;
    }
	  if(map->xscroll>map->width*Square_size-WIDTH-1)
        map->xscroll=map->width*Square_size-WIDTH-1;
	  if(map->yscroll>map->height*Square_size-HEIGHT-1)
        map->yscroll=map->height*Square_size-HEIGHT-1;
}

void updateRender(void)
{
    if(player->textureDelay>20)
    {
        player->textureDelay=0;
    }

    if(player->textureDelay==5 && player->isJumping==0) // every 5 mili-seconds.
    {
        player->texSelected++;
        if(player->texSelected==10)
        {
            player->texSelected=2; // Texture indice for the monkey.
        }
        player->textureDelay=0;
    }
    
    if(ennemy->textureDelay==5)
    {
        ennemy->texSelected++;
        if(ennemy->texSelected==13)
        {
            ennemy->texSelected=10; // Texture indice for the monkey.
        }
        ennemy->textureDelay=0;
    }

    ennemy->textureDelay++;

    if(player->keyTextureDelay>10)
    {
        player->texSelected=1; // sitting position.
        player->keyTextureDelay=31; // delay enabled.
    }
    else
    {
        player->textureDelay++;
        player->keyTextureDelay++;
    }

    if(player->isJumping==true && player->v_y<0)
    {
      player->texSelected=19; // jumping texture.
    }
    else if(player->isJumping==true && player->v_y>=0)
    {
      player->texSelected=20; // falling texture.
    }
}

void updateSound(void)
{
    if(soundDelay==1)
    {
        playAudio(soundIndice,1);
    }
    else if(soundDelay==100)
    {
        playAudio(soundIndice,0);
        soundIndice++;
    }
    
    if(soundDelay==100){
        soundDelay=0;
    }
    if(soundIndice==11){
        soundIndice=6;
    }
    soundDelay++;
}


// ----------------------------------------------------
//  Timer.
// ----------------------------------------------------

/* Timer function to handle update of the game screen. */
void Loop()
{
    if(!pause){
        updateScroll();
        updateSnake(ennemy,player,map);
        updateRender();
        updatePosition(player,ennemy,map,decor,game);
        updateLimits(player,map);
        updateSound();
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

void freeGame(typ_game *game)
{
    if(game){
        free(game);
        //g=0;
    }
}

void freeCharacter(typ_character *player,typ_character *ennemy)
{
    if(player){
        free(player);
        //char=0;
    }
    if(ennemy){
        free(ennemy);
        //ennemy=0;
    }
}

void freeDecor(typ_decor *decor)
{
    if(decor){
        free(decor);
        //game=0;
    }
}

void freeAll(void)
{
    freeCharacter(player, ennemy);
    freeDecor(decor);
    freeGame(game);
    // delete textures.
    if (!cleaned){
        freeTextureGame(texID);
		freeMap(map);
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
    //insertSorted(&L, now, game->username, game->score);
    LI=readData("../../data/score/score.csv");
    writeData("../../data/score/score.csv", LI);

    printList(LI);

    return 0;
}
#endif
