#pragma once

#include <Windows.h>
#include <stdio.h>

void createProcess(int argc, TCHAR* argv[]) {
	STARTUPINFO si{};
	PROCESS_INFORMATION pi{};

	if (argc != 2) {
		printf("Usage: %s [path to executable]\n", argv[0]);
		std::exit(1);
	}

	// Start the child process. 
	printf("----- process start ----\n");
	if (!CreateProcess(nullptr,   // No module name (use command line)
		argv[1],        // Command line
		nullptr,        // Process handle not inheritable
		nullptr,        // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		nullptr,        // Use parent's environment block
		nullptr,        // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)            // Pointer to PROCESS_INFORMATION structure
		) {
		printf("CreateProcess failed (%lu).\n", GetLastError());
		std::exit(1);
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(pi.hProcess); // process handle
	CloseHandle(pi.hThread);  // primary thread handle
	printf("----- process end ----\n");
}