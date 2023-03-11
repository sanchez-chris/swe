/*
 * hmi.cpp
 *
 *  Created on: Mar 9, 2023
 *      Author: vmuser
 */
// Header

#include "hmi.h"
#include "lib/CContainer.h"

float kmh_degree = 0.0f;
float rpm_degree = 0.0f;



GLfloat kmh2deg(GLfloat kmh) {
	//1 kmh = 1,5 grad rotation
	float deg_of_kmh = (-kmh * 1.5 + 135);

	return deg_of_kmh;
}


GLfloat rpm2deg(GLfloat rpm) {
	//1000 rpm = 26,25 grad rotation
	float deg_of_rpm = ((rpm/1000.0) * 30 - 135);

	return deg_of_rpm;
}

extern CContainer* MyContainer;

void* logic_main(void*)
{

	float get_kmh, get_rpm;
	while(true) {
	MyContainer->get(&get_kmh, &get_rpm);



		kmh_degree = kmh2deg(get_kmh);
		rpm_degree = rpm2deg(get_rpm);
	}
		return 0;
}






