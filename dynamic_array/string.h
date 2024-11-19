#pragma once

#include "dynamic_array.h"
#include <stdbool.h>

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
string_view sv_split(string_view *sv, char spliter);
bool sv_equal(string_view a, string_view b);
bool sv_start_with(string_view sv, const char* cstr);
string_view sv_trim_left(string_view sv);
string_view sv_trim_right(string_view sv);
string_view sv_trim(string_view sv);
bool sv_in(string_view sv, const char** arr, int count);
#define sv_in_carr(sv, arr) sv_in(sv, arr, arr_count(arr))

string_builder* sb_init(const char* src);
void sb_free(string_builder* sb);

void sb_add(string_builder* sb, string_view sv);

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
#include <ctype.h>

string_view sv_copy(string_view sv)
{
	return sv_from_cstr(sb_sprintf("%.*s", sv.count, sv.data));
}

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

string_view sv_split(string_view *sv, char spliter)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != spliter) {
        i += 1;
    }

    string_view result = sv_from_parts(sv->data, i);

    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data  += i + 1;
    } else {
        sv->count -= i;
        sv->data  += i;
    }

    return result;
}

bool sv_equal(string_view a, string_view b)
{
    if (a.count != b.count) {
        return false;
    } else {
        return memcmp(a.data, b.data, a.count) == 0;
    }
}

bool sv_start_with(string_view sv, const char* cstr)
{
	size_t cstr_count = strlen(cstr);
	if (sv.count < cstr_count) return false;

	return memcmp(sv.data, cstr, cstr_count) == 0;
}
string_view sv_trim_left(string_view sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i += 1;
    }

    return sv_from_parts(sv.data + i, sv.count - i);
}

string_view sv_trim_right(string_view sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
        i += 1;
    }

    return sv_from_parts(sv.data, sv.count - i);
}

string_view sv_trim(string_view sv)
{
    return sv_trim_right(sv_trim_left(sv));
}

bool sv_in(string_view sv, const char** arr, int count)
{
    for (int i = 0; i < count; i++) {
        if (sv_equal(sv_from_cstr(arr[i]), sv)) {
            return true;
        }
    }
    return false;
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

void sb_add(string_builder* sb, string_view sv)
{
    da_append_many(sb, sv.data, sv.count);
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