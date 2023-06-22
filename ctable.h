#include <stdlib.h>

typedef struct ctable {
    size_t column_count;
    char** columns;
    size_t* col_max_width;
    size_t row_count;
    char*** rows;
} ctable;

ctable* ctable_make(void** col, size_t col_c);
int ctable_add_col(ctable* t, void** col, size_t col_c);
int ctable_add_row(ctable* t, void** row, size_t row_c);
char* ctable_str(ctable* t, size_t* c);
void ctable_print(ctable* t);
void ctable_free(ctable* t);
