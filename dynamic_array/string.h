#include "dynamic_array.h"

typedef struct StringBuilder {
    DefineDa(char);
} StringBuilder;

StringBuilder* init_string(const char* src);
void free_string(StringBuilder* sb);
void str_add(StringBuilder* sb, const char* src);
void str_delete_range(StringBuilder* sb, int start, int end);
void str_clear(StringBuilder* sb);
void c_add(StringBuilder* sb, const char c);
void c_delete(StringBuilder* sb, int index);

#ifdef STRING_BUILDER_IMPLEMENTATION
StringBuilder* init_string(const char* src)
{
    StringBuilder* sb;
    da_init(sb);
    if (src != NULL) {
        str_add(sb, src);
    }
    return sb;
}

void str_add(StringBuilder* sb, const char* src)
{
    da_append_many(sb, src, strlen(src));
}

void str_delete_range(StringBuilder* sb, int start_index, int end_index)
{
    da_delete_range(sb, start_index, end_index);
}

void str_clear(StringBuilder* sb)
{
	da_clear(sb);
}

void c_add(StringBuilder* sb, const char c)
{
    da_append(sb, c);
}

void c_delete(StringBuilder* sb, int index)
{
    da_delete(sb, index);
}

void free_string(StringBuilder* sb)
{
    da_free(sb);
}
#endif