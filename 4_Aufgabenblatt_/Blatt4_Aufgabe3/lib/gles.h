// --------------------------------------------------------------------------
//  Copyright © 2014  Christian Steiger  -  All rights reserved
// --------------------------------------------------------------------------

// Sicherstellen, dass die Header-Datei nur einmal eingebunden wird.
#ifndef GLES_H_
#define GLES_H_ 1

#include <EGL/egl.h>   // EGL Datentypen und Funktionen
//#include <GLES/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen  	Uncomment/Comment for Target
#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include <X11/Xlib.h>	// 												Uncomment/Comment for Host

struct opengles
{
	// EGL
	EGLDisplay  display;
	EGLSurface  surface;
	EGLContext  context;

	// Display- und Windowhandler des übergeordneten Frameworks, z.B. X11
	EGLNativeDisplayType native_display;
	EGLNativeWindowType  native_window;

	// Dimension des Framebuffers (fbdev) oder Fensters (X11)
	uint16_t  native_width;
	uint16_t  native_height;

	// Dimensionen der OpenGL ES Oberfläche
	uint16_t  width;
	uint16_t  height;
	float     ratio;
};

// Funktionsprototyp für can_open-Funktion.
int glesInitialize(struct opengles *opengles);
int glesDraw(struct opengles *opengles);
int glesRun(struct opengles *gles);
int glesDestroy(struct opengles *opengles);

#endif
