// --------------------------------------------------------------------------
// SWE3 CanParser - Aufgabe 1
//
// Bearbeitet von:
//		Name, Matrikelnummer
//     	Name, Matrikelnummer
//
// --------------------------------------------------------------------------
// vor dem Ausführen: vcan0 erzeugen und CAN-Logger starten!

// Lokale Header
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // write()
#include <linux/can.h>  // struct can_frame
#include "lib/can.h"    // can_open()


int main (void)
{
	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can;
	fd_can = can_open("vcan0");	// erzeugt filedeskriptor und öffnet das Device VCAN0

	// Diese Datenstruktur enthält eine einzelne CAN-Nachricht,
	// deren Inhalt wir nach Belieben festlegen können.
	struct can_frame frame1;
	frame1.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
	frame1.can_dlc = 4;       // Anzahl der Datenbytes, länge der Nachricht
	frame1.data[0] = 0x10;    // Datenbyte 1
	frame1.data[1] = 0x11;    // Datenbyte 2
	frame1.data[2] = 0x12;    // ..
	frame1.data[3] = 0x13;

	struct can_frame frame2;
		frame2.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
		frame2.can_dlc = 8;       // Anzahl der Datenbytes, länge der Nachricht
		frame2.data[0] = 0x14;    // Datenbyte 1
		frame2.data[1] = 0x15;    // Datenbyte 2
		frame2.data[2] = 0x16;    // ..
		frame2.data[3] = 0x17;
		frame2.data[4] = 0x18;    // Datenbyte 1
		frame2.data[5] = 0x19;    // Datenbyte 2
		frame2.data[6] = 0x20;    // ..
		frame2.data[7] = 0x21;


	while(1) {
	write(fd_can, &frame1, sizeof(struct can_frame));

	for(int i=0; i<4;i++)  {
				frame1.data[i]++;

			}

	usleep(2000);
	}
	//write(fd_can, &frame2, sizeof(struct can_frame));

	// Mithilfe des Befehls write() können eine einzelne Nachricht
	// an das CAN-Interface senden, als Inhalt verwenden wir dazu
	// unsere vorher angelegte Datenstruktur.

/*	while(1) {
		write(fd_can, &frame1, sizeof(struct can_frame));

		usleep(1000);
		write(fd_can, &frame2, sizeof(struct can_frame));

		for(int i=0; i<8;i++)  {
			frame1.data[i]++;
			frame2.data[i] += 2;
		}
	}
*/



	// Noch besser: close() um CAN aus device-Liste wieder herauszunehmen
	close(fd_can);

	// Programm beenden
	return 0;
}
