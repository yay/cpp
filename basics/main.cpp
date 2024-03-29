#include <vector>
#include <complex>
#include <algorithm>
#include <print>

void lambda();

int main()
{
    std::println("{}", "Hello, World!");

    const double PI = 3.1415926;

    double pi = 3.14159'26535'89793;

    char myGrade = 'A';

    // char b {245}; // error: constant expression evaluates to 245
    // which cannot be narrowed to type 'char'
    // char b = 245; // warning: implicit conversion from 'int' to 'char'
    // changes value from 245 to -11
    unsigned char byte = 245; // OK

    bool isHappy = true;

    int myAge = 39;
    // int i1 = 7.8;   // warning: implicit conversion from 'double' to 'int' changes value from 7.8 to 7
    // int i2 { 7.8 }; // error: double cannot be narrowed to int

    // Unfortunately, conversions that lose information, narrowing conversions,
    // such as double to int and int to char, are allowed and implicitly applied.
    // The problems caused by implicit narrowing conversions are a price paid for C compatibility.

    // A constant cannot be left uninitialized and a variable should only be left uninitialized
    // in extremely rare circumstances. Don’t introduce a name until you have a suitable value for it.

    long bigNumber = 1'000'000'000'000'000;

    std::complex<double> z{1.0, 2.0};

    float favNum = 3.141592; // normally accurate to 6 decimal numbers

    double otherFavNum = PI / 2;

    std::println("{}", sizeof(myGrade));
    std::println("{}", sizeof(isHappy));
    std::println("{}", sizeof(myAge));
    std::println("{}", sizeof(favNum));
    std::println("{}", sizeof(PI));

    int myNums[5] = {1, 2, 3, 5, 8};

    char myName[2][3] = {{'J', 'o', 'e'}, {'B', 'o', 'b'}};

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            std::print("{}", myName[j][i]);
        }
        std::println("");
    }

    lambda();

    return 0;
}

void lambda()
{

    struct isOdd
    {
        bool operator()(int x) // basically means that `isOdd` is callable
        {
            return x % 2 == 1;
        }
    };

    std::vector<int> col{1, 2, 3, 4, 5, 9, 8, 7, 6, 10, 15, 18, 20};

    int64_t numOdd = count_if(col.begin(), col.end(), isOdd());

    std::println("{} integers that are odd", numOdd);

    int64_t numEven = count_if(col.begin(), col.end(), [](int x)
                               { return !(x % 2); });

    // For comparison, in Kotlin this would be:
    //
    //     val numEven = col.count { it % 2 == 0 }
    //

    std::println("{} integers that are even", numEven);
}