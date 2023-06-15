#include <stdlib.h>

typedef struct table {
    size_t column_count;
    char** columns;
    size_t* col_max_width;
    size_t row_count;
    char*** rows;
} table;

table* table_make(void** col, size_t col_c);
int table_add_col(table* t, void** col, size_t col_c);
int table_add_row(table* t, void** row, size_t row_c);
char* table_str(table* t, size_t* c);
void table_print(table* t);
void table_free(table* t);
