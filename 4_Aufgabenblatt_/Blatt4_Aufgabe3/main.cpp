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
	//1 kmh = 1,5 grad rotation
	int deg_of_kmh = (-kmh * 1.5 + 136);

	return deg_of_kmh;
}



GLfloat rpm2deg(GLfloat rpm) {
	//1000 rpm = 26,25 grad rotation
	int deg_of_rpm = (rpm/1000 * 30 - 135);

	return deg_of_rpm;
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

		// Tachonadel rotieren
		glRotatef(kmh2deg(0), 0.0f, 0.0f, 1.0f);

		// Tachonadel verschieben.
		glTranslatef(0, 0.25, 0);


		// Tachonadel zeichnen.
		tileDraw(&needle);


		glPopMatrix();



		// ---- Rechte Tachonadel zeichnen ---------------------------


		glPushMatrix();


		// Tachonadel verschieben.
		glTranslatef(1, 0, 0);

		// Tachonadel rotieren bis 0 rpm winkel -135 und 7000 rpm winkel 75
		glRotatef(rpm2deg(0), 0.0f, 0.0f, 1.0f);

		// Tachonadel verschieben.
		glTranslatef(0, 0.25, 0);

		// Tachonadel zeichnen.
		tileDraw(&needle);

		glPopMatrix();

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
