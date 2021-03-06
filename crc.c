#include "crc.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// return A MOD B
uint32_t GF2_MOD(uint32_t A,uint32_t B)
{
    int B_lz = __builtin_clz(B);
    while(true)
    {
        int A_remain_lz = A!=0?__builtin_clz(A):32;
        if(A_remain_lz > B_lz)
        {
            break;
        }
        A = A ^ (B << (B_lz - A_remain_lz));
    }
    return A;
}

uint32_t GF2_MOD_with_B_lz(uint32_t A,uint32_t B,int B_lz)
{
    while(true)
    {
        int A_remain_lz = A!=0?__builtin_clz(A):32;
        if(A_remain_lz > B_lz)
        {
            break;
        }
        A = A ^ (B << (B_lz - A_remain_lz));
    }
    return A;
}

uint32_t crc_T1(uint32_t A,uint32_t G,int G_lz,int X_bits)
{
    return GF2_MOD_with_B_lz(A<<X_bits,G,G_lz);
}

uint32_t crc_T2(uint32_t A,uint32_t G,int G_lz)
{
    return GF2_MOD_with_B_lz(A,G,G_lz);
}

uint32_t crc_calculate(uint32_t G,uint8_t* data,size_t len)
{
    int G_lz = __builtin_clz(G);
    // can only calculate up to crc16
    if(G_lz < 32 - 16 - 1)
    {
        return 0;
    }
    int X_bits = 8;
    uint32_t crc = 0;
    for(size_t i=0;i<len;i++)
    {
        crc = crc_T1(crc,G,G_lz,X_bits) ^ crc_T2(data[i],G,G_lz);
    }
    return crc;
}

int crc_init_crc8_cache(crc8_cache_t* cache,uint32_t G)
{
    int G_lz = __builtin_clz(G);
    int X_bits = 8;
    if(X_bits > 8)
    {
        return -1;
    }
    for(int i = 0;i<256;i++)
    {
        cache->T1[i] = crc_T1(i,G,G_lz,X_bits);
        cache->T2[i] = crc_T2(i,G,G_lz);
    }
    return 0;
}

uint8_t crc8_calculate_with_cache(crc8_cache_t* cache,uint8_t* data,size_t len)
{
    uint8_t crc = 0;
    for(size_t i=0;i<len;i++)
    {
        crc = cache->T1[crc] ^ cache->T2[data[i]];
    }
    return crc;
}

