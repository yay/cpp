#include <iostream>
#include <bitset>

typedef std::bitset<8> BYTE;

// Two's complement (swap 0 for 1, 1 for 0 and add 1).
void twosComplement()
{
    const BYTE p = 53;
    const BYTE n = -53;

    std::cout << "+53 -> " << p << "\n";
    std::cout << "-53 -> " << n << "\n";


    const char pc = 53;
    const char nc = -pc;

    std::cout << (int)pc << " -> " << BYTE(pc) << "\n";
    std::cout << (int)nc << " -> " << BYTE(nc) << "\n";


    const unsigned char puc = 53;
    const unsigned char nuc = -puc;

    std::cout << (unsigned int)puc << " -> " << BYTE(puc) << "\n";
    std::cout << (unsigned int)nuc << " -> " << BYTE(nuc) << "\n";

    // No matter if type is signed or unsigned, the bits always flip the same way
    // for negative numbers, but the interpretation of what those bits mean is of
    // course different for signed and unsigned types.
}

int main(int, char**) {
    twosComplement();

	return 0;
}