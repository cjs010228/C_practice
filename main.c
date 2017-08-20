#include <stdio.h>

int main(void)
{
    /* Bit mask: Change specific bit to be 1 */
    unsigned char output = 0, input = 0;
    output = input | 0x80;
    printf("%2x\n", output);

    /* Bit mask: Change specific bit to be 0 */
    output = input & 0x7F;
    printf("%2x\n", output);

    return 0;
}