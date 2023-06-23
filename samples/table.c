#include <stdio.h>
#include <stdlib.h>
#include "../ctable.h"
#include "../ccolor.h"

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
    // Make ctable with void* array and it's size:
    ctable* t = ctable_make(c0, 2);
    if(!t) exit(EXIT_FAILURE);

    // Realloc available columns:
    if(!ctable_add_col(t, c1, 1)) exit(EXIT_FAILURE);

    // Always add the rows after all the columns are there:
    if(!ctable_add_row(t, r0, 3)) exit(EXIT_FAILURE);
    if(!ctable_add_row(t, r1, 3)) exit(EXIT_FAILURE);

    // Print the ctable to stdout:
    ctable_print(t);

    // Save the ctable to char*.
    // Second parameter is size_t* null-terminated character count.
    // It can be null if we don't need to know the size:
    char* str = ctable_str(t, NULL);
    if(!str) exit(EXIT_FAILURE);
    printf("%s", str);
    // char* str is dynamically allocated inside ctable_str, so it needs to be freed:
    free(str);

    // Remember to free the ctable after you don't need it:
    ctable_free(t);

    return EXIT_SUCCESS;
}
