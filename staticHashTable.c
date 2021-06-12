#include "staticHashTable.h"
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "crc.h"

typedef struct
{
    uint32_t G;
    int bits;
    void *hash_map;
} static_hash_table_t;

typedef struct
{
    uint16_t value[256];
    uint32_t total;
} counter_8bit_t;

typedef struct
{
    float P_overlap;
    int bits;
} hash_function_score_t;

float calculate_P_overlap(counter_8bit_t *counter, int bits)
{
    if (counter->total == 0 || bits > 8)
    {
        return 1.0F;
    }
    uint32_t overlap_counter = 0;
    for (int i = 0; i < 1 << bits; i++)
    {
        uint32_t same_ending_counter = 0;
        for (int j = 0; j < 1 << (8 - bits); j++)
        {
            same_ending_counter += counter->value[i + (j * (1 << bits))];
        }
        if (same_ending_counter > 1)
        {
            overlap_counter += same_ending_counter;
        }
    }
    return ((float)overlap_counter) / ((float)counter->total);
}

uint8_t calculate_hash(uint8_t *key, size_t key_len, uint32_t G, int bits)
{
    uint8_t hash = (uint8_t)(crc_calculate(G, key, key_len) & ((1U << bits) - 1));
    return hash;
}

void evaluate_hash_function(uint32_t G, hash_table_item_t *items, size_t len, hash_function_score_t *score)
{
    counter_8bit_t counter;
    memset(&counter, 0, sizeof(counter));
    // count all the crc values
    for (size_t i = 0; i < len; i++)
    {
        counter.value[calculate_hash(items[i].key, items[i].key_len, G, 8)]++;
        counter.total++;
    }
    // calculate score
    hash_function_score_t temp_score = {
        .bits = 8,
    };
    while (true)
    {
        temp_score.P_overlap = calculate_P_overlap(&counter, temp_score.bits);
        if (temp_score.P_overlap != 0)
        {
            if (temp_score.bits == 8)
            {
                score->P_overlap = temp_score.P_overlap;
                score->bits = temp_score.bits;
            }
            break;
        }
        else
        {
            score->P_overlap = temp_score.P_overlap;
            score->bits = temp_score.bits;
            temp_score.bits--;
        }
    }
}

int static_hash_table_create(static_hash_table_handle_t *handle, hash_table_item_t *items, size_t len)
{
    // get the best crc hash functions for the given items
    hash_function_score_t best_score = {
        .bits = 8,
        .P_overlap = 1.0F,
    };
    uint32_t best_G = 0;
    int best_bits = 8;
    int target_bits = 32 - __builtin_clz((uint32_t)len);
    for (uint32_t G = 0b100000001; G < 0b111111111; G++)
    {
        hash_function_score_t score;
        evaluate_hash_function(G, items, len, &score);
        if (score.bits < best_score.bits ||
            (score.bits == best_score.bits && score.P_overlap < best_score.P_overlap))
        {
            best_bits = score.bits;
            best_G = G;
            best_score.bits = score.bits;
            best_score.P_overlap = score.P_overlap;
            if(best_bits == target_bits)
            {
                // early return
                break;
            }
        }
    }
    printf("G: 0x%03x, bits: %d\n",best_G,best_bits);
    // create the hash table
    

    *handle = NULL;
    return -1;
}

void *static_hash_table_get(static_hash_table_handle_t handle, uint8_t *key, size_t key_len)
{

    return NULL;
}