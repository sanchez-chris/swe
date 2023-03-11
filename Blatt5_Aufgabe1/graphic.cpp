/*
 * graphic.cpp
 *
 *  Created on: Mar 9, 2023
 *      Author: vmuser
 */

#include "graphic.h"

extern float kmh_degree;
extern float rpm_degree;

void* graphic_main(void*) {

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

			// Tachonadel rotieren: 0kmh -> 136		150kmh: -90
			glRotatef(kmh_degree, 0.0f, 0.0f, 1.0f);

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
			glRotatef(rpm_degree, 0.0f, 0.0f, 1.0f);

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


			usleep(10000);


		}
		while(glesRun(&opengles));



		// OpenGL ES Ressourcen freigeben.
		glesDestroy(&opengles);

		return 0;
}
