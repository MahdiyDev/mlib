#include <stdio.h>

#define HT_IMPLEMENTATION
#include "../hash_table.h"

#define ht_insert_int(ht, key, value) do { int _v = value; ht_insert(ht, key, &_v, sizeof(int)); } while(0)
#define ht_search_int(ht, key) *(int*)ht_search(ht, key)

int main()
{
    hash_table* ht = ht_init();

    ht_insert_int(ht, "hello", 123);
    ht_insert_int(ht, "hello1", 13);
    ht_insert_int(ht, "hello2", 43);

    printf("[1] %d\n", ht_search_int(ht, "hello"));
    printf("[2] %d\n", ht_search_int(ht, "hello1"));
    printf("[3] %d\n", ht_search_int(ht, "hello2"));

    ht_free(ht);
}
