#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../list/list.h"
#include "async.h"

#ifndef TASK_COUNT
	#define TASK_COUNT 64
#endif
#ifndef ASYNC
	#define ASYNC
#endif // ASYNC

typedef struct Task Task;
typedef struct {
    void* resolve;
    void* reject;
} TaskResponse;
typedef struct AsyncState AsyncState;

typedef void (*resolve_func)(Task* task, void*);
typedef void (*reject_func)(Task* task, void*);
typedef void (*task_call_back)(Task* task, resolve_func resolve, reject_func reject);
typedef void (*task_update)(Task* task);
typedef void (*task_init)(Task* task);

ASYNC AsyncState* async_init();
ASYNC AsyncState* async_init_priority(int priority_start, int priority_end);
ASYNC void async_close(AsyncState* state);
ASYNC void async_update(AsyncState* state);
ASYNC Task* async_func(AsyncState* state, task_call_back call_back);
ASYNC Task* async_func_priority(AsyncState* state, task_call_back call_back, int priority);
ASYNC int get_priority_start(AsyncState* state);
ASYNC int get_priority_end(AsyncState* state);
ASYNC bool async_is_finished(AsyncState* state);

ASYNC void set_task_update(AsyncState* state, Task* task, task_update update);
ASYNC void finish_task(Task* task);
ASYNC void wait_task(Task* task);
ASYNC void set_reject(Task* task, void* reject);
ASYNC void set_resolve(Task* task, void* resolve);
ASYNC void* get_task_resolve(Task* task);
ASYNC void* get_task_reject(Task* task);
ASYNC TaskResponse await(Task* task);

#ifdef ASYNC_IMPLEMENTATION

typedef enum Status {
    ASYNC_INIT = 0,
    ASYNC_WAITING,
    ASYNC_FINISHED,
} Status;

typedef struct Task {
    void* resolve;
    void* reject;
    Status status;
    task_call_back call_back;
    task_update update;
    // task_init init;
    Task* next;
} Task;

typedef struct TaskList {
    size_t length;
    Task* head;
} TaskList;

typedef struct Tasks {
    int index_priority;
    TaskList* items;
} Tasks;

typedef struct AsyncState {
    Tasks* tasks;
    int task_count;
    int priority_start;
    int priority_end;

    bool is_finished;
} AsyncState;

void set_resolve(Task* task, void* resolve)
{
    task->resolve = resolve;
}

void set_reject(Task* task, void* reject)
{
    task->reject = reject;
}

void* get_task_resolve(Task* task)
{
    return task->resolve;
}

void* get_task_reject(Task* task)
{
    return task->reject;
}

void set_task_update(AsyncState* state, Task* task, task_update update)
{
    task->status = ASYNC_WAITING;
    task->update = update;
    state->is_finished = false;
}

TaskResponse await(Task* task)
{
    assert(task != NULL && "Task must be not NULL");
    if (task->call_back != NULL) {
        task->call_back(task, set_resolve, set_reject);
    }
    return (TaskResponse) {
        .resolve = task->resolve,
        .reject = task->reject,
    };
}

AsyncState* async_init_priority(int priority_start, int priority_end)
{
    assert(priority_start <= priority_end);
    AsyncState* state = (AsyncState*)malloc(sizeof(AsyncState));

    state->priority_start = priority_start;
    state->priority_end = priority_end;

    int priority_diff = (priority_end - priority_start) + 1;

    state->tasks = (Tasks*)malloc(priority_diff * sizeof(Tasks));
    state->task_count = priority_diff;

    for (int i = 0, j = priority_start; i < priority_diff; i++) {
        state->tasks[i].index_priority = j;
        create_list(state->tasks[i].items);
        j++;
    }

    return state;
}

int get_priority_start(AsyncState* state)
{
    return state->priority_start;
}

int get_priority_end(AsyncState* state)
{
    return state->priority_end;
}

AsyncState* async_init()
{
    return async_init_priority(0, 0);
}

void async_close(AsyncState* state)
{
    for (int i = 0; i < state->task_count; i++) {
        free_list(state->tasks[i].items);
    }
    free(state->tasks);
    free(state);
}

bool async_is_finished(AsyncState* state)
{
    state->is_finished = true;

    for (int i = 0; i < state->task_count; i++) {
        Task* task = state->tasks[i].items->head;
        for (int j = 0; j < state->tasks[i].items->length; j++) {
            if (task == NULL) {
                break;
            }
            if (task->status == ASYNC_WAITING) {
                state->is_finished = false;
            }
            task = task->next;
        }
    }

    return state->is_finished;
}

void async_update(AsyncState* state)
{
    for (int i = (state->task_count - 1); i >= 0; i--) {
        Task* task = state->tasks[i].items->head;
        for (int j = 0; j < state->tasks[i].items->length; j++) {
            if (task == NULL) {
                break;
            }
            if (task->update != NULL && task->status == ASYNC_WAITING) {
                task->update(task);
            }
            task = task->next;
        }
    }
}

void finish_task(Task* task)
{
    task->status = ASYNC_FINISHED;
}

void wait_task(Task* task)
{
    task->status = ASYNC_WAITING;
}

Task* async_func_priority(AsyncState* state, task_call_back call_back, int priority)
{
    assert(state->priority_start <= priority && state->priority_end >= priority);
    int priority_diff = (state->priority_end - state->priority_start);
    int task_index = abs(state->priority_start - priority);

    TaskList* current_task_list = state->tasks[task_index].items;

    Task new_task = {
        .call_back = call_back,
        .status = ASYNC_INIT,
        .resolve = NULL,
        .reject = NULL,
        .update = NULL,
    };

    list_push(current_task_list, new_task);

    return current_task_list->head;
}

Task* async_func(AsyncState* state, task_call_back call_back)
{
    return async_func_priority(state, call_back, state->priority_start);
}
#endif // ASYNC_IMPLEMENTATION
