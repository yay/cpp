#include <iostream>
#include <string>
#include <string_view>

using namespace std;

// track heap allocations
static uint32_t allocCount = 0;
void* operator new(size_t size)
{
	allocCount++;
	cout << "Allocating: " << size << " bytes\n";
	return malloc(size);
}

void printName(const string_view name) {
	cout << name << endl;
}

void test1()
{
	string name = "Vitaly";  // 1 allocation (if this string was longer, we would have 2 allocation)
	printName(name);         // 0 allocations
	printName("Kravchenko"); // 0 allocations (would be 1 allocation, if the parameter was of type "const string&")
	string firstThree = name.substr(0, 3);  // 1 allocation
	string fullName = name + " Kravchenko"; // 3 allocations
	string_view firstThreeNoAlloc(name.c_str(), 3); // 0 allocations
	printName(firstThreeNoAlloc);
}

void test2()
{
	string hello{ "Hello, " }; // 1 allocation
	string name{ "Vitaly" };   // 1 allocation
	for (int i = 0; i < 1000; i++) {  // 14 allocations
		hello += name;
	}
}

void test3()
{
	string hello; // 1 allocation (16 bytes)
	hello.reserve(6007); // 1 allocation (6063 bytes)
	string_view name{ "Vitaly" };   // 0 allocations
	for (int i = 0; i < 1000; i++) {  // 0 allocations
		hello += name;
	}
}

void test4()
{
	const char* name = "Vitaly Kravchenko"; // 0 allocations
	string_view firstName(name, 6);         // 0 allocations
	string_view lastName(name + 7, 17);     // 0 allocations
	printName(firstName);
	printName(lastName);
}

int main()
{
	test1();
	cout << allocCount << " allocations\n";

	return 0;
}
