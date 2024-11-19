#include <stdio.h>

#include "../dynamic_array.h"

typedef struct StringBuilder {
    char* items;
    size_t capacity;
    size_t count;
} ArrayOfNumbers;

int main()
{
    ArrayOfNumbers* num_array;

    da_init(num_array);

    for (int i = 0; i < 5; i++) {
        da_append(num_array, i);
    }

    int numbers[] = { 23, 47, 25 };

    da_append_many(num_array, numbers, 3);

    for (int i = 0; i < num_array->count; i++) {
        printf("num_array[%d]: %d\n", i, num_array->items[i]);
    }

    da_delete(num_array, 7);
    da_delete(num_array, 5);

    da_delete_range(num_array, 0, 2);

    for (int i = 0; i < num_array->count; i++) {
        printf("after delete num_array[%d]: %d\n", i, num_array->items[i]);
    }

    da_clear(num_array);

    da_append(num_array, 10);

    for (int i = 0; i < num_array->count; i++) {
        printf("after clear num_array[%d]: %d\n", i, num_array->items[i]);
    }

    da_free(num_array);

    return 0;
}