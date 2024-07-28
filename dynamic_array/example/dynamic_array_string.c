#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string.h"

int main()
{
    StringBuilder* sb = init_string("Hello World");

    c_add(sb, '\n');

    str_add(sb, "Hello mello shmello!");

    str_delete_range(sb, 2, 3);

    printf("The message: %.*s\n", sb->length, sb->items);

    str_clear(sb);

    str_add(sb, "Some thing...");

    printf("The message: %.*s\n", sb->length, sb->items);

    free_string(sb);

    return 0;
}