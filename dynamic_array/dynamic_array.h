#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef DA_ASSERT
	#define DA_ASSERT assert
#endif
#ifndef DA_MALLOC
	#define DA_MALLOC malloc
#endif
#ifndef DA_CALLOC
	#define DA_CALLOC calloc
#endif
#ifndef DA_REALLOC
	#define DA_REALLOC realloc
#endif
#ifndef DA_FREE
	#define DA_FREE free
#endif

#define da_init_with_capacity(da, cap)                                                          \
    do {                                                                                        \
        typeof(da) _new_da = (typeof(da))DA_MALLOC(sizeof(typeof(*da)));                        \
        DA_ASSERT(_new_da != NULL && "Buy more RAM...");                                        \
        da = _new_da;                                                                           \
        (da)->capacity = cap;                                                                   \
        if (cap > 0) {                                                                          \
            (da)->items = (typeof((da)->items))DA_MALLOC(cap * (sizeof(typeof(*(da)->items)))); \
        }                                                                                       \
        (da)->count = 0;                                                                       \
    } while (0)

#define da_init(da) da_init_with_capacity(da, 1)

#define da_free(da)                \
    do {                           \
        if ((da)->items != NULL) { \
            DA_FREE((da)->items);  \
        }                          \
        DA_FREE(da);               \
    } while (0)

// Append an item to a dynamic array
#define da_append(da, item)                                                                                    \
    do {                                                                                                       \
        if ((da)->count >= (da)->capacity) {                                                                  \
            (da)->capacity = (da)->capacity * 2;                                                               \
            (da)->items = (typeof((da)->items))DA_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            DA_ASSERT((da)->items != NULL && "Buy more RAM...");                                               \
        }                                                                                                      \
        (da)->items[(da)->count++] = (item);                                                                  \
    } while (0)

#define da_append_many(da, new_items, new_items_count)                                                         \
    do {                                                                                                       \
        if ((da)->count + (new_items_count) > (da)->capacity) {                                               \
            if ((da)->capacity == 0) {                                                                         \
                (da)->capacity = 2;                                                                            \
            }                                                                                                  \
            while ((da)->count + (new_items_count) > (da)->capacity) {                                        \
                (da)->capacity *= 2;                                                                           \
            }                                                                                                  \
            (da)->items = (typeof((da)->items))DA_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            DA_ASSERT((da)->items != NULL && "Buy more RAM...");                                               \
        }                                                                                                      \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count) * sizeof(*(da)->items));             \
        (da)->count += (new_items_count);                                                                     \
    } while (0)

#define da_delete_range(da, start_index, end_index)                                                                             \
    do {                                                                                                                        \
        if ((da)->count > start_index && (da)->count > end_index) {                                                           \
            (da)->count -= (end_index - start_index);                                                                          \
            (da)->capacity -= (end_index - start_index);                                                                        \
            memmove(&(da)->items[start_index], &(da)->items[end_index], ((da)->capacity - start_index) * sizeof(*(da)->items)); \
            (da)->items = DA_REALLOC((da)->items, ((da)->capacity) * sizeof(*(da)->items));                                     \
        }                                                                                                                       \
    } while (0)

#define da_delete(da, index)                                                                                        \
    do {                                                                                                            \
        if ((da)->count > index) {                                                                                 \
            (da)->count--;                                                                                         \
            (da)->capacity--;                                                                                       \
            memmove(&(da)->items[index], &(da)->items[index + 1], ((da)->capacity - index) * sizeof(*(da)->items)); \
            (da)->items = DA_REALLOC((da)->items, ((da)->capacity) * sizeof(*(da)->items));                         \
        }                                                                                                           \
    } while (0)

#define da_clear(da)                                \
    do {                                            \
        da_delete_range(da, 0, ((da)->count - 1)); \
        (da)->count = 0;                           \
    } while (0)
