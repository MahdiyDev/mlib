#include <stdio.h>

#define HT_IMPLEMENTATION
#include "../hash_table.h"

struct vector { int x; int y; };

#define ht_insert_vec_int(ht, key, value) do { \
    int _v = value; \
    struct vector _vk = key; \
    char* _key = ht_to_char(&_vk, sizeof(_vk)); \
    ht_insert(ht, _key, &_v, sizeof(int)); \
    free(_key); \
} while(0)
#define ht_search_vec_int(ht, key) ({ \
    struct vector _vk = key; \
    char* _key = ht_to_char(&_vk, sizeof(_vk)); \
    int _r = *(int*)ht_search(ht, _key); \
    free(_key); \
    _r; /* return value */ \
})

int main()
{
    struct vector v = {12, 21};
    char* r = ht_to_char(&v, sizeof(v));

    printf("bin data: { ");
    for (int i = 0; i < sizeof(v); i++) {
        printf("%d ", r[i]);
    }
    printf("}\n");

    free(r);

    hash_table* ht = ht_init();

    struct vector vec1 = {1, 2};
    ht_insert_vec_int(ht, vec1, 123);

    struct vector vec2 = {5, 2};
    ht_insert_vec_int(ht, vec2, 13);
    
    struct vector vec3 = {5, 3};
    ht_insert_vec_int(ht, vec3, 43);

    printf("[1] %d\n", ht_search_vec_int(ht, vec1));
    printf("[2] %d\n", ht_search_vec_int(ht, vec2));
    printf("[3] %d\n", ht_search_vec_int(ht, vec3));

    ht_free(ht);
}
