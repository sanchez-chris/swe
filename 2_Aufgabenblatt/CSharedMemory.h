/*  openICM-framework
 ***************************************************************************
 *  Copyright 2010 Joachim Wietzke and Manh Tien Tran
 *
 *  Contact and informations:
 *  University of Applied Sciences Darmstadt, Germany
 *  	http://www.fbi.h-da.de/~j.wietzke
 *  or
 *  	http://www.fbi.h-da.de/~openicm
 *
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  	http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#ifndef _CSHAREDMEMORY_H
#define _CSHAREDMEMORY_H

/**
 * Simple wrapper for SHM creation. const char * name for now. Size as an uint Parameter.
 * * enter rt in path/library
 *
 * @code
 * while (true ) {
 * 	sem.take();  // wait for signal
 *
 *	// get the data and do some works with it
 *	...
 * }
 * @endcode
 *
 * The sender thread (it can be an interrupt handler) can signal
 * the receiver by releasing the semaphore.
 *
 * @code
 * 	sem.give();
 * @endcode
 *
 * An object of class CBinarySemaphore can be taken by different processes.
 * It is also designed to be placed into a shared memory.
 */

#include "Global.h"
#include "FWAssertion.h"

#include <sys/time.h>
#include <errno.h>
#include <pthread.h>				// auch lib anziehen !!

 // die beiden Inkludes sind notwendig für POSIX-Shared-Memory   rt in path library eintragen!
 #include <sys/types.h>
 #include <sys/mman.h>
 // unistd notwendig wegen ftruncate
 #include <unistd.h>
 // fcntl.h notwendig wegen der Konstanten O_RDER, O_CREAT etc.)
 #include <fcntl.h>
 // wegen S_IRUSR usw.
 #include <sys/stat.h>


 //extern int errno;

 #define FILE_MODE (S_IRUSR| S_IWUSR | S_IRGRP | S_IROTH)


 // semaphore must be simulated

class CSharedMemory {
public:
	/**
	 * \brief constructor. Create a SHM. No Syncs, bcs. the objects themselves should provide them
	 *
	 */
	CSharedMemory (const char * SHM_name, UInt32 Size_in_Byte);
	~CSharedMemory();

	UInt8 * mPtr;

private:
	// to avoid misuse
	CSharedMemory(const CSharedMemory & rhs);
	CSharedMemory & operator=(const CSharedMemory & rhs);

	const char * mSHM_name;
	size_t mSize;

};

#endif // end of _CSHAREDMEMORY_H





