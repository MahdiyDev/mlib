# Mahdiy's C library

Header-only C11 building blocks. Each module lives in its own folder with a
`USAGE.md`, runnable programs in `example/`, a `Makefile`, and a `test/` suite.
The shared test harness is [`test.h`](test.h) at the repository root.

## vec

Typed dynamic array, "C++ template" style. `DEFINE_VEC(T, Name)` stamps out a
`{ T* items; size_t count; size_t capacity; }` struct plus `static inline`
`Name_*` functions for that one element type — type-checked calls, no `typeof`,
no extensions.

```c
DEFINE_VEC(int, IntVec);
IntVec v = {0};
IntVec_append(&v, 42);
IntVec_free(&v);
```

Full API and tuning macros: [vec/USAGE.md](vec/USAGE.md).
Runnable example: [vec/example/](vec/example/) (`cd vec && make examples`).

## string

`string_builder` and `string_view` helpers. `string_builder` is
`DEFINE_VEC(char, string_builder)` plus the `sb_*` / `sv_*` API: building,
splitting, trimming, searching, file slurping. Depends on `vec` (compile with
`-I../vec`); needs `#define STRING_IMPLEMENTATION` in one translation unit.

```c
#define STRING_IMPLEMENTATION
#include "string.h"

string_builder sb = sb_init("Hello");
sb_add_f(&sb, ", %s!", "World");
printf("%.*s\n", sv_fmt(sb_to_sv(&sb)));
sb_free(&sb);

string_view rest = sv_from_cstr("a,b,c");
string_view first = sv_split_c(&rest, ',');   // "a"; rest becomes "b,c"
```

Full API: [string/USAGE.md](string/USAGE.md).
Runnable examples: [string/example/](string/example/) (`cd string && make examples`).

## list

Typed doubly-linked list. `DEFINE_LIST(T, Name)` stamps out a node type, a
`{ head, tail, count }` container, and `static inline` `Name_*` functions —
O(1) insert/remove at either end or at a known node, forward and backward
iteration, no `typeof`.

```c
DEFINE_LIST(int, IntList);
IntList l = {0};
IntList_push_back(&l, 1);
IntList_node* n = IntList_push_front(&l, 0);
IntList_insert_after(&l, n, 9);          // 0, 9, 1
list_foreach(IntList, it, &l) printf("%d ", it->value);
IntList_free(&l);
```

Full API: [list/USAGE.md](list/USAGE.md).
Runnable example: [list/example/](list/example/) (`cd list && make examples`).

## stream

Lazy, pull-based streams (Java Stream / iterator style). `DEFINE_STREAM(T, Name)`
generates a stream type plus `static inline` sources (`from_array`, `from_fn`),
lazy ops (`map`, `filter`, `take`, `skip`, `peek`) and terminals (`for_each`,
`reduce`, `count`, `any`/`all`, `find`, `collect`). Nothing runs until a terminal
op, so sources can be infinite.

```c
DEFINE_STREAM(int, IntStream);
int a[] = { 1, 2, 3, 4, 5, 6 };
IntStream_for_each(
    IntStream_map(IntStream_filter(IntStream_from_array(a, 6), is_even), square),
    print_i);                               // 4 16 36
```

Full API (plus opt-in `DEFINE_STREAM_RANGE` / cross-type `DEFINE_STREAM_MAP`):
[stream/USAGE.md](stream/USAGE.md).
Runnable example: [stream/example/](stream/example/) (`cd stream && make examples`).
