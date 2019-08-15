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

#include "map.h"
#include "render.h"
#include "audio.h"
#include "game.h"

int nF1, nFcour, nFullScreen2=0;
bool nCache=0;

GLuint secondScreen(char *windowName);
void vReshape2(int w, int h);
void vDisplay2(void);
void keyPressed2(unsigned char key, int x, int y);

int main(int argc, char *argv[])
{
	glutInit(&argc, argv); // Initialize GLUT.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Enable double buffered mode.
	
	//window2=secondScreen(argv[0]);
	glutInitWindowPosition(100, 100); // position the window's initial top-left corner.
	glutInitWindowSize(MaxX*Square_size, MaxY*Square_size); // Set the window's initial width/height - non-square.
	window=glutCreateWindow(argv[0]); // Create window with the given title.
	glutSetWindow (window);

		InitImage(); // Our own DevIL initialization.
		InitAudio();
		InitGame();
		playAudio(0,1);
	glutDisplayFunc(vDisplay); // Register callback handler for window re-paint event.
	glutIdleFunc(vDisplay);	// callback activiting.
	glutTimerFunc(TIMER, timer, 0);
	glutReshapeFunc(vReshape); // Register callback handler for window re-size event.
	glutKeyboardFunc(keyPressed); // Register the function called when the keyboard is pressed.
	glutSpecialFunc(specialKeyPressed);
	
	Setup();

	glutMainLoop(); // Enter the infinite event-processing loop.
	
	return 0;
}

GLuint secondScreen(char *windowName)
{
	GLuint window2;
	glutInitWindowSize(WIDTH2,HEIGHT2);
	/* Centred screen. */
    unsigned int Largeur_Ecran_x = glutGet(GLUT_SCREEN_WIDTH);
    unsigned int Hauteur_Ecran_Y = glutGet(GLUT_SCREEN_HEIGHT);
    unsigned int position_centree_X = (Largeur_Ecran_x-WIDTH2)/2;
    unsigned int position_centree_Y = (Hauteur_Ecran_Y-HEIGHT2)/2;
    glutInitWindowPosition(position_centree_X, position_centree_Y); /* Position d'initialisation en haut à gauche du coin de mon écran. */
	window2=glutCreateWindow(windowName);
	glutDisplayFunc(vDisplay2);
	glutIdleFunc(vDisplay2);
	glutReshapeFunc(vReshape);
	return window2;
}
void vReshape2(int w, int h)
{
	float L;
	float H;
	if (h==0)
    	h=1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w,h);
	if (w<=h)
	{
	  if (w==0) H=HEIGHT2;
	  else H=(GLfloat) (WIDTH2*h/w);
	  L=WIDTH2;
	}
	else
	{
	  H=HEIGHT2;
	  if (h==0) L=WIDTH2;
	  else L=(GLfloat) (HEIGHT2*w/h);
	}
	gluOrtho2D(-L/2,L/2,-H/2,H/2);
}

void keyPressed2(unsigned char key, int x, int y)
{
	switch (key) {

		case 'f' :
		case 'F' :
			if (nFullScreen2==0)
			{
				glutFullScreen();
				nFullScreen2=1;
				break;
			}
			if (nFullScreen2==1)
			{
				glutReshapeWindow(WIDTH2,HEIGHT2);
				glutPositionWindow(150,150);
				nFullScreen2=0;
				break;
			}

		case 'd' :
		case 'D' :
			nFcour=glutGetWindow();
			glutDestroyWindow(nFcour);
			break;

		case 'c' :
		case 'C' :
			if (nCache==0){
				glutHideWindow();
				nCache=1;
			}
			break;

		case 'm' :
		case 'M' :
			if (nCache==1){
				glutSetWindow(nF1);
				glutShowWindow();
				nCache=0;
			}
			break;

		default :
			break;
	}

}
