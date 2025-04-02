#include <stdio.h>

#define STRING_IMPLEMENTATION
#include "../string.h"

int main()
{
    string_builder sb = sb_init("Hello World");

    sb_add_c(&sb, '\n');

    sb_add_cstr(&sb, "Hello mello shmello!");

    sb_delete_range_cstr(&sb, 2, 3);

    printf("The message: %.*s\n", (int)sb.count, sb.items);

    sb_clear(&sb);

    sb_add_cstr(&sb, "Some thing...");

    printf("The message: %.*s\n", (int)sb.count, sb.items);

    sb_free(&sb);

    return 0;
}