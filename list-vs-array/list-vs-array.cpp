#include <algorithm>
#include <iostream>
#include <list>
#include <chrono> 
#include <vector>

// https://lemire.me/blog/2019/11/25/memory-parallelism-amd-rome-versus-intel

// The very latest Intel processors (e.g., Cannon Lake), can sustain more than 20 memory requests at any one time (AMD too).
// It is about twice what the prior generation (Skylake) could do.

// In a linked list, you access the element of a list one by one, as the location of the next entry is always coded
// in the current entry and nowhere else. Obviously, it is a potential problem performance-wise because it makes it hard
// to exploit memory-level parallelism.

int main() {
	for (size_t size = 1024; size < 1024 * 1024; size *= 2) {
		std::list<int> l;
		std::vector<int> v;

		for (int z = 0; z < (int)size; z++) l.push_back(z);
		for (int z = 0; z < (int)size; z++) v.push_back(z);

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		int sum = 0;
		for (int n : l) { sum += n; }

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		for (int n : v) { sum += n; }
		if (sum == 0) printf("bug\n");

		std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

		double dif = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
		double div = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();
		std::cout << size << " " << dif / size << " " << div / size << " " << dif / div << std::endl;
	}
}
