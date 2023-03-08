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
	int kmh = 0;
	int rpm = 0;
	CBinarySemaphore read_sem, write_sem;


public:
    CContainer();


    // Set und Get Methoden
    void set(int KMH, int RPM);

    int get(int *KMH, int *RPM);

};


#endif /* CCONTAINER_H_ */
