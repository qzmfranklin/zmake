#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
    printf(
            "        Test endianness of the computer.\n"
            "    AA BB CC DD: big endian\n"
            "    DD CC BB AA: small endian\n"
            "\n"
            "Here it is:\n"
            );
    uint32_t a = 0xAABBCCDDlu;
    uint8_t *b = (uint8_t*)&a;
    for (int i = 0; i < 4; i++)
        printf("%02X ", b[i]);
    printf("\n");
    return 0;
}
