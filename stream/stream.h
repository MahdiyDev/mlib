#pragma once

#include "../list/list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef STREAM_MALLOC
	#define STREAM_MALLOC malloc
#endif
#ifndef STREAM_REALLOC
	#define STREAM_REALLOC realloc
#endif
#ifndef STREAM_ASSERT
	#define STREAM_ASSERT assert
#endif

#define DefineStream \
    int length;      \
    LazyList* list

#define StreamItem(stream) typeof(stream->data[0])
typedef struct LazyItem LazyItem;
typedef void* (*LazyFunc)(void* stream, void* each);

typedef enum FuncType {
    STREAM_MAP = 0,
    STREAM_FOREACH,
    STREAM_FILTER,
} FuncType;

typedef struct LazyItem {
    FuncType func_type;
    LazyFunc lazy_func;

    LazyItem* next;
} LazyItem;

typedef struct LazyList {
    int length;
    LazyItem* head;
} LazyList;

#define create_stream(stream, data_len, _data)                                              \
    do {                                                                                    \
        typeof(stream) new_stream = (typeof(stream))STREAM_MALLOC(sizeof(typeof(*stream))); \
        STREAM_ASSERT(new_stream != NULL && "Buy more RAM...");                             \
        stream = new_stream;                                                                \
        (stream)->length = data_len;                                                        \
        (stream)->data = STREAM_MALLOC(data_len * sizeof(typeof(_data[0])));                \
        create_list((stream)->list);                                                        \
        for (int _i = 0; _i < (stream)->length; _i++) {                                     \
            (stream)->data[_i] = _data[_i];                                                 \
        }                                                                                   \
    } while (0)

#define close_stream(stream)     \
    do {                         \
        free(stream->data);      \
        free_list(stream->list); \
        free(stream);            \
    } while (0)

#define collect(stream)                                                                                                 \
    do {                                                                                                                \
        StreamItem(stream)* _value = NULL;                                                                              \
        for (int _j = 0; _j < stream->length; _j++) {                                                                   \
            _value = &stream->data[_j];                                                                                 \
            LazyItem* currentItem = stream->list->head;                                                                 \
            bool _b = true;                                                                                            \
            for (int _i = 0; _i < stream->list->length; _i++) {                                                         \
                if (currentItem->func_type == STREAM_MAP) {                                                             \
                    _value = currentItem->lazy_func(stream, _value);                                                    \
                }                                                                                                       \
                if (currentItem->func_type == STREAM_FOREACH) {                                                         \
                    currentItem->lazy_func(stream, _value);                                                             \
                }                                                                                                       \
                if (currentItem->func_type == STREAM_FILTER) {                                                          \
                    _b = (bool)currentItem->lazy_func(stream, _value);                                                  \
                    if (!_b) {                                                                                          \
                        memmove(                                                                                        \
                            &stream->data[_j],                                                                          \
                            &stream->data[_j + 1],                                                                      \
                            (stream->length - _j - 1) * sizeof(StreamItem(stream)));                                    \
                        stream->data = STREAM_REALLOC(stream->data, (stream->length - 1) * sizeof(StreamItem(stream))); \
                        stream->length--;                                                                               \
                        _j--;                                                                                           \
                        break;                                                                                          \
                    }                                                                                                   \
                }                                                                                                       \
                if (currentItem->next == NULL) {                                                                        \
                    break;                                                                                              \
                }                                                                                                       \
                currentItem = currentItem->next;                                                                        \
            }                                                                                                           \
        }                                                                                                               \
    } while (0)

#define map(stream, func) \
    list_append((stream)->list, ((LazyItem) { .func_type = STREAM_MAP, .lazy_func = (LazyFunc)func }))

#define for_each(stream, func) \
    list_append((stream)->list, ((LazyItem) { .func_type = STREAM_FOREACH, .lazy_func = (LazyFunc)func }))

#define filter(stream, func) \
    list_append((stream)->list, ((LazyItem) { .func_type = STREAM_FILTER, .lazy_func = (LazyFunc)func }))

#define _map(func) \
    (_value = func(stream, _value))

#define _for_each(func) \
    (func(stream, _value))

#define _filter(func)                                                                                   \
    (func(stream, _value) ? ({                                                                          \
        memmove(                                                                                        \
            &stream->data[_i],                                                                          \
            &stream->data[_i + 1],                                                                      \
            (stream->length - _i - 1) * sizeof(StreamItem(stream)));                                    \
        stream->data = STREAM_REALLOC(stream->data, (stream->length - 1) * sizeof(StreamItem(stream))); \
        _value = &stream->data[_i];                                                                     \
        stream->length--;                                                                               \
        _i--;                                                                                           \
        continue;                                                                                       \
    })                                                                                                  \
                          : 0)

#define array_len(arr) (sizeof(arr) / sizeof(typeof(arr[0])))
