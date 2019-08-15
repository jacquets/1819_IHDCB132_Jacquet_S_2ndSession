/**
 *      Filename : render.h
 *      Description: texture functions.
 *      Created:  19-07-19 | Revision: 29-07-19.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _RENDER_H_
#define _RENDER_H_

/* Définitions. */
//-

/* Prototypes. */
void HandleDevILErrors(char *filename);
void InitImage(void);
GLuint ImageLoad(char *filename);
void freeTexture(GLuint TexID);

#endif
