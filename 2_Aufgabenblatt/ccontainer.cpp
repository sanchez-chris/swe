/*
 * ccontainer.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: vmuser
 */

#include "CContainer.h"


//CContainer constructor
CContainer::CContainer(): kmh(0), rpm(0), read_sem(false, true), write_sem(true, true) {


}

//  Methoden
void CContainer::set(int KMH, int RPM) {
	write_sem.take(); //
    kmh = KMH;
    rpm = RPM;
    read_sem.give();
}

int CContainer::get(int* KMH, int* RPM) {
	read_sem.take();
	*KMH = kmh;
    *RPM = rpm;
    write_sem.give();
}
