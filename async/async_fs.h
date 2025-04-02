#pragma once

#include "../dynamic_array/dynamic_array.h"

#ifndef ASYNC_IMPLEMENTATION
    #define ASYNC_IMPLEMENTATION
#endif // ASYNC_IMPLEMENTATION
#include "async.h"

#include <stdio.h>

#ifndef ASYNC_FS
    #define ASYNC_FS
#endif // ASYNC_FS

typedef struct FsTextDa {
    FILE* file;
    size_t capacity;
    size_t count;
    char* items;
} FsTextDa;

typedef struct FsDataDa {
    FILE* file;
    size_t capacity;
    size_t count;
    unsigned char* items;
} FsDataDa;

ASYNC_FS Task* async_fs_init(AsyncState* state, task_update update, const char* file_name, const char* mode, int priority);
ASYNC_FS void async_fs_close(Task* task);

ASYNC_FS Task* async_read_file_text(AsyncState* state, const char* file_name);
ASYNC_FS Task* async_read_file_data(AsyncState* state, const char* file_name);
ASYNC_FS Task* async_write_file_text(AsyncState* state, const char* file_name, const void* data, size_t size);
ASYNC_FS Task* async_write_file_data(AsyncState* state, const char* file_name, const void* data, size_t size);
ASYNC_FS Task* async_read_file_text_priority(AsyncState* state, const char* file_name, int priority);
ASYNC_FS Task* async_read_file_data_priority(AsyncState* state, const char* file_name, int priority);
ASYNC_FS Task* async_write_file_text_priority(AsyncState* state, const char* file_name, const void* data, size_t size, int priority);
ASYNC_FS Task* async_write_file_data_priority(AsyncState* state, const char* file_name, const void* data, size_t size, int priority);

ASYNC_FS void async_read_file_text_update(Task* task);
ASYNC_FS void async_read_file_data_update(Task* task);
ASYNC_FS void async_write_file_text_update(Task* task);
ASYNC_FS void async_write_file_data_update(Task* task);

#ifdef ASYNC_FS_IMPLEMENTATION

Task* async_fs_init(AsyncState* state, task_update update, const char* file_name, const char* mode, int priority)
{
    Task* task = async_func_priority(state, NULL, priority);
    FsTextDa* resolve = (FsTextDa*)get_task_resolve(task);
    da_init(resolve);
    resolve->file = fopen(file_name, mode);
    if (resolve->file == NULL)
        return NULL;
    set_resolve(task, resolve);
    set_task_update(state, task, update);
    return task;
}

Task* async_read_file_text_priority(AsyncState* state, const char* file_name, int priority)
{
    return async_fs_init(state, async_read_file_text_update, file_name, "r", priority);
}

Task* async_read_file_text(AsyncState* state, const char* file_name)
{
    return async_read_file_text_priority(state, file_name, get_priority_start(state));
}

Task* async_read_file_data_priority(AsyncState* state, const char* file_name, int priority)
{
    return async_fs_init(state, async_read_file_data_update, file_name, "rb", priority);
}

Task* async_read_file_data(AsyncState* state, const char* file_name)
{
    return async_read_file_data_priority(state, file_name, get_priority_start(state));
}

Task* async_write_file_text_priority(AsyncState* state, const char* file_name, const void* data, size_t size, int priority)
{
    Task* task = async_fs_init(state, async_write_file_text_update, file_name, "w", priority);
    if (task == NULL)
        return NULL;
    FsTextDa* resolve = (FsTextDa*)get_task_resolve(task);
    da_append_many(resolve, data, size);
    return task;
}

Task* async_write_file_text(AsyncState* state, const char* file_name, const void* data, size_t size)
{
    return async_write_file_text_priority(state, file_name, data, size, get_priority_start(state));
}

Task* async_write_file_data_priority(AsyncState* state, const char* file_name, const void* data, size_t size, int priority)
{
    Task* task = async_fs_init(state, async_write_file_data_update, file_name, "wb", priority);
    if (task == NULL)
        return NULL;
    FsDataDa* resolve = (FsDataDa*)get_task_resolve(task);
    da_append_many(resolve, data, size);
    return task;
}

Task* async_write_file_data(AsyncState* state, const char* file_name, const void* data, size_t size)
{
    return async_write_file_data_priority(state, file_name, data, size, get_priority_start(state));
}

void async_fs_close(Task* task)
{
    FsTextDa* resolve = (FsTextDa*)get_task_resolve(task);
    fclose(resolve->file);
    da_free(resolve);
}

void async_read_file_text_update(Task* task)
{
    char ch;
    FsTextDa* resolve = (FsTextDa*)get_task_resolve(task);

    if (feof(resolve->file)) {
        finish_task(task);
        return;
    }

    ch = fgetc(resolve->file);
    da_append(resolve, ch);
}

void async_read_file_data_update(Task* task)
{
    unsigned char buffer;
    size_t bytes_read;
    FsDataDa* resolve = (FsDataDa*)get_task_resolve(task);
    bytes_read = fread(&buffer, sizeof(unsigned char), 1, resolve->file);

    if (bytes_read == 0) {
        finish_task(task);
        return;
    }

    da_append(resolve, buffer);
}

void async_write_file_text_update(Task* task)
{
    static size_t i = 0;
    int bytes_writen;
    FsDataDa* resolve = (FsDataDa*)get_task_resolve(task);
    bytes_writen = fputc(resolve->items[i], resolve->file);
    i++;

    if (bytes_writen == EOF || i >= resolve->count) {
        finish_task(task);
        return;
    }
}

void async_write_file_data_update(Task* task)
{
    static size_t i = 0;
    size_t bytes_writen;
    FsDataDa* resolve = (FsDataDa*)get_task_resolve(task);
    bytes_writen = fwrite(&resolve->items[i], sizeof(unsigned char), 1, resolve->file);
    i++;

    if (bytes_writen == 0 || i >= resolve->count) {
        finish_task(task);
        return;
    }
}
#endif // ASYNC_FS_IMPLEMENTATION