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
	struct can_frame frame;
	frame.can_id  = 0x123;		// Can-Nummer der Nachricht (z.B. 100 ist Zündung)
	frame.can_dlc = 4;       // Anzahl der Datenbytes, länge der Nachricht
	frame.data[0] = 0x10;    // Datenbyte 1
	frame.data[1] = 0x11;    // Datenbyte 2
	frame.data[2] = 0x12;    // ..
	frame.data[3] = 0x13;

	// Mithilfe des Befehls write() können eine einzelne Nachricht
	// an das CAN-Interface senden, als Inhalt verwenden wir dazu
	// unsere vorher angelegte Datenstruktur.
	write(fd_can, &frame, sizeof(struct can_frame));

	// Noch besser: close() um CAN aus device-Liste wieder herauszunehmen
	close(fd_can);

	// Programm beenden
	return 0;
}
