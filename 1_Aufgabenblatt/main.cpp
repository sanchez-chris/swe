/*
 * main.cpp
 *
 *  Created on: Mar 6, 2023
 *      Author: sach1024
 */


 #include <unistd.h>

#include <iostream>
#include "pthread.h"

using namespace std;


void main_child() {

	while (1){
		cout << "Child here" << endl;
	}
}

void main_parent() {

	while (1){
		cout << "Parent here" << endl;
	}
}

void* thread_func0(void *arg) {
	cout << "0" << endl;
	return 0;
}

void* thread_func1(void *arg) {
	cout << "1" << endl;
	return 0;
}



int main() {
		pid_t pid1, pid2;

	    // Create first process
		pid1 = fork();
	    if (pid1 != 0) {//it is main parent

	        main_parent();
	    }

	    else { //it is main child
	    	main_child();
	    }


	     pid2 = fork();
	     if (pid2 == 0) {
	        // this is the child process 2
	        main_child();

	       }


	     pthread_t thread_0, thread_1;
	     pthread_create(&thread_0, NULL, thread_func0, NULL); //create a thread for func 0
	     pthread_create(&thread_1, NULL, thread_func1, NULL); //create a thread for func 1

	     pthread_join(thread_0, NULL);
	     pthread_join(thread_1, NULL);

	     return 0;


}


/*
 Aufgabe 3 und 4

 To test the thread safety of a program, the SCHED_FIFO scheduling policy can be set in the operating system.


SCHED_FIFO is a real-time scheduling policy in which processes are executed in first-come, first-served order.
Processes run until they block or terminate, and then the next process in the queue is executed.

By setting this scheduling policy, the program threads will execute sequentially and in order.
This allows testing whether the program is thread safe, since the threads run without interfering with each other.


 * */
