#pragma once

#include "windows.h"
#include "stdio.h"

void systemInfo() {
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	printf("Is AMD64: %d\n", systemInfo.dwProcessorType == PROCESSOR_AMD_X8664);
	printf("Number of processors: %u\n", systemInfo.dwNumberOfProcessors);
}