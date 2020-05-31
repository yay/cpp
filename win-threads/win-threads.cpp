// https://docs.microsoft.com/en-us/windows/win32/procthread/processes-and-threads

/*
An application consists of one or more processes. A "process", in the simplest terms, is an executing program.
A "thread" is the entity within a process that can be scheduled for execution.
All threads of a process share its virtual address space and system resources.
Each process is started with a single thread, but can create additional threads from any of its threads.
A "thread pool" is a collection of worker threads that efficiently execute asynchronous callbacks on behalf of the application.
A "fiber" is a unit of execution that must be manually scheduled by the application.
Fibers run in the context of the threads that schedule them.

With preemptive multitasking the system allocates a processor time slice to each thread it executes.
The length of the time slice depends on the operating system and the processor (approximately 20 ms).
The process priority class and thread priority level are combined to form the base priority of each thread (0..31).
The scheduler maintains a queue of executable threads for each priority level.
These are known as ready threads. When a processor becomes available, the system performs a context switch.

The steps in a context switch are:

- Save the context of the thread that just finished executing.
- Place the thread that just finished executing at the end of the queue for its priority.
- Find the highest priority queue that contains ready threads.
- Remove the thread at the head of the queue, load its context, and execute it.

The most common reasons for a context switch are:

- The time slice has elapsed.
- A thread with a higher priority has become ready to run.
- A running thread needs to wait.
*/

#include "Windows.h"
#include "tchar.h"
#include <string>
#include <stdio.h>

#include "create-thread.h"

int main(int argc, TCHAR* argv[]) {
	createThread();
	return 0;
}
