#include <stdio.h>

#define HT_IMPLEMENTATION
#include "../hash_table.h"

struct vector { int x; int y; };

#define ht_insert_vec_int(ht, key, value) ht_insert_generic(ht, struct vector, key, int, value)
#define ht_search_vec_int(ht, key) ht_search_generic(ht, struct vector, key, int)

int main()
{
    struct vector v = {12, 21};
    char r[sizeof(struct vector)];
    ht_to_char(r, &v, sizeof(v));

    printf("bin data: { ");
    for (int i = 0; i < sizeof(v); i++) {
        printf("%d ", r[i]);
    }
    printf("}\n");

    hash_table* ht = ht_init();

    struct vector vec1 = {1, 2};
    ht_insert_vec_int(ht, vec1, 123);

    struct vector vec2 = {5, 2};
    ht_insert_vec_int(ht, vec2, 13);
    
    struct vector vec3 = {5, 3};
    ht_insert_vec_int(ht, vec3, 43);

    printf("[1] %d\n", *ht_search_vec_int(ht, vec1));
    printf("[2] %d\n", *ht_search_vec_int(ht, vec2));
    printf("[3] %d\n", *ht_search_vec_int(ht, vec3));

    ht_free(ht);
}
