#pragma once

#include <iostream>
#include <thread>

void greet(const char* name) {
	printf("Hello, %s\n", name); // single operation, output is always consistent
	std::cout << "Hello, " << name << std::endl; // output can get messed up when `greet` is called from multiple threads
}
void greet_everyone() {
	std::thread t1{ []() { greet("Alice"); } };
	std::thread t2{ []() { greet("Bob"); } };
	std::thread t3{ []() { greet("Vitaly"); } };
	t1.join();
	t2.join();
	t3.join();
}