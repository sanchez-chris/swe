/*
 * can_reader.cpp
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

#include "can_player.h"
#include <cstdint> //uint32
#include "lib/CContainer.h"
#include <iostream>

using namespace std;

#define CAN_INTERFACE    	"vcan0"
#define LOG_FILE 			"logs/20121011_bmw_stopngo_5min.raw"


extern CContainer* MyContainer;


void warten() {

}


void can_reader_main()
{

	// virtual can öffnen. nochmal aufmachen????
	// Einträgen hinterlegt, die nacheinander mit read() lesbar sind.
	int fd_vcan0 = can_open(CAN_INTERFACE);

	struct can_frame frame;
	float kmh, rpm;

	while(1) {



		read(fd_vcan0, &frame, sizeof(frame));

		// Eine einzelne Nachricht an Shared Memory senden.
		// Als Inhalt dient das can_frame innerhalb der log_entry Struktur.


		uint32_t id;





		id = frame.can_id & CAN_EFF_MASK; // siehe can.h
		uint8_t *d = &(frame.data[0]);

		switch(id)
		{
		case 0x254:
					kmh = ( (((d[1]*256.0f + d[0]) - 32767.0f) / 50.0f) +
					(((d[3]*256.0f + d[2]) - 32767.0f) / 50.0f) +
					(((d[5]*256.0f + d[4]) - 32767.0f) / 50.0f) +
					(((d[7]*256.0f + d[6]) - 32767.0f) / 50.0f) ) / 4.0f;
					break;
		case 0xa5:
					rpm = ((d[6]* 256.0f) + d[5]) / 4.0f;
					break;

		default:
					break;
		}

		MyContainer -> set(kmh, rpm);
		cout << kmh << endl;

		usleep(5);
	}






}

