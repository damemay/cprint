# ctable
minimalistic table generator written in C
- no external dependecies (only standard library)
- supports unicode characters, ansi color codes
- asan-tested for no memory leaks or overflows
- lowest possible memory usage and code size
- all functions documented in [sample main.c](main.c)

## example
```c
#include "table.h"
void* c0[] = {"Column1", "Column2",};
void* r0[] = {"Row1", "Row2",};

int main() {
    table* t = table_make(c0, 2);
    table_add_row(t, r0, 2);
    table_print(t);
    table_free(t);
}
```
prints this:
```
+---------+---------+
| Column1 | Column2 | 
+---------+---------+
| Row1    | Row2    | 
+---------+---------+
```
