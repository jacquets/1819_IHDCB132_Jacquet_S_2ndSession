/**
 *      Filename : audio.c
 *      Description:  audio functions.
 *      Created:  21-07-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

// https://github.com/kengonakajima/moyai/blob/master/fmod/examples/submixing/main.c
// linus :
// https://wiki.debian.org/fr/FMOD
#ifdef _WIN32
#include <windows.h>
#include <GL/freeglut.h>
#include <GL/GL.h>
#elif __linux__
#include <GL/glut.h> /* GLUT, includes glu.h and gl.h */
#else
#error "OS not supported!"
#endif

#include <stdlib.h>
#include <stdio.h>

#include "audio.h"

#ifdef _AUDIO_ENABLED

#include <FMOD/fmod.h>
#include <FMOD/fmod_errors.h>
#include <string.h>

// Declaractions.
FMOD_SYSTEM *sys;
FMOD_SOUND *sound[MAX_SOUND];
FMOD_CHANNEL *channel_sound[MAX_SOUND];
FMOD_RESULT err;

void FMOD_Check(FMOD_RESULT result)
{
    if (result != FMOD_OK) // FMOD_OK=0.
    {
        fprintf(stderr, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(EXIT_FAILURE);
    }
}

void InitAudio()
{
    // creating and initializating of an object.
    FMOD_System_Create(&sys);
    FMOD_System_Init(sys,32,FMOD_INIT_NORMAL,NULL);
    // sound loading and verification.
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_00,FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL,0,&sound[0]); FMOD_Check(err);
    FMOD_Sound_SetLoopCount(sound[0], -1); // infinitely retetition.
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_01,FMOD_CREATESAMPLE,0,&sound[1]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_02,FMOD_CREATESAMPLE,0,&sound[2]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_03,FMOD_CREATESAMPLE,0,&sound[3]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_04,FMOD_CREATESAMPLE,0,&sound[4]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_05,FMOD_CREATESAMPLE,0,&sound[5]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_06,FMOD_CREATESAMPLE,0,&sound[6]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_07,FMOD_CREATESAMPLE,0,&sound[7]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_08,FMOD_CREATESAMPLE,0,&sound[8]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_09,FMOD_CREATESAMPLE,0,&sound[9]); FMOD_Check(err);
    err = FMOD_System_CreateSound(sys,SOUND_FILE_PATH_10,FMOD_CREATESAMPLE,0,&sound[10]); FMOD_Check(err);
}

void playAudio(unsigned int SoundID, FMOD_BOOL isPlaying)
{
    err = FMOD_System_PlaySound(sys,sound[SoundID],0,0,&channel_sound[SoundID]); FMOD_Check(err);
    // playing/displaying the sound.
    err = FMOD_Channel_IsPlaying(channel_sound[SoundID],&isPlaying); FMOD_Check(err);
}

// freeing of the sounds and freeing of a 'system' object.
void stopAudio()
{
    int n = 0;
    while(n<=MAX_SOUND)
    {
        err = FMOD_Sound_Release(sound[n]); FMOD_Check(err);
        n++;
    }
    err = FMOD_System_Close(sys); FMOD_Check(err);
    err = FMOD_System_Release(sys); FMOD_Check(err);
}

// Print the version.
void FMOD_Version()
{
    FMOD_SYSTEM *sys;
    FMOD_RESULT err;
    unsigned int version;
    err = FMOD_System_Create(&sys); FMOD_Check(err);
    err = FMOD_System_GetVersion(sys, &version); FMOD_Check(err);
    printf("Welcome on FMOD version %d !\n\n",FMOD_VERSION%4);
    /* FMOD_VERSION : 0xaaaabbcc -> aaaa = product version, bb = major version, cc = minor version.*/
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    }
}
#endif // audio enabled.


#ifdef _FMOD
void display(void){
  //...
  glPushMatrix();
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);glVertex2f(0.0f,   0.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex2f(128.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex2f(128.0f, 128.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex2f(0.0f,   128.0f);
  glEnd();
  glPopMatrix();
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
  glutCreateWindow("TEST"); // Create window with the given title.
  glutDisplayFunc(display); // Register callback handler for window re-paint event.
  glutReshapeFunc(reshape);

    InitAudio();
    playAudio(2, 1);
    FMOD_Version();
    //FMOD_Sound_SetLoopCount(sound[1], -1);
    //stopAudio();

  glutMainLoop();
    return 0;
}
#endif // demo fmod.
