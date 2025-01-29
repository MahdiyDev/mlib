#pragma once

#include <stddef.h>

typedef struct {
    char* key;
    void* value;
    int value_size;
} ht_item;

typedef struct {
    size_t capacity;
    size_t count;
    ht_item** items;

    size_t base_capacity;
} hash_table;

hash_table* ht_init();
hash_table* ht_init_with_capacity(const int base_capacity);
void ht_free(hash_table* ht);

void  ht_insert(hash_table* ht, const char* key, void* value, int value_size);
void* ht_search(hash_table* ht, const char* key);
void  ht_delete(hash_table* ht, const char* key);

void ht_to_char(char* cstr, void* data, int data_size);

#define ht_insert_generic_value(ht, key, value_type, value) do { value_type _v = value; ht_insert(ht, key, &_v, sizeof(value_type)); } while(0)
#define ht_search_generic_value(ht, key, value_type) (value_type*)ht_search(ht, key)
#define ht_delete_generic_value(ht, key) do { ht_delete(ht, key); } while(0)

#define ht_insert_generic_key(ht, key_type, key, value_type, value) do { \
    value_type _v = value; \
    key_type _vk = key; \
    char _key[sizeof(key_type)]; \
    ht_to_char(_key, &_vk, sizeof(_vk)); \
    ht_insert(ht, _key, &_v, sizeof(value_type)); \
} while(0)
#define ht_search_generic_key(ht, key_type, key, value_type) ({ \
    key_type _vk = key; \
    char _key[sizeof(key_type)]; \
    ht_to_char(_key, &_vk, sizeof(_vk)); \
    (value_type*)ht_search(ht, _key); \
})
#define ht_delete_generic_key(ht, key_type, key) { \
    key_type _vk = key; \
    char _key[sizeof(key_type)]; \
    ht_to_char(_key, &_vk, sizeof(_vk)); \
    ht_delete(ht, _key); \
} while(0)

#ifdef HT_IMPLEMENTATION
#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRIME_IMPLEMENTATION
#include "prime.h"
#include "hash_table.h"

// Specific prime number for reduce collisions 
#define HT_INITIAL_BASE_SIZE 53

void ht_resize_up(hash_table* ht);
void ht_resize_down(hash_table* ht);

void ht_to_char(char* cstr, void* data, int data_size)
{
    if (!data || data_size <= 0) {
        cstr = NULL;
    }

    memcpy(cstr, data, data_size);
    cstr[data_size - 1] = '\0';
}

//
// Hash formula:
//      h_b(<a_0, a_1, ..., a_(n-1)>) = (SUM_{j=0 to n-1} (a_j * b^j)) mod p
//
size_t ht_hash(const char* s, const int a, const int m)
{
    unsigned long hash = 0;
    unsigned long a_pow = 1;  // a^0 = 1
    size_t len_s = strlen(s);

    for (size_t i = 0; i < len_s; i++) {
        hash += s[len_s - (i + 1)] * a_pow;
        hash = hash % m;
        a_pow = (a_pow * a) % m;  // Compute a^(i+1) % m
    }

    return hash;
}


//
// Double hashing:
//      h(k, i) = (h_1(k) + i * h_2(k)) mod m
//      h_1(k) = k mod m
//      h_2(k) = m' - (k % m')
//
#define HT_PRIME_1 31
#define HT_PRIME_2 37

size_t ht_get_hash(const char* s, const size_t num_buckets, const size_t attempt)
{
    size_t hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    size_t hash_b = ht_hash(s, HT_PRIME_2, num_buckets - 2); // m' = num_buckets - 2
    hash_b = (num_buckets - 1) - hash_b;  // Ensure non-zero secondary hash
    return (hash_a + attempt * hash_b) % num_buckets;
}


ht_item* ht_new_item(const char* k, void* v, int value_size)
{
    ht_item* i = malloc(sizeof(ht_item));
    if (!i) {
        perror("Failed to allocate memory for ht_item");
        exit(EXIT_FAILURE);
    }

    i->key = strdup(k);  // Duplicate the key
    if (!i->key) {
        perror("Failed to allocate memory for key");
        free(i);
        exit(EXIT_FAILURE);
    }

    i->value = malloc(value_size);
    if (!i->value) {
        perror("Failed to allocate memory for value");
        free(i->key);
        free(i);
        exit(EXIT_FAILURE);
    }

    memcpy(i->value, v, value_size);
    i->value_size = value_size;
    return i;
}

hash_table* ht_init_with_capacity(const int base_capacity)
{
    hash_table* ht = malloc(sizeof(hash_table));
    ht->base_capacity = base_capacity;

    ht->capacity = next_prime(ht->base_capacity);

    ht->count = 0;
    ht->items = calloc((size_t)ht->capacity, sizeof(ht_item*));
    return ht;
}

hash_table* ht_init()
{
    return ht_init_with_capacity(HT_INITIAL_BASE_SIZE);
}

void ht_del_item(ht_item* i)
{
    free(i->key);
    free(i->value);
    free(i);
}

ht_item HT_DELETED_ITEM = {NULL, NULL, 0};

void ht_free(hash_table* ht)
{
    for (int i = 0; i < ht->capacity; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

void ht_insert(hash_table* ht, const char* key, void* value, int value_size)
{
    int load = ht->count * 100 / ht->capacity;
    if (load > 70) {
        ht_resize_up(ht);
    }

    ht_item* new_item = ht_new_item(key, value, value_size);
    int index = ht_get_hash(new_item->key, ht->capacity, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        index = ht_get_hash(new_item->key, ht->capacity, i);
        cur_item = ht->items[index];
        i++;
    } 
    ht->items[index] = new_item;
    ht->count++;
}

void* ht_search(hash_table* ht, const char* key)
{
    int index = ht_get_hash(key, ht->capacity, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->capacity, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

void ht_delete(hash_table* ht, const char* key)
{
    int index = ht_get_hash(key, ht->capacity, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item != NULL) {
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
            ht_del_item(item);
            ht->items[index] = &HT_DELETED_ITEM;
            ht->count--;
            break;
        }
        index = ht_get_hash(key, ht->capacity, i);
        item = ht->items[index];
        i++;
    }

    int load = ht->count * 100 / ht->capacity;
    if (load < 10 && ht->capacity > HT_INITIAL_BASE_SIZE) {
        ht_resize_down(ht);
    }
}

void ht_resize(hash_table* ht, const int base_size)
{
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    hash_table* new_ht = ht_init_with_capacity(base_size);
    for (int i = 0; i < ht->capacity; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value, item->value_size);
        }
    }

    ht->base_capacity = new_ht->base_capacity;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->capacity;
    ht->capacity = new_ht->capacity;
    new_ht->capacity = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_free(new_ht);
}

void ht_resize_up(hash_table* ht)
{
    const int new_size = ht->base_capacity * 2;
    ht_resize(ht, new_size);
}

void ht_resize_down(hash_table* ht)
{
    const int new_size = ht->base_capacity / 2;
    ht_resize(ht, new_size);
}
#endif // HT_IMPLEMENTATION
