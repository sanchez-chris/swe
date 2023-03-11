// --------------------------------------------------------------------------
//  Copyright © 2014  Christian Steiger  -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>     // fprintf, stderr
#include <stdlib.h>    // exit
#include <signal.h>    // signal
#include <EGL/egl.h>   // EGL Datentypen und Funktionen
//#include <GLES/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen  	Uncomment/Comment for Target
#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include "gles.h"

// Dimensionen der Zeichenoberfläche (bzw. des Fenster)
#define VIEWPORT_WIDTH  1024
#define VIEWPORT_HEIGHT  512

// --------------------------------------------------------------------------
//   Verschiedenes
// --------------------------------------------------------------------------
// Einfache Hilfsfunktion um den Lesefluss des Programms zu verbessern.
// Ist value != 0, wird der Name der fehlgeschlagenen Funktion
// zusammen mit dem dazugehören EGL-Fehlercode ausgegeben.
// Genauere Informationen über die EGL-Fehlercodes finden sich hier:
//   www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetError.html
static inline void handleEglError(const int value, const char *name)
{
	if (value == 0) { return; }
	fprintf(stderr,	"%s fehlgeschlagen!\n", name);
	fprintf(stderr, "EGL Fehler 0x%x\n", eglGetError());
	exit(-1);
}

// Simple Hilfsfunktion um den Lesefluss zu verbessern.
// Gibt eine Fehlermeldung mit perror() aus und beendet das Programm.
inline void handlePosixError(const int condition, const char *name)
{
	if (condition == 0) { return; }
	perror(name);
	exit(-1);
}

// --------------------------------------------------------------------------
//   Verschiedenes
// --------------------------------------------------------------------------

// Globale Variable. Beendet die Endlosschleife in main falls == 0.
static volatile int gles_running = 1;

// Signalhandler.
void signal_sigint_handler(int unused  __attribute__((unused)))
{
	gles_running = 0;
}


// ==========================================================================
//   Oberflächenabhängiger Code, Xlib basierte Funktionen
// ==========================================================================
// Diesen Teil des Programms nur nutzen wenn USE_FBDEV nicht deklariert ist.
#ifndef USE_FBDEV

// --------------------------------------------------------------------------
//   OpenGL-Oberfläche mit einem Fbdev-Device initialisieren
// --------------------------------------------------------------------------
static int surfaceInitialize(struct opengles *opengles)
{
	// Handler für eine Verbindung zu einem X-Server.
	Display *xlib_display;

	// Handler für ein Fenster innerhalb eines X-Servers.
	Window xlib_window;

	// Handler für das Fenster, in dem unser eigenes Fenster erstellt wird.
	Window xlib_parent;

	// Attribute für das zu erstellende Fenster.
	const uint32_t  x = 0,
	                y = 0,
	            width = VIEWPORT_WIDTH,
	           height = VIEWPORT_HEIGHT,
	     border_width = 0,
	           border = 0,
	       background = 0;

	// ---- Initialisierung ---------------------------------------------

	// Verbindung zu Standard-X-Server (localhost) aufbauen.
	xlib_display = XOpenDisplay(0);
	if (xlib_display == 0) {
		fprintf(stderr, "XOpenDisplay fehlgeschlagen!\n");
		exit(-1);
	}

	// Handler für Hauptfenster von xlib_display auslesen.
	xlib_parent = XDefaultRootWindow(xlib_display);

	// Neues X-Fenster erstellen
	xlib_window = XCreateSimpleWindow(xlib_display, xlib_parent,
	                                  x, y, width, height,
	                                  border_width, border, background);

	// Dem Fenster einen Namen geben.
	XStoreName(xlib_display, xlib_window, "swe3-dashboard");

	// Das Fenster sichtbar machen.
	XMapWindow(xlib_display, xlib_window);

	// Alle Befehle sofort an den X-Server senden.
	XFlush(xlib_display);

	Atom wmDeleteMessage;
	wmDeleteMessage = XInternAtom(xlib_display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(xlib_display, xlib_window, &wmDeleteMessage, 1);

	// Variablen in opengles Struktur kopieren
	opengles->native_display = xlib_display;
	opengles->native_window  = xlib_window;
	opengles->native_width   = width;
	opengles->native_height  = height;

	return 0;
}

// --------------------------------------------------------------------------
//   Event-Schleife für X11
// --------------------------------------------------------------------------
int glesRun(struct opengles *gles)
{
	XEvent event;

	// Alle Events abarbeiten.
	while(XPending(((Display*)gles->native_display)))			// KK: explicit type cast added
	{
		// Nächstes Event auslesen.
		XNextEvent(((Display*)gles->native_display), &event);	// KK: explicit type cast added

		// Mini-Handler um WM_DELETE_WINDOW abzufangen.
		switch(event.type)
		{
		case ClientMessage: return 0; break;
		default: break;
		}
	}

	return gles_running;
}

// --------------------------------------------------------------------------
//   Ressourcen für X11-Oberfläche freigeben
// --------------------------------------------------------------------------
void surface_destroy(struct opengles *opengles)
{
	// Speicher für Fenster freigeben
	XDestroyWindow(((Display*)opengles->native_display), opengles->native_window);

	// Verbindung zu X-Server schließen
	XCloseDisplay(((Display*)opengles->native_display));
}

// Diesen Teil des Programms nur nutzen wenn USE_FBDEV deklariert ist.
#else
// ==========================================================================
//   Oberflächenabhängiger Code, Fbdev basierte Funktionen
// ==========================================================================

#include <unistd.h>     // close
#include <fcntl.h>      // O_RDWR
#include <sys/mman.h>   // mmap und dazugehörige Defines
#include <sys/ioctl.h>  // ioctl
#include <linux/fb.h>   // fbdev Werte und Datentypen (z.B. fb_var_screeninfo)

// --------------------------------------------------------------------------
//   Fbdev-Device initialisieren
// --------------------------------------------------------------------------
static int surfaceInitialize(struct opengles *opengles)
{
	//  ---- fbdev ------------------------------------------------------

	// Dateiname des Framebuffer-Devices, Standardwert ist "/dev/fb0"
	const char *fbdev_file = "/dev/fb0";

	// Handler für Framebuffer-Device
	int fbdev_fd;

	// Zwischenspeicher für Rückgabewerte und andere Dinge
	int value;

	// Datenstruktur mit festen Informationen des Framebuffer-Devices,
	// Beispielsweise der Framebuffer-Größe, verwendeter Treiber, e.t.c.
	struct fb_var_screeninfo variable_screeninfo;

	// Framebuffer-Device öffnen
	fbdev_fd = open(fbdev_file, O_RDONLY);
	handlePosixError(fbdev_fd < 0, "open: ");

	// Variable Informationen des Framebuffer-Devices auslesen
	value = ioctl(fbdev_fd, FBIOGET_VSCREENINFO, &variable_screeninfo);
	handlePosixError(value, "ioctl FBIOGET_VSCREENINFO: ");

	// Framebuffer-Datei schließen
	value = close(fbdev_fd);
	handlePosixError(value < 0, "close: ");


	//  ---- vtcon ------------------------------------------------------

	// Device vtcon1 öffnen
	int vtcon_fd;
	vtcon_fd = open("/sys/class/vtconsole/vtcon1/bind", O_WRONLY);
	handlePosixError(vtcon_fd < 0, "vtcon/open: ");

	// Textkonsolenfeatures (z.B. Cursor) deaktivieren
	value = write(vtcon_fd, "0\n", 2);
	handlePosixError(value < 0, "vtcon/write: ");

	// vtcon1 schließen
	close(vtcon_fd);


	//  ---- opengles ---------------------------------------------------

	// Variablen in opengles Struktur kopieren
	opengles->native_display = EGL_DEFAULT_DISPLAY;
	opengles->native_window  = 0;
	opengles->native_width   = variable_screeninfo.xres;
	opengles->native_height  = variable_screeninfo.yres;

	return 0;
}

// --------------------------------------------------------------------------
//   Event-Schleife für Fbdev bzw. Konsolenbetrieb
// --------------------------------------------------------------------------
int glesRun(struct opengles *opengles __attribute__((unused)))
{
	return gles_running;
}

// --------------------------------------------------------------------------
//   Ressourcen für Fbdev-Oberfläche freigeben
// --------------------------------------------------------------------------
void surface_destroy(struct opengles *opengles __attribute__((unused)))
{
	//  ---- vtcon ------------------------------------------------------

	// Device vtcon1 öffnen
	int vtcon_fd;
	vtcon_fd = open("/sys/class/vtconsole/vtcon1/bind", O_WRONLY);
	handlePosixError(vtcon_fd < 0, "vtcon/open: ");

	// Textkonsolenfeatures (z.B. Cursor) aktivieren
	int ret;
	ret = write(vtcon_fd, "1\n", 2);
	handlePosixError(ret < 0, "vtcon/write: ");

	// vtcon1 schließen
	close(vtcon_fd);

	return;
}

#endif
// ==========================================================================
//   Weiter mit Oberflächenunabhänigem Code
// ==========================================================================

// --------------------------------------------------------------------------
//   OpenGL ES Kontext samt Oberfläche zum Zeichnen erstellen
// --------------------------------------------------------------------------
int glesInitialize(struct opengles *opengles)
{
	// ------------------------------------------------------------------
	//   Oberfläche und Singnalhandler
	// ------------------------------------------------------------------

	// Signalhandler registrieren. Hiermit wird ein "Strg+C" in
	// der Konsole abgefangen, um das Programm sauber beenden zu können.
	signal(SIGINT, signal_sigint_handler);

	// Für EGL/OpenGL nutzbare Oberfläche initialiseren
	surfaceInitialize(opengles);

	EGLNativeDisplayType nat_display = opengles->native_display;
	EGLNativeWindowType  nat_window  = opengles->native_window;

	// ------------------------------------------------------------------
	//   EGL
	// ------------------------------------------------------------------

	// Zwischenspeicher für Rückgabewerte von Funktionen wie eglInitialize.
	EGLBoolean ret;

	// Zwei Integer die die Major und Minor Version von EGL beinhalten.
	EGLint eglmajor, eglminor;

	// Handler für ein Display, hier eine Verbindung zu einem X-Server.
	EGLDisplay display;

	// Handler für ein Surface, in diesem Fall dem Framebuffer.
	EGLSurface surface;

	// Liste mit Eigenschaften die der Framebuffer haben muss.
	const EGLint configAttr[] = {
		EGL_SURFACE_TYPE    , EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE , EGL_OPENGL_ES_BIT,
		EGL_NONE
	};

	// Rückgabewerte von eglChooseConfig. Beinhalten eine (oder mehrere)
	// Konfiguration(en) mit den von configAttr geforderten Eigenschaften.
	EGLConfig   config;
	EGLint      configCount;

	// Handler für einen OpenGL-Kontext.
	EGLContext  context;

	// Version des OpenGL ES Kontextes festlegen.
	const EGLint contextAttr[] = {
		EGL_CONTEXT_CLIENT_VERSION , 1,
		EGL_NONE
	};

	// ---- Initialisierung ---------------------------------------------

	// EGL-Display-Handler für unsere im oberen Abschnitt
	// aufgebaute X-Server-Verbindung erstellen.
	display = eglGetDisplay(nat_display);
	handleEglError(display == EGL_NO_DISPLAY, "eglGetDisplay");

	// Eine Verbindung zwischen EGL und X-Server herstellen
	// sowie die EGL Version auslesen und ausgeben
	ret = eglInitialize(display, &eglmajor, &eglminor);
	handleEglError(ret == EGL_FALSE, "eglInitialize");
	printf("EGL Version %d.%d\n", eglmajor, eglminor);

	// Legt die zu von EGL zu verwendende Render-API fest, hier OpenGL ES.
	ret = eglBindAPI(EGL_OPENGL_ES_API);
	handleEglError(ret == EGL_FALSE, "eglBindAPI");

	// Überprüfen ob uns display einen Framebuffer mit den Eigenschaften
	// aus configAttr zur Verfügung stellen kann.
	// Anzahl der möglichen Rückgabewerte dabei auf 1 limitieren.
	ret = eglChooseConfig(display, configAttr, &config, 1, &configCount);
	handleEglError(ret == EGL_FALSE, "eglChooseConfig");
	if (configCount == 0)
	{
		fprintf(stderr, "eglChooseConfig fehlgeschlagen!\n");
		fprintf(stderr, "configCount == 0, configAttr anpassen!\n");
		exit(-1);
	}

	// Ein surface, hier gleichzeitig der Framebuffer, erstellen und
	// mit dem im oberen Abschnitt erstellten X-Fenster verknüpfen.
	surface = eglCreateWindowSurface(display, config, nat_window, 0);
	handleEglError(surface == EGL_NO_SURFACE, "eglCreateWindowSurface");

	// OpenGL-Kontext erstellen
	context = eglCreateContext(display, config, 0, contextAttr);
	handleEglError(context == EGL_NO_CONTEXT, "eglCreateContext");

	// Display, X-Fenster-Framebuffer und OpenGL-Kontext miteinander
	// verknüpfen und als Basis für alle folgenden gl...-Befehle verwenden
	ret = eglMakeCurrent(display, surface, surface, context);
	handleEglError(ret == EGL_FALSE, "eglMakeCurrent");

	// Variablen für OpenGLES-Struktur kopieren
	opengles->display = display;
	opengles->surface = surface;
	opengles->context = context;
	opengles->width   = VIEWPORT_WIDTH;
	opengles->height  = VIEWPORT_HEIGHT;
	opengles->ratio   = (float)opengles->width / (float)opengles->height;

	// Unterstützung für Texturen aktivieren.
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Viewportwerte berechnen, falls Framebuffer größer als Viewport
	GLint        x = (opengles->native_width  - opengles->width ) / 2;
	GLint        y = (opengles->native_height - opengles->height) / 2;
	GLsizei  width =  opengles->width;
	GLsizei height =  opengles->height;

	// Skalierung für Viewport berechnen
	GLfloat scale_x = (GLfloat)height / (GLfloat)width;
	GLfloat scale_y = 1.0f;

	// Sonderfall: Framebuffer kleiner als Viewport
	if (x < 0)
	{
		// Faktor ausrechnen, mit dem Skaliert werden muss
		GLfloat f = opengles->native_width / (GLfloat)width;

		// Faktor auf Skalierungen anwenden
		scale_x *= f;
		scale_y *= f;
	}


	// Viewport (unsere nutzbare Zeichenfläche) festlegen
	glViewport(x, y, width, height);

	// Attribute der Zeichenfläche setzen
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(scale_x, scale_y, 1.0f);

	// Hintergrundfarbe nach löschen des Framebuffer festlegen.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 0;
}

// --------------------------------------------------------------------------
//   OpenGL ES Buffer austauschen
// --------------------------------------------------------------------------
int glesDraw(struct opengles *opengles)
{
	// Das gezeichnete Bild sichtbar machen
	eglSwapBuffers(opengles->display, opengles->surface);

	return 0;
}

// --------------------------------------------------------------------------
//   Ressourcen freigeben
// --------------------------------------------------------------------------
int glesDestroy(struct opengles *opengles)
{
	// ---- EGL ---------------------------------------------------------

	// Kontext deaktivieren
	eglMakeCurrent(opengles->display, 0, 0, 0);

	// Speicher für Surface freigeben
	eglDestroySurface(opengles->display, opengles->surface);

	// Speicher für Kontext freigeben
	eglDestroyContext(opengles->display, opengles->context);

	// Verbindung zu display schließen
	eglTerminate(opengles->display);

	// EGL-Backend vollständig zurücksetzen
	eglReleaseThread();

	// Ressourcen für Oberfläche freigeben
	surface_destroy(opengles);

	return 0;
}


