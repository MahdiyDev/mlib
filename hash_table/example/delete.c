#include <stdio.h>

#define HT_IMPLEMENTATION
#include "../hash_table.h"

struct vector { int x; int y; };

#define ht_insert_vec_int(ht, key, value) ht_insert_generic(ht, struct vector, key, int, value)
#define ht_search_vec_int(ht, key)        ht_search_generic(ht, struct vector, key, int)
#define ht_delete_vec_int(ht, key)        ht_delete_generic(ht, struct vector, key)

int main()
{
    hash_table* ht = ht_init();

    struct vector vec1 = {1, 2};
    ht_insert_vec_int(ht, vec1, 123);

    struct vector vec2 = {5, 2};
    ht_insert_vec_int(ht, vec2, 13);
    
    struct vector vec3 = {5, 3};
    ht_insert_vec_int(ht, vec3, 43);

    ht_delete_vec_int(ht, vec1);

    int* result1 = ht_search_vec_int(ht, vec1);
    if (result1) {
        printf("[1] %d\n", *result1);
    } else {
        printf("[1] (null)\n");
    }

    int* result2 = ht_search_vec_int(ht, vec2);
    if (result2) {
        printf("[2] %d\n", *result2);
    } else {
        printf("[2] (null)\n");
    }

    int* result3 = ht_search_vec_int(ht, vec3);
    if (result3) {
        printf("[3] %d\n", *result3);
    } else {
        printf("[3] (null)\n");
    }

    ht_free(ht);
}
