/*
 * graphic.h
 *
 *  Created on: Mar 9, 2023
 *      Author: vmuser
 */



#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include <unistd.h>
#include "lib/gles.h"  // struct opengles, gles*-Funktionen
#include "lib/tile.h"  // struct tile, loadPngTile


void* graphic_main(void*);


#endif /* GRAPHIC_H_ */
