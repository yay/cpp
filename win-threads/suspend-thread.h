#pragma once

#include <Windows.h>

void suspendThread() {
    printf("Suspend thread: %lu zzz...\n", GetCurrentThreadId());
    Sleep(1000); // it's like a SuspendThread/ResumeThread combo
    printf("Resume thread: %lu\n", GetCurrentThreadId());
}