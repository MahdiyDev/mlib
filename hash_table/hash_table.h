#pragma once

#include <stddef.h>

// Specific prime number for reduce collisions 
#define HT_INITIAL_BASE_SIZE 53

typedef struct {
    char* key;
    char* value;
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

void  ht_insert(hash_table* ht, const char* key, const char* value);
char* ht_search(hash_table* ht, const char* key);
void  ht_delete(hash_table* ht, const char* key);

#ifdef HT_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define PRIME_IMPLEMENTATION
#include "prime.h"

void ht_resize_up(hash_table* ht);
void ht_resize_down(hash_table* ht);

//
// Hash formula:
//      h_b(<a_0, a_1, ..., a_(n-1)>) = (SUM_{j=0 to n-1} (a_j * b^j)) mod p
//
size_t ht_hash(const char* s, const int a, const int m)
{
    unsigned long hash = 0;
    size_t len_s = strlen(s);
    for (size_t i = 0; i < len_s; i++) {
        hash += pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return hash;
}


//
// Double hashing:
//      h(k, i) = (h_1(k) + i * h_2(k)) mod m
//      h_1(k) = k mod m
//      h_2(k) = 1 + (k mod m')
//      m' = m - 2
//
#define HT_PRIME_1 31
#define HT_PRIME_2 37

size_t ht_get_hash(const char* s, const size_t num_buckets, const size_t attempt)
{
    size_t hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    size_t hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

ht_item* ht_new_item(const char* k, const char* v)
{
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
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

void ht_free(hash_table* ht)
{
    for (int i = 0; i < ht->capacity; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

ht_item HT_DELETED_ITEM = {NULL, NULL};

void ht_insert(hash_table* ht, const char* key, const char* value)
{
    int load = ht->count * 100 / ht->capacity;
    if (load > 70) {
        ht_resize_up(ht);
    }

    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->capacity, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        index = ht_get_hash(item->key, ht->capacity, i);
        cur_item = ht->items[index];
        i++;
    } 
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(hash_table* ht, const char* key)
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
    int load = ht->count * 100 / ht->capacity;
    if (load < 10) {
        ht_resize_down(ht);
    }

    int index = ht_get_hash(key, ht->capacity, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->capacity, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--;
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
            ht_insert(new_ht, item->key, item->value);
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
