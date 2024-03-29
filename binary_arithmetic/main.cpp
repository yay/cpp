#include <bitset>
#include <print>

typedef std::bitset<8> BYTE;

// Two's complement (swap 0 for 1, 1 for 0 and add 1).
void twosComplement()
{
    const BYTE p = 53;
    const BYTE n = -53;

    std::println("+53 -> {}", p.to_string());
    std::println("-53 -> {}", n.to_string());

    const char pc = 53;
    const char nc = -pc;

    std::println("{} -> {}", (int)pc, BYTE(pc).to_string());
    std::println("{} -> {}", (int)nc, BYTE(nc).to_string());

    const unsigned char puc = 53;
    const unsigned char nuc = -puc;

    std::println("{} -> {}", (unsigned int)puc, BYTE(puc).to_string());
    std::println("{} -> {}", (unsigned int)nuc, BYTE(nuc).to_string());

    // No matter if type is signed or unsigned, the bits always flip the same way
    // for negative numbers, but the interpretation of what those bits mean is of
    // course different for signed and unsigned types.
}

int main(int, char **)
{
    twosComplement();

    return 0;
}