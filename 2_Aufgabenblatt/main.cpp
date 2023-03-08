#include <mutex>
#include <new>
#include <pthread.h>
#include "CBinarySemaphore.h"
#include "CSharedMemory.h"
#include "CContainer.h"
#include <iostream>

using namespace std;

int kmh = 0;
int rpm = 0;

CSharedMemory SHM("mem1", sizeof(CContainer));

CContainer *MyContainer;


//int *get_kmh;
//int *get_rpm;


void* thread_1_function(void *arg){


	while(1){
		kmh += 1;
		rpm += 1;
		MyContainer->set(kmh, rpm);
	}
}


void* thread_2_function(void *arg){

	int get_kmh;
	int get_rpm;

	while(1){
			MyContainer->get(&get_kmh, &get_rpm);

			cout << "kmh: " << get_kmh << endl;
			cout << "rpm: " << get_rpm << endl;
		}

}



int main() {

	MyContainer = new(SHM.mPtr) CContainer();

	pthread_t thread_1, thread_2;

	pthread_create(&thread_1, NULL, thread_1_function, NULL);
	pthread_create(&thread_2, NULL, thread_2_function, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);




return 0;


}






