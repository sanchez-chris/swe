// --------------------------------------------------------------------------
// SWE3 DashBoard - Aufgabe 1
//
// Bearbeitet von:
//     Name, Matrikelnummer
//     Name, Matrikelnummer
//
// --------------------------------------------------------------------------

// Header
#include <stdio.h>     // printf, fprintf
#include <stdlib.h>    // exit
#include <unistd.h>    // sleep
#include <math.h>      // z.B. M_PI, cos(), sin()
//#include <GLES/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen  	Uncomment/Comment for Target
#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include "lib/gles.h"  // struct opengles, gles*-Funktionen
#include "lib/tile.h"  // struct tile, loadPngTile


GLfloat kmh2deg(GLfloat kmh) {

	//0,9 kmh each grad rotation
	int deg = (kmh * 0.9 + 136);

	return deg;

}



int main(void)
{
	// OpenGL ES initialisieren
	struct opengles opengles;
	glesInitialize(&opengles);

	// Textur für Dashboard laden
	struct tile dashboard = TILE_ZEROINIT;
	tileLoadPng(&opengles, &dashboard, "./bilder/dashboard.png");

	// Textur für Tachonadel laden
	struct tile needle = TILE_ZEROINIT;
	tileLoadPng(&opengles, &needle, "./bilder/needle.png");


	do
	{
		// Framebuffer löschen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Dashboard zeichnen
		tileDraw(&dashboard);


		// ---- Linke Tachonadel zeichnen ---------------------------

		glPushMatrix();

			// Tachonadel verschieben.
			glTranslatef(-1, 0, 0);

			// Tachonadel rotieren bis 0 Kmh
			glRotatef(kmh2deg(0), 0.0f, 0.0f, 1.0f);

			// Tachonadel verschieben.
			glTranslatef(0, 0.25, 0);


			// Tachonadel zeichnen.
			tileDraw(&needle);


/*

			//Tachonadel verschieben
			glTranslatef(-1, 0, 0);

			// Tachonadel rotieren bis 150 Kmh
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

			// Tachonadel verschieben.
			glTranslatef(0, 0.25, 0);
*/

		glPopMatrix();

		// ---- Das gezeichnete Bild sichtbar machen ----------------
		glesDraw(&opengles);



		usleep(50000); //20 Bilder/sek

	}
	while(glesRun(&opengles));



	// OpenGL ES Ressourcen freigeben.
	glesDestroy(&opengles);

	return 0;
}
