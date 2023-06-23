simple utilities for printing in C, written with lowest possible memory usage and code size in goal

# ctable
minimalistic table generator
- depends only on standard library and posix (regex)
- supports unicode characters, ansi color codes
- asan-tested for no memory leaks or overflows
- all functions documented in [samples](samples/table.c)

## example
```c
#include "ctable.h"
void* c0[] = {"Column1", "Column2",};
void* r0[] = {"Row1", "Row2",};

int main() {
    table* t = ctable_make(c0, 2);
    ctable_add_row(t, r0, 2);
    ctable_print(t);
    ctable_free(t);
}
```
prints:
```
+---------+---------+
| Column1 | Column2 |
+---------+---------+
| Row1    | Row2    |
+---------+---------+
```

# cspin
minimalistic progress spinner
- depends on posix (pthreads) and standard library
- thread and memory/address sanitized
- all functions documented in [samples](samples/spinner.c)

## example
```c
#include "cspin.h"
cspin* t;

int main() {
    cspin_start(&t, "Working...");
    // Some actual code to do stuff
    cspin_free(&t);
}
```

# ccolor
ansi color codes macros
- check [ccolor.h](ccolor.h)
