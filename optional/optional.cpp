#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <variant>
#include <any>

using namespace std;

optional<string> readFileAsString(const string& filepath)
{
	ifstream stream(filepath);
	if (stream)
	{
		string result;
		string line;
		while (getline(stream, line)) {
			result += line + '\n';
		}
		stream.close();
		return result;
	}
	return {};
}

void testOptional()
{
	optional<string> data = readFileAsString("data.txt");
	if (data) { // same as data.has_value(), because the optional has the bool operator
		cout << data.value() << endl;
	}
	else {
		cout << "File could not be opened.\n";
	}
	cout << data.value_or("File not found.") << endl;
}

enum class ErrorCode {
	Node = 0,
	NotFound = 1,
	NoAccess = 2
};

variant<string, ErrorCode> readFileAsString2(const string& filepath)
{
	return ErrorCode::NotFound;
}

void testVariant() {
	variant<string, int> data; // unlike unions, variant's size is size of all types combined
	cout << "sizeof(string): " << sizeof(string) << endl; // 40
	cout << "sizeof(int): " << sizeof(int) << endl;       // 4
	cout << "sizeof(data): " << sizeof(data) << endl;     // 48 = 40 + 4 + alignment

	data = "Hello";
	cout << data.index() << ": " << get<string>(data) << endl;
	data = 12;
	cout << data.index() << ": " << get<int>(data) << endl;

	if (auto value = get_if<int>(&data)) {
		cout << *value << endl;
	}
}

void testAny()
{
	any data; // aligned union for small types (<=32), dynamically allocated for bigger ones
	data = 2;
	data = "Vitaly";
	data = string("Many");

	cout << any_cast<string&>(data) << endl;
	try {
		cout << any_cast<int>(data) << endl;
	}
	catch (exception e) {
		cout << e.what() << endl; // bad cast
	}
}

int main()
{
	testOptional();
	testVariant();
	testAny();
	return 0;
}
