# ctable
minimalistic table generator written in C
- depends only on standard library and posix (regex)
- supports unicode characters, ansi color codes
- asan-tested for no memory leaks or overflows
- lowest possible memory usage and code size
- all functions documented in [samples](samples/table_colors.c)

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

## example
available in [samples](samples/spinner.c)
