#pragma once

#include <Windows.h>

void syncThreads() {
	/*
	https://docs.microsoft.com/en-us/windows/win32/procthread/synchronizing-execution-of-multiple-threads
	There are a number of objects whose handles can be used to synchronize multiple threads.
	The state of each of these objects is either signaled or not signaled.
	When you specify a handle to any of these objects in a call to one of the wait functions,
	the execution of the calling thread is blocked until the state of the specified object becomes signaled.

	Some of these objects are useful in blocking a thread until some event occurs.
	For example, a console input buffer handle is signaled when there is unread input, such as a keystroke or mouse button click.
	Process and thread handles are signaled when the process or thread terminates.

	Other objects are useful in protecting shared resources from simultaneous access.
	For example, multiple threads can each have a handle to a mutex object.
	Before accessing a shared resource, the threads must call one of the wait functions
	to wait for the state of the mutex to be signaled. When the mutex becomes signaled,
	only one waiting thread is released to access the resource. The state of the mutex
	is immediately reset to not signaled so any other waiting threads remain blocked.
	When the thread is finished with the resource, it must set the state of the mutex to signaled
	to allow other threads to access the resource.

	For the threads of a single process, critical-section objects provide a more efficient
	means of synchronization than mutexes. A thread can use the "EnterCriticalSection" function
	to request ownership of a critical section. If it is already owned by another thread,
	the requesting thread is blocked. A thread can use the "TryEnterCriticalSection" function
	to request ownership of a critical section, without blocking upon failure to obtain
	the critical section. After it receives ownership, the thread is free to use the protected resource.
	*/
}