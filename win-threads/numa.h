#pragma once

#include <iostream>
#include <bitset>

/*
	Computers with multiple processors are typically designed for one of two architectures:
	non-uniform memory access (NUMA) or symmetric multiprocessing (SMP).
	In a NUMA computer, each processor is closer to some parts of memory than others,
	making memory access faster for some parts of memory than other parts.
	Under the NUMA model, the system attempts to schedule threads on processors that are close to the memory being used.
	"Thread affinity" forces a thread to run on a specific subset of processors.
	When you specify a "thread ideal processor", the scheduler runs the thread on the specified processor when possible.
	In an SMP computer, two or more identical processors or cores connect to a single shared main memory.
	Under the SMP model, any thread can be assigned to any processor.
	Therefore, scheduling threads on an SMP computer is similar to scheduling threads on a computer with a single processor.
*/
void numa() {
	ULONG highestNodeNumber;
	if (!GetNumaHighestNodeNumber(&highestNodeNumber)) {
		printf("GetNumaHighestNodeNumber failed: %u\n", GetLastError());
		std::exit(1);
	}
	printf("Numa Highest Node Number: %u\n", highestNodeNumber); // 0 for Ryzen 3 3200G

	ULONGLONG processorMask;
	GetNumaNodeProcessorMask(0, &processorMask);
	std::bitset<sizeof(processorMask) * 8> bits(processorMask);
	std::cout << "Processor mask: " << bits << std::endl;
}