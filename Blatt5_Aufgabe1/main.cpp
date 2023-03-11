// --------------------------------------------------------------------------
// SWE3 DashBoard - Aufgabe 1
//
// Bearbeitet von:
//     Name, Matrikelnummer
//     Name, Matrikelnummer
//
// --------------------------------------------------------------------------
// Header
#include <stdio.h>     // printf, fprintf, perror
#include <stdlib.h>    // exit
#include <math.h>      // z.B. M_PI, cos(), sin()
//#include <GLES/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen  	Uncomment/Comment for Target
#include <GL/gl.h>   // OpenGL ES 1.1 Datentypen und Funktionen			Uncomment/comment for Host
#include <unistd.h>
#include "lib/gles.h"  // struct opengles, gles*-Funktionen
#include "lib/tile.h"  // struct tile, loadPngTile
#include <unistd.h>
#include <iostream>
#include "pthread.h"
#include "hmi.h"
#include "graphic.h"

#include "can_player.h"
#include "can_reader.h"
#include "lib/CSharedMemory.h"
#include "lib/CContainer.h"

CSharedMemory SHM("mem1", sizeof(CContainer));
CContainer *MyContainer;




int main(void) {

	MyContainer = new(SHM.mPtr) CContainer();

	//declare the children processes
	pid_t canPlayer, canReader;


	//create process canPlayer
	canPlayer = fork();

	if(canPlayer == 0) { // its the child process (canPlayer)

		can_player_main();
	}

	//create process canReader
	canReader = fork();

	if(canReader == 0) { // its the child process (canReader)

			can_reader_main();
		}






	pthread_t thread_1, thread_2;
	pthread_create(&thread_1, NULL, logic_main, NULL); //create a thread for logic_main
	pthread_create(&thread_2, NULL, graphic_main, NULL); //create a thread for graphic_main
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	return 0;
}




