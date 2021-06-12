#ifndef __CRC_H
#define __CRC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* This lib is only capable of calculating up to 16bit crc */
uint32_t crc_calculate(uint32_t G,uint8_t* data,size_t len);

#endif