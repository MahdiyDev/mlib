#include <stdio.h>
#define HT_IMPLEMENTATION
#include "../hash_table.h"

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
                    ht_insert(ht, key, val);
                }
            }
        }
    }
    printf("--- insert done! ---\n");

    ht_insert(ht, "cat", "ui ua ui ua");
    ht_insert(ht, "dog", "what a dog doing?");
    ht_insert(ht, "snake", "snake snakes");
    ht_insert(ht, "man", "I like money!");

    printf("cat: %s\n", ht_search(ht, "cat"));
    printf("dog: %s\n", ht_search(ht, "dog"));
    printf("snake: %s\n", ht_search(ht, "snake"));
    printf("man: %s\n", ht_search(ht, "man"));

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
                    char* val2 = ht_search(ht, key);
                    if (strcmp(val, val2) == 0){ tr_val++;}
                    else {printf("%s\n", val2);}
                }
            }
        }
    }
    printf("--- value checking done! ---\n");
    printf("true value count: %d\n", tr_val);
    

    ht_free(ht);
    return 0;
}