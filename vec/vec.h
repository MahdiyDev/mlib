#pragma once

// Typed dynamic array, "C++ template" style.
//
// Instead of one set of typeof() macros shared by every container, DEFINE_VEC(T, Name)
// stamps out a real struct plus real (static inline) functions for a single element
// type. Every generated function is type checked by the compiler, shows up in a
// debugger, and nothing here needs typeof / C23.
//
//     DEFINE_VEC(int, IntVec);
//
//     IntVec v = {0};
//     IntVec_init(&v);
//     IntVec_append(&v, 42);
//     int xs[] = { 1, 2, 3 };
//     IntVec_append_many(&v, xs, 3);
//     for (size_t i = 0; i < v.count; i++) printf("%d\n", v.items[i]);
//     IntVec_free(&v);
//
// The generated struct always has the fields { T* items; size_t count; size_t capacity; }
// so it stays layout compatible with the rest of mlib (e.g. string.h's string_builder).

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef DA_ASSERT
    #define DA_ASSERT assert
#endif
#ifndef DA_REALLOC
    #define DA_REALLOC realloc
#endif
#ifndef DA_FREE
    #define DA_FREE free
#endif
#ifndef DA_INIT_CAP
    #define DA_INIT_CAP 4
#endif

#define DEFINE_VEC(T, Name) \
    typedef struct { \
        T* items; \
        size_t count; \
        size_t capacity; \
    } Name; \
 \
    /* Ensure capacity is at least `want`, growing geometrically. */ \
    static inline void Name##_reserve(Name* v, size_t want) \
    { \
        if (want <= v->capacity) { \
            return; \
        } \
        size_t cap = v->capacity ? v->capacity : (size_t)DA_INIT_CAP; \
        while (cap < want) { \
            cap *= 2; \
        } \
        v->items = (T*)DA_REALLOC(v->items, cap * sizeof(T)); \
        DA_ASSERT(v->items != NULL && "Failed to allocate memory"); \
        v->capacity = cap; \
    } \
 \
    static inline void Name##_init_with_capacity(Name* v, size_t cap) \
    { \
        v->items = NULL; \
        v->count = 0; \
        v->capacity = 0; \
        if (cap > 0) { \
            Name##_reserve(v, cap); \
        } \
    } \
 \
    static inline void Name##_init(Name* v) \
    { \
        Name##_init_with_capacity(v, (size_t)DA_INIT_CAP); \
    } \
 \
    static inline void Name##_free(Name* v) \
    { \
        if (v == NULL) { \
            return; \
        } \
        DA_FREE(v->items); \
        v->items = NULL; \
        v->count = 0; \
        v->capacity = 0; \
    } \
 \
    static inline T* Name##_get(Name* v, size_t index) \
    { \
        DA_ASSERT(index < v->count && "index out of range"); \
        return &v->items[index]; \
    } \
 \
    static inline void Name##_append(Name* v, T item) \
    { \
        Name##_reserve(v, v->count + 1); \
        v->items[v->count++] = item; \
    } \
 \
    static inline void Name##_append_many(Name* v, const T* items, size_t n) \
    { \
        if (n == 0) { \
            return; \
        } \
        Name##_reserve(v, v->count + n); \
        memcpy(v->items + v->count, items, n * sizeof(T)); \
        v->count += n; \
    } \
 \
    static inline void Name##_prepend(Name* v, T item) \
    { \
        Name##_reserve(v, v->count + 1); \
        memmove(v->items + 1, v->items, v->count * sizeof(T)); \
        v->items[0] = item; \
        v->count++; \
    } \
 \
    static inline void Name##_prepend_many(Name* v, const T* items, size_t n) \
    { \
        if (n == 0) { \
            return; \
        } \
        Name##_reserve(v, v->count + n); \
        memmove(v->items + n, v->items, v->count * sizeof(T)); \
        memcpy(v->items, items, n * sizeof(T)); \
        v->count += n; \
    } \
 \
    /* Remove one element, shifting the tail left. */ \
    static inline void Name##_delete(Name* v, size_t index) \
    { \
        DA_ASSERT(index < v->count && "index out of range"); \
        memmove(v->items + index, v->items + index + 1, \
                (v->count - index - 1) * sizeof(T)); \
        v->count--; \
    } \
 \
    /* Remove the half-open range [start, end), shifting the tail left. */ \
    static inline void Name##_delete_range(Name* v, size_t start, size_t end) \
    { \
        DA_ASSERT(start <= end && end <= v->count && "invalid range"); \
        size_t n = end - start; \
        if (n == 0) { \
            return; \
        } \
        memmove(v->items + start, v->items + end, (v->count - end) * sizeof(T)); \
        v->count -= n; \
    } \
 \
    static inline void Name##_clear(Name* v) \
    { \
        v->count = 0; \
    } \
 \
    /* consume the trailing semicolon at the call site */ \
    struct Name##_semicolon_eater_
