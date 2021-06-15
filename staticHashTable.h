#ifndef __STATIC_HASH_TABLE_H
#define __STATIC_HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* This hash table is created once and does not accept new items */

#define HASH_KEY_VALUE(_key,_data) {_key,strlen(_key),_data,-1}

typedef void* static_hash_table_handle_t;

typedef struct{
    uint8_t* key;
    size_t key_len;
    void* data;
    int next_index;
}hash_table_item_t;

int static_hash_table_create(static_hash_table_handle_t* handle,hash_table_item_t* items,size_t len,bool create_cache);
int static_hash_table_delete(static_hash_table_handle_t handle);
void* static_hash_table_get(static_hash_table_handle_t handle,uint8_t* key,size_t key_len);
#define static_hash_table_get_str_key(_handle,_key) static_hash_table_get(_handle,_key,strlen(_key))


#endif