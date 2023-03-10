/*
 * hmi.cpp
 *
 *  Created on: Mar 9, 2023
 *      Author: vmuser
 */
// Header

#include "hmi.h"


float kmh_degree;
float rpm_degree;



GLfloat kmh2deg(GLfloat kmh) {
	//1 kmh = 1,5 grad rotation
	float deg_of_kmh = (-kmh * 1.5 + 136);

	return deg_of_kmh;
}


GLfloat rpm2deg(GLfloat rpm) {
	//1000 rpm = 26,25 grad rotation
	float deg_of_rpm = ((rpm/1000) * 30 - 135);

	return deg_of_rpm;
}

void* logic_main(void*)
{
	float kmh = 0;
	float rpm = 0;

	while(1){

		if(kmh < 150) {
			kmh += 0.3;
		}

		while (kmh <150) {
			if (rpm >= 7000) {
				rpm =2000;
			}

			else{
				rpm += 51.8;
			}

			break;

		}

		kmh_degree = kmh2deg(kmh);
		rpm_degree = rpm2deg(rpm);
		usleep(10000); //20 Bilder/sek
	}



	return 0;
}
