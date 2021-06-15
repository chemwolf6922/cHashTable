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
    int16_t *hash_map;
    hash_table_item_t* items;
    crc8_cache_t* crc_cache;
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

uint8_t calculate_hash(uint8_t *key, size_t key_len, static_hash_table_t* hash_table)
{
    if(hash_table->crc_cache != NULL)
    {
        return crc8_calculate_with_cache(hash_table->crc_cache,key,key_len) & ((1U<<hash_table->bits) -1);
    }
    else
    {
        return (uint8_t)(crc_calculate(hash_table->G, key, key_len) & ((1U << hash_table->bits) - 1));
    }
}

uint8_t calculate_hash_raw(uint8_t *key, size_t key_len, uint32_t G,int bits)
{
        return (uint8_t)(crc_calculate(G, key, key_len) & ((1U << bits) - 1));
}

void evaluate_hash_function(uint32_t G, hash_table_item_t *items, size_t len, hash_function_score_t *score)
{
    counter_8bit_t counter;
    memset(&counter, 0, sizeof(counter));
    // count all the crc values
    for (size_t i = 0; i < len; i++)
    {
        counter.value[calculate_hash_raw(items[i].key, items[i].key_len, G, 8)]++;
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

int static_hash_table_create(static_hash_table_handle_t *handle, hash_table_item_t *items, size_t len,bool create_cache)
{
    static_hash_table_t* hash_table = (static_hash_table_t*)malloc(sizeof(static_hash_table_t));
    if(hash_table == NULL)
    {
        *handle = NULL;
        return -1;
    }
    // get the best crc hash functions for the given items
    hash_function_score_t best_score = {
        .bits = 8,
        .P_overlap = 1.0F,
    };
    uint32_t best_G = 0;
    int best_bits = 8;
    int target_bits = 32 - __builtin_clz((uint32_t)len);
    for (uint32_t G = 0b1; G < 0b111111111; G++)
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
    // create the hash table
    hash_table->G = best_G;
    hash_table->bits = best_bits;
    hash_table->items = items;
    // init crc cache
    if(create_cache)
    {
        hash_table->crc_cache = (crc8_cache_t*)malloc(sizeof(crc8_cache_t));
        if(hash_table->crc_cache == NULL)
        {
            free(hash_table);
            *handle = NULL;
            return -1;
        }
        crc_init_crc8_cache(hash_table->crc_cache,hash_table->G);
    }
    
    // init hash map
    size_t hash_map_size = sizeof(int16_t)*(1U<<hash_table->bits);
    hash_table->hash_map = (int16_t*)malloc(hash_map_size);
    if(hash_table->hash_map == NULL)
    {
        free(hash_table->crc_cache);
        free(hash_table);
        *handle = NULL;
        return -1;
    }
    // set initial values to -1
    memset(hash_table->hash_map,0xFF,hash_map_size);
    for(size_t i = 0; i < len; i++)
    {
        uint8_t hash_value = calculate_hash(items[i].key,items[i].key_len,hash_table);
        if(hash_table->hash_map[hash_value]!=-1)
        {
            hash_table_item_t* item = &items[hash_table->hash_map[hash_value]];
            while(item->next_index != -1)
            {
                item = &items[item->next_index];
            }
            item->next_index = i;
        }
        else
        {
            hash_table->hash_map[hash_value] = i;
        }
    }
    *handle = hash_table;
    return 0;
}

int static_hash_table_delete(static_hash_table_handle_t handle)
{
    if(handle == NULL)
    {
        return -1;
    }
    static_hash_table_t* hash_table = (static_hash_table_t*)handle;
    free(hash_table->hash_map);
    free(hash_table->crc_cache);
    free(hash_table);
    return 0;
}

void *static_hash_table_get(static_hash_table_handle_t handle, uint8_t *key, size_t key_len)
{
    if(handle == NULL)
    {
        return NULL;
    }
    static_hash_table_t* hash_table = (static_hash_table_t*)handle;
    uint8_t hash_value = calculate_hash(key,key_len,hash_table);
    int16_t index = hash_table->hash_map[hash_value];
    if(index == -1)
    {
        return NULL;
    }
    hash_table_item_t* item = &hash_table->items[index];
    while(true)
    {
        // fully compare the key
        if(key_len == item->key_len)
        {
            if(memcmp(key,item->key,item->key_len)==0)
            {
                return item->data;
            }
        }
        // goto next if there is any
        if(item->next_index == -1)
        {
            break;
        }
        else
        {
            item = &hash_table->items[item->next_index];
        }
    }
    return NULL;
}