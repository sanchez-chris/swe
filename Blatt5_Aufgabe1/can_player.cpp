/*
 * can_player.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: vmuser
 */




// Lokale Header
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // write()
#include <fcntl.h>      // O_RDONLY
#include <linux/can.h>  // struct can_frame
#include "lib/can.h"    // can_open()
#include <iostream>

using namespace std;

#define CAN_INTERFACE    "vcan0"
#define LOG_FILE "logs/20121011_bmw_stopngo_5min.raw"

struct timevalx
{
	int64_t tv_sec;
	int64_t tv_usec;
};

struct log_entry
{
	struct timevalx timeval;
	struct can_frame can_frame;
};


void can_player_main()
{

	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can = can_open(CAN_INTERFACE);


	// Log-Datei öffnen. Hier sind die Daten in Form von struct log_entry
	// Einträgen hinterlegt, die nacheinander mit read() lesbar sind.
	int fd_log = open(LOG_FILE, O_RDONLY, 0660);


	struct log_entry entry;

	while(1) {
		// Einzelnen Log-Eintrag auslesen.




		int ret = read(fd_log, &entry, sizeof(struct log_entry));

		if (ret <=0){
			break;
		}


		// Eine einzelne Nachricht an das CAN-Interface senden.
		// Als Inhalt dient das can_frame innerhalb der log_entry Struktur.
		write(fd_can, &entry.can_frame, sizeof(struct can_frame));
		//cout << entry.timeval.tv_sec << endl;
		usleep(5);
	}

perror("");
}
