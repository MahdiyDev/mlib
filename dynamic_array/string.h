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
void sb_str_add(string_builder* sb, const char* src);
void sb_c_add(string_builder* sb, const char c);
void sb_str_delete_range(string_builder* sb, int start, int end);
void sb_c_delete(string_builder* sb, int index);
void sb_clear(string_builder* sb);
void sb_sprintf(string_builder* sb, const char *format, ...);

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
    sb_c_add(sb, '\0');
    return sv_from_parts((sb)->items, (sb)->count);
}

string_builder* sb_init(const char* src)
{
    string_builder* sb;
    da_init(sb);
    if (src != NULL) {
        sb_str_add(sb, src);
    }
    return sb;
}

void sb_str_add(string_builder* sb, const char* src)
{
    da_append_many(sb, src, strlen(src));
}

void sb_str_delete_range(string_builder* sb, int start_index, int end_index)
{
    da_delete_range(sb, start_index, end_index);
}

void sb_clear(string_builder* sb)
{
	da_clear(sb);
}

void sb_c_add(string_builder* sb, const char c)
{
    da_append(sb, c);
}

void sb_c_delete(string_builder* sb, int index)
{
    da_delete(sb, index);
}

void sb_free(string_builder* sb)
{
    da_free(sb);
}

void sb_sprintf(string_builder* sb, const char *format, ...)
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

    sb_str_add(sb, temp);
    free(temp);
}

#endif