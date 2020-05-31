#pragma once

#include <Windows.h>

void threadLocalStorage() {
	/*
	// https://docs.microsoft.com/en-us/windows/win32/procthread/thread-local-storage
	All threads of a process share its virtual address space. The local variables of a function
	are unique to each thread that runs the function. However, the static and global variables
	are shared by all threads in the process.

	With thread local storage (TLS), you can provide unique data for each thread that the process
	can access using a global index. One thread allocates the index, which can be used by the other
	threads to retrieve the unique data associated with the index.

	When the threads are created, the system allocates an array of LPVOID values for TLS,
	which are initialized to NULL. Before an index can be used, it must be allocated by one of the threads.
	Each thread stores its data for a TLS index in a TLS slot in the array.

	It is ideal to use TLS in a dynamic-link library (DLL).
	*/
	printf("TLS_MINIMUM_AVAILABLE: %d\n", TLS_MINIMUM_AVAILABLE);
}