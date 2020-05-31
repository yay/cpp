/*
https://docs.microsoft.com/en-us/windows/win32/procthread/creating-threads
*/

#pragma once

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

/*
The `CreateThread` function creates a new thread for a process.
The creating thread must specify the starting address of the code that the new thread is to execute.
The calling thread
*/

#define MAX_THREADS 3
#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct MyData {
	int val1;
	int val2;
} MYDATA, * PMYDATA;

int createThread() {
	PMYDATA pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	// Create MAX_THREAD worker threads.
	for (int i = 0; i < MAX_THREADS; i++) {
		// Allocate memory for thread data.
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
        if (pDataArray[i] == nullptr) {
			ExitProcess(2);
		}

		pDataArray[i]->val1 = i;
		pDataArray[i]->val2 = i + 100;

		hThreadArray[i] = CreateThread(
			NULL,                 // default security attributes
			0,                    // use default stack size
			MyThreadFunction,     // thread function name,
			pDataArray[i],        // argument to thread function
			0,                    // use default creation flags
			&dwThreadIdArray[i]   // returns the thread identifier
		);

		// Check the return value for success.
		// If `CreateThread` fails, terminate execution.
		// This will automatically clean up threads and memory.
        if (hThreadArray[i] == nullptr) {
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	}

	// Wait until all threads have terminated.
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.
	for (int i = 0; i < MAX_THREADS; i++) {
		CloseHandle(hThreadArray[i]);
        if (pDataArray[i] != nullptr) {
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = nullptr; // Ensure address is not reused.
		}
	}

	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	// Make sure there is a console to receive output results.

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		return 1;
	}

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.
	pDataArray = (PMYDATA)lpParam;

	// Print the parameter values using thread-safe functions.
	// (C run-time library (CRT) versions are not thread-safe.)
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
		pDataArray->val1, pDataArray->val2);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	return 0;
}

void ErrorHandler(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code.
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message.
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40L) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	// Free error-handling buffer allocations.
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
