/*
 * CContainer.h
 *
 *  Created on: Mar 7, 2023
 *      Author: vmuser
 */

#ifndef CCONTAINER_H_
#define CCONTAINER_H_
#include "CBinarySemaphore.h"

class CContainer {

private:
	float kmh = 0;
	float rpm = 0;
	CBinarySemaphore read_sem, write_sem;


public:
    CContainer();


    // Set und Get Methoden
    void set(float KMH, float RPM);

    int get(float *KMH, float *RPM);

};


#endif /* CCONTAINER_H_ */
