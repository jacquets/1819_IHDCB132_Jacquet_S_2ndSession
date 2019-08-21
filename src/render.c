/**
 *      Filename : render.c
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

void HandleDevILErrors(char *filename){
#ifdef _WIN32
  ILenum error=ilGetError();
	if(error!=IL_NO_ERROR){
		do{
      fprintf(stderr, "Error : %s.\n", iluErrorString(error));
      exit(EXIT_FAILURE); /* program left. */
		}while((error=ilGetError()));
		exit(EXIT_FAILURE);
	}
#endif
}

void InitImage(void)
{
#ifdef _WIN32
  ilInit(); // Needed to initialize DevIL.
  iluInit();
  ilutInit(); // Needed to initialize ilut here.
  ilEnable(IL_CONV_PAL); // GL cannot use palettes anyway, so convert early.
	ilutEnable(ILUT_OPENGL_CONV); // Gets rid of dithering on some nVidia-based cards.
  ilutRenderer(ILUT_OPENGL);
#endif
  //loadTexture();
}

GLuint ImageLoad(char *filename)
{
#ifdef _WIN32
  ILuint ImgID;
  ilGenImages(1,&ImgID); // Grab a new image name.
  ilBindImage(ImgID); // Bind this image name.
  // Loads the image specified by File into the ImgId.
  if (!ilLoadImage(filename)){
      HandleDevILErrors(filename); // Loading the image and check for errors
  }
  // Bind image.
  GLuint TexID=ilutGLBindTexImage();
  // Unbind image and free DevIL image memory.
  ilBindImage(0);
	ilDeleteImages(1,&ImgID); // We're done with our image, so we go ahead and delete it.
  HandleDevILErrors(filename);
#else
  GLuint TexID=SOIL_load_OGL_texture(
      filename,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_POWER_OF_TWO
      | SOIL_FLAG_MULTIPLY_ALPHA
      | SOIL_FLAG_INVERT_Y
      // |  SOIL_FLAG_COMPRESS_TO_DXT
  );
  if(TexID==0)
  {
      fprintf(stderr,"Error : %s : %s.\n",SOIL_last_result(), filename);
      exit(EXIT_FAILURE); /* program left. */
  }
#endif
  return TexID;
}

// Cleans up any loose ends.
void freeTexture(GLuint *TexID)
{
  if(TexID!=0)
    glDeleteTextures(1,TexID);  // Delete The Shader Texture.
    //SOIL_free_image_data("test.png");
}

//
//    Render compilation for local test.
//

#ifdef _RENDER
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

int main(int argc,char *argv[]){
  glutInit(&argc,argv); // Initialize GLUT.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE); // Enable double buffered mode.
  glutCreateWindow("TEST"); // Create window with the given title.
  GLuint Texture[5]={0};
#ifdef _WIN32
  InitIL(); // Our own DevIL initialization
#endif
  Texture[0]=ImageLoad("../../data/images/img_test.bmp");
  glBindTexture(GL_TEXTURE_2D,Texture[0]); // bind our texture.
  glutDisplayFunc(display); // Register callback handler for window re-paint event.
  glutReshapeFunc(reshape);
  //freeTexture(Texture);
  glEnable(GL_TEXTURE_2D);
  glutMainLoop();
  return 0;
}
#endif
