#include <stdio.h>
#define HT_IMPLEMENTATION
#include "../hash_table.h"

#define ht_insert_cstr(ht, key, value) do { char* _v = value; ht_insert(ht, key, &_v, sizeof(char*)); } while(0)
#define ht_search_cstr(ht, key) *(char**)ht_search(ht, key)

int main()
{
    hash_table* ht = ht_init();

    printf("--- insert start! ---\n");

    char key[5];
    char val[5];
    for (int i = 'a'; i <= 'z'; i++) {
        for (int j = 'a'; j <= 'z'; j++) {
            for (int k = 'a'; k <= 'z'; k++) {
                for (int p = 'a'; p <= 'z'; p++) {
                    key[0] = i;
                    key[1] = j;
                    key[2] = k;
                    key[3] = p;
                    key[4] = '\0';
                    val[0] = p;
                    val[1] = k;
                    val[2] = j;
                    val[3] = i;
                    val[4] = '\0';
                    ht_insert_cstr(ht, key, val);
                }
            }
        }
    }
    printf("--- insert done! ---\n");

    ht_insert_cstr(ht, "cat", "ui ua ui ua");
    ht_insert_cstr(ht, "dog", "what a dog doing?");
    ht_insert_cstr(ht, "snake", "snake snakes");
    ht_insert_cstr(ht, "man", "I like money!");

    printf("cat: %s\n", ht_search_cstr(ht, "cat"));
    printf("dog: %s\n", ht_search_cstr(ht, "dog"));
    printf("snake: %s\n", ht_search_cstr(ht, "snake"));
    printf("man: %s\n", ht_search_cstr(ht, "man"));

    printf("--- capacity: %zu ---\n", ht->capacity);
    printf("--- count: %zu ---\n", ht->count);

    printf("--- value checking start! ---\n");
    int tr_val = 0;
    for (int i = 'a'; i <= 'z'; i++) {
        for (int j = 'a'; j <= 'z'; j++) {
            for (int k = 'a'; k <= 'z'; k++) {
                for (int p = 'a'; p <= 'z'; p++) {
                    key[0] = i;
                    key[1] = j;
                    key[2] = k;
                    key[3] = p;
                    key[4] = '\0';
                    val[0] = p;
                    val[1] = k;
                    val[2] = j;
                    val[3] = i;
                    val[4] = '\0';
                    char* val2 = ht_search_cstr(ht, key);
                    if (strcmp(val, val2) == 0) tr_val++;
                }
            }
        }
    }
    printf("--- value checking done! ---\n");
    printf("true value count: %d\n", tr_val);

    ht_free(ht);
    return 0;
}