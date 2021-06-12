#ifndef __STATIC_HASH_TABLE_H
#define __STATIC_HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

/* This hash table is created once and does not accept new items */

typedef void* static_hash_table_handle_t;

typedef struct{
    uint8_t* key;
    size_t key_len;
    void* data;
}hash_table_item_t;

int static_hash_table_create(static_hash_table_handle_t* handle,hash_table_item_t* items,size_t len);
void* static_hash_table_get(static_hash_table_handle_t handle,uint8_t* key,size_t key_len);


#endif