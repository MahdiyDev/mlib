#include "dynamic_array.h"

typedef struct {
    char* items;
    size_t capacity;
    size_t count;
} string_builder;

typedef struct {
    const char* data;
    size_t count;
} string_view;

string_view sv_from_cstr(const char* cstr);
string_view sv_from_parts(const char* data, size_t count);
string_view sb_to_sv(string_builder* sb);

string_builder* sb_init(const char* src);
void sb_free(string_builder* sb);

void sb_add_str(string_builder* sb, const char* src);

void sb_add_first_str(string_builder* sb, const char* src);
void sb_delete_range_str(string_builder* sb, int start, int end);

void sb_add_c(string_builder* sb, const char c);
void sb_add_first_c(string_builder* sb, const char c);
void sb_delete_c(string_builder* sb, int index);

void sb_clear(string_builder* sb);
char* sb_sprintf(const char *format, ...);

#ifdef STRING_IMPLEMENTATION
#include <stdarg.h>

string_view sv_from_parts(const char* data, size_t count)
{
    string_view sv;
    sv.count = count;
    sv.data = data;
    return sv;
}

string_view sv_from_cstr(const char* cstr)
{
    return sv_from_parts(cstr, strlen(cstr));
}

string_view sb_to_sv(string_builder* sb)
{
    sb_add_c(sb, '\0');
    return sv_from_parts((sb)->items, (sb)->count);
}

string_builder* sb_init(const char* src)
{
    string_builder* sb;
    da_init(sb);
    if (src != NULL) {
        sb_add_str(sb, src);
    }
    return sb;
}

void sb_add_str(string_builder* sb, const char* src)
{
    da_append_many(sb, src, strlen(src));
}

void sb_add_first_str(string_builder* sb, const char* src)
{
    da_prepend_many(sb, src, strlen(src));
}

void sb_delete_range_str(string_builder* sb, int start_index, int end_index)
{
    da_delete_range(sb, start_index, end_index);
}

void sb_clear(string_builder* sb)
{
	da_clear(sb);
}

void sb_add_c(string_builder* sb, const char c)
{
    da_append(sb, c);
}

void sb_add_first_c(string_builder* sb, const char c)
{
    da_prepend(sb, c);
}

void sb_delete_c(string_builder* sb, int index)
{
    da_delete(sb, index);
}

void sb_free(string_builder* sb)
{
    da_free(sb);
}

char* sb_sprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int n = vsnprintf(NULL, 0, format, args);
    va_end(args);

    DA_ASSERT(n >= 0);
    char* temp = (char*)malloc(n + 1);
    DA_ASSERT(temp != NULL && "Failed to allocate memory\n");

    va_start(args, format);
    vsnprintf(temp, n + 1, format, args);
    va_end(args);

    return temp;
}

#endif