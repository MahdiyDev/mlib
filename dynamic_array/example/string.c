#include <stdio.h>

#define STRING_IMPLEMENTATION
#include "../string.h"

int main()
{
    string_view sv = sv_from_cstr("Hello, World");

    printf("%s\n", sv.data);

    string_builder* sb = sb_init("String builder:\n");

    sb_add_str(sb, "    ");
    sb_add_str(sb, "capacity: ");
    sb_add_str(sb, sb_sprintf("%d\n", sb->capacity));
    sb_add_str(sb, "    ");
    sb_add_str(sb, "count: ");
    sb_add_str(sb, sb_sprintf("%d\n", sb->count));

    sv = sb_to_sv(sb);

    printf("%s\n", sv.data);

    sb_free(sb);

    return 0;
}