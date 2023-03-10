/*
 * hmi.h
 *
 *  Created on: Mar 9, 2023
 *      Author: vmuser
 */

#ifndef HMI_H_
#define HMI_H_

#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include <unistd.h>
#include "lib/gles.h"  // struct opengles, gles*-Funktionen
#include "lib/tile.h"  // struct tile, loadPngTile





GLfloat kmh2deg(GLfloat kmh);



GLfloat rpm2deg(GLfloat rpm);

void* logic_main(void*);




#endif /* HMI_H_ */
