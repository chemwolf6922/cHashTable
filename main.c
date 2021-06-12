#include <stdio.h>
#include "crc.h"

int main(int argc, void** argv)
{
    uint32_t G = 0b10001000000100001;
    char test_data[] = "hahhchadafasdfasdfasdfasdfashahhsssssh";
    uint32_t crc = crc_calculate(G,test_data,sizeof(test_data));
    printf("0x%02x\n",crc);
    return 0;
}