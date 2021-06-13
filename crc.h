#ifndef __CRC_H
#define __CRC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* This lib is only capable of calculating up to 16bit crc */
uint32_t crc_calculate(uint32_t G,uint8_t* data,size_t len);

/* CRC8 with cache */
typedef struct{
    uint8_t T1[256];
    uint8_t T2[256];
}crc8_cache_t;
int crc_init_crc8_cache(crc8_cache_t* cache,uint32_t G);
uint8_t crc8_calculate_with_cache(crc8_cache_t* cache,uint8_t* data,size_t len);

#endif