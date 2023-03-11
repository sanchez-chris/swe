#include "CSharedMemory.h"
#include <iostream>
using namespace std;

/*
 * constructor. Create a SHM:
 *
 * params: 	name, Size in Byte
 */
CSharedMemory::CSharedMemory (const char * SHM_name, UInt32 Size_in_Byte): mSHM_name(SHM_name), mSize(Size_in_Byte)
{

	shm_unlink(mSHM_name);		// zur Sicherheit immer löschen


  // Versuch, ein Shared Memory-Objekt zu erzeugen
  Int32 fd = shm_open(mSHM_name, O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
  bool success = true;

  if (fd == -1)
  {
    if (errno != EEXIST)
    {
      cout << "can not open " << endl;
      success = false;
    }
    else
    {
      fd = shm_open("TestSharedMem", O_RDWR, FILE_MODE);
      if (fd == -1)
      {
		cout << "check permission " << endl;
		success = false;
      }
      else
      {
    	  cout << "open an existing shared memory" << endl;
      }
    }
  }
  else
  {
    cout << "create a new shared memory " << endl;
  }

  if (true == success)
  {
    ftruncate(fd, mSize);
  }

  // mappen in den eigenen Adressraum
   mPtr = (UInt8 *) mmap(NULL, mSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   //mPtr = new(mPtr)CEvent;


}

CSharedMemory::~CSharedMemory()
{
	shm_unlink(mSHM_name);
};

