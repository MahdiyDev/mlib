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
