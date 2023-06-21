#include <stdio.h>
#include "table.h"
#include "color.h"

void* c0[] = {
    "The first column that spawns long distance",
    "2",
};
void* c1[] = {
    "Column 3",
};
void* r0[] = {
    "A test of some unicóde →",
    "2",
    "Not enough rows will result in unexpected behavior",
};
void* r1[] = {
    cc_bg_b_black cc_fg_green cc_bold "Another row of dątą" cc_clear,
    " ",
    "It works!",
};

int main() {
    // Make table with void* array and it's size:
    table* t = table_make(c0, 2);
    if(!t) exit(EXIT_FAILURE);

    // Realloc available columns:
    if(!table_add_col(t, c1, 1)) exit(EXIT_FAILURE);

    // Always add the rows after all the columns are there:
    if(!table_add_row(t, r0, 3)) exit(EXIT_FAILURE);
    if(!table_add_row(t, r1, 3)) exit(EXIT_FAILURE);

    // Print the table to stdout:
    table_print(t);

    // Save the table to char*. size is null-terminated character count:
    size_t size;
    char* str = table_str(t, &size);
    printf("%s", str);
    // char* str is dynamically allocated inside table_str, so it needs to be freed:
    free(str);

    // Remember to free the table after you don't need it:
    table_free(t);

    return EXIT_SUCCESS;
}
