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
#include <unistd.h>     // write(), read()
#include <linux/can.h>  // struct can_frame
#include "lib/can.h"    // can_open()
#include <fcntl.h>		//open


int main (void)
{




	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can = open("./test.bin", O_RDWR | O_CREAT, 0660);
	//int fd_can = can_open("vcan0");	// erzeugt filedeskriptor und öffnet das Device VCAN0

	// Diese Datenstruktur enthält eine einzelne CAN-Nachricht,
	// deren Inhalt wir nach Belieben festlegen können.
	struct can_frame frame;
	frame.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
	frame.can_dlc = 4;       // Anzahl der Datenbytes, länge der Nachricht
	frame.data[0] = 0x10;    // Datenbyte 1
	frame.data[1] = 0x11;    // Datenbyte 2
	frame.data[2] = 0x12;    // ..
	frame.data[3] = 0x13;

	struct can_frame frame2;
	frame2.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
	frame2.can_dlc = 4;       // Anzahl der Datenbytes, länge der Nachricht
	frame2.data[0] = 0x10;    // Datenbyte 1
	frame2.data[1] = 0x11;    // Datenbyte 2
	frame2.data[2] = 0x12;    // ..
	frame2.data[3] = 0x13;

	struct can_frame frame3;
	frame3.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
	frame3.can_dlc = 4;       // Anzahl der Datenbytes, länge der Nachricht
	frame3.data[0] = 0x10;    // Datenbyte 1
	frame3.data[1] = 0x11;    // Datenbyte 2
	frame3.data[2] = 0x12;    // ..
	frame3.data[3] = 0x13;

	// Mithilfe des Befehls write() können eine einzelne Nachricht
	// an das CAN-Interface senden, als Inhalt verwenden wir dazu
	// unsere vorher angelegte Datenstruktur.

	//write(fd_can, &frame.data[0], 1);




	write(fd_can, &frame, sizeof(struct can_frame));

	write(fd_can, &frame2, sizeof(struct can_frame));

	write(fd_can, &frame3, sizeof(struct can_frame));




	perror("");
	// Noch besser: close() um CAN aus device-Liste wieder herauszunehmen
	close(fd_can);

	// Programm beenden
	return 0;
}
