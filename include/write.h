/**
 *      Filename : write.h
 *      Description: writing functions.
 *      Created:  20-07-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _WRITE_H_
#define _WRITE_H_

/* Definition.*/
//-

/* Prototypes. */
void vBitmapOutput(int x, int y, char *string, void *font);
void vStrokeOutput(GLfloat x, GLfloat y, char *string, void *font);

#endif
