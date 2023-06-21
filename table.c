#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <stdint.h>

#include "table.h"

static inline int handle_regex(char* line) {
    const char* reg = "(\033\[[0-9]{,4}m)";
    regex_t regex;
    regmatch_t pmatch[1];
    regoff_t len;
    intmax_t count = 0;
    size_t offset = 0;
    size_t linelen = strlen(line);
    if(regcomp(&regex, reg, REG_EXTENDED)) return -1;
    while(!regexec(&regex, line+offset, 1, pmatch, REG_EXTENDED)) {
        offset += pmatch[0].rm_eo;
        count += (intmax_t)(pmatch[0].rm_eo - pmatch[0].rm_so);
        if(offset > linelen) break;
    }
    regfree(&regex);
    return (int)count;
}

table* table_make(void** col, size_t col_c) {
    table* t = malloc(sizeof(table));
    if(!t) return NULL;

    t->column_count = col_c;
    t->columns = calloc(sizeof(char*), col_c);
    if(!t->columns) {
        free(t);
        return NULL;
    }

    t->col_max_width = calloc(sizeof(size_t*), col_c);
    if(!t->col_max_width) {
        free(t->columns);
        free(t);
        return NULL;
    }

    for(size_t i=0; i<col_c; ++i) {
        t->columns[i] = calloc(sizeof(char), strlen((char*)col[i])+1);
        strcpy(t->columns[i], (char*)col[i]);
        t->col_max_width[i] = strlen((char*)col[i]);
    }

    t->rows = NULL;
    t->row_count = 0;
    return t;
}

int table_add_col(table* t, void** col, size_t col_c) {
    char** prev = t->columns;
    size_t new_c = t->column_count + col_c;
    char** new = realloc(t->columns, new_c*sizeof(char*));
    if(!new) return 0;

    size_t* prev_w = t->col_max_width;
    size_t* new_w = realloc(t->col_max_width, new_c*sizeof(size_t*));
    if(!new_w) {
        free(new);
        return 0;
    }

    for(size_t i=t->column_count; i<new_c; ++i) {
        new[i] = calloc(sizeof(char), strlen((char*)col[i-t->column_count])+1);
        strcpy(new[i], (char*)col[i-t->column_count]);
        new_w[i] = strlen(new[i]);
    }

    prev = NULL;
    prev_w = NULL;
    t->columns = new;
    t->col_max_width = new_w;
    t->column_count = new_c;
    return 1;
}

int table_add_row(table* t, void** row, size_t row_c) {
    char*** prev = t->rows;
    size_t new_c = t->row_count + 1;
    char*** new = realloc(t->rows, sizeof(char**)*new_c);
    if(!new) return 0;
    prev = NULL;
    t->rows = new;

    for(size_t i=t->row_count; i<new_c; ++i) {
        t->rows[i] = calloc(sizeof(char*), t->column_count);
        if(!t->rows[i]) return 0;

        for(size_t j=0; j<row_c; ++j) {
            t->rows[i][j] = calloc(sizeof(char), strlen((char*)row[j])+1);
            strcpy(t->rows[i][j], (char*)row[j]);
            if(strlen(t->rows[i][j]) > t->col_max_width[j]) t->col_max_width[j] = strlen(t->rows[i][j]);
        }

        if(row_c < t->column_count) {
            for(size_t j=t->column_count-row_c; j<t->column_count; ++j) {
                t->rows[i][j] = calloc(sizeof(char), 2);
                strcpy(t->rows[i][j], "");
                if(strlen(t->rows[i][j]) > strlen(t->columns[j])) t->col_max_width[j] = strlen(t->rows[i][j]);
            }
        }
    }

    t->row_count = new_c;

    return 1;
}

static inline void count_headline(table* t, size_t* c) {
    ++(*c);
    for(size_t i=0; i<t->column_count; ++i) {
        for(size_t j=0; j<t->col_max_width[i]+2; ++j) ++(*c);
        ++(*c);
    }
}

static inline void count_nl(size_t* c) {
    ++(*c);
}

static inline void count_content(table* t, char** con, size_t* c) {
    (*c) += 2;
    for(size_t i=0; i<t->column_count; ++i) {
        ++(*c);
        (*c) += strlen(con[i]);
        for(size_t j=0; j<t->col_max_width[i]-strlen(con[i]); ++j) ++(*c);
        int regex = 0;
        if((intmax_t)(regex = handle_regex(con[i])) > 0)
            for(size_t j=0; j<(intmax_t)regex; ++j) ++(*c);
        (*c) += 2;
    }
}

static inline size_t count_size(table* t) {
    size_t count = 0;
    count_headline(t, &count);
    count_content(t, t->columns, &count);
    count_nl(&count);
    count_headline(t, &count);
    for(size_t i=0; i<t->row_count; ++i) {
        count_content(t, t->rows[i], &count);
        count_nl(&count);
    }
    count_headline(t, &count);
    count_nl(&count);
    // null-terminate:
    ++count;
    return count;
}

static inline void str_headline(table* t, char* str) {
    for(size_t i=0; i<t->column_count; ++i) {
        for(size_t j=0; j<t->col_max_width[i]+2; ++j)
            strcat(str, "-");
        strcat(str, "+");
    }
}

static inline void str_start_headline(table* t, char* str) {
    sprintf(str, "+");
    str_headline(t, str);
}

static inline void str_new_headline(table* t, char* str) {
    strcat(str, "+");
    str_headline(t, str);
}

static inline void str_content(table* t, char* str, char** con) {
    for(size_t i=0; i<t->column_count; ++i) {
        strcat(str, " ");
        size_t uni_c = 0;
        for(size_t c=0; c<strlen(con[i])+1; ++c) {
            char tmp[1024];
            sprintf(tmp, "%c", con[i][c]);
            strcat(str, tmp);
            if(con[i][c]<0) ++uni_c;
        }
        uni_c -= (uni_c/2);
        for(size_t j=0; j<t->col_max_width[i]-(strlen(con[i])-uni_c); ++j) strcat(str, " ");
        int regex = 0;
        if((intmax_t)(regex = handle_regex(con[i])) > 0)
            for(size_t j=0; j<(intmax_t)regex; ++j) strcat(str, " ");
        strcat(str, " |");
    }
}

static inline void str_content_line(table* t, char* str, char** con) {
    strcat(str, "\n|");
    str_content(t, str, con);
}

char* table_str(table* t, size_t* c) {
    size_t count = count_size(t);
    char* str = malloc(sizeof(char)*count*2);
    if(!str) return NULL;
    str_start_headline(t, str);
    str_content_line(t, str, t->columns);
    strcat(str, "\n");
    str_new_headline(t, str);
    for(size_t i=0; i<t->row_count; ++i)
        str_content_line(t, str, t->rows[i]);
    strcat(str, "\n");
    str_new_headline(t, str);
    strcat(str, "\n");
    *c = count;
    return str;
}

static inline void print_headline(table* t) {
    printf("+"); for(size_t i=0; i<t->column_count; ++i) {
        for(size_t j=0; j<t->col_max_width[i]+2; ++j) printf("-");
        printf("+");
    }
}


static inline void print_content(table* t, char** con) {
    for(size_t i=0; i<t->column_count; ++i) {
        printf(" ");
        size_t uni_c = 0;
        for(size_t c=0; c<strlen(con[i])+1; ++c) {
            putchar(con[i][c]);
            if(con[i][c]<0) ++uni_c;
        }
        uni_c -= (uni_c/2);
        for(size_t j=0; j<t->col_max_width[i]-(strlen(con[i])-uni_c); ++j) printf(" ");
        int regex = 0;
        if((intmax_t)(regex = handle_regex(con[i])) > 0)
            for(size_t j=0; j<(intmax_t)regex; ++j) printf(" ");
        printf(" |");
    }
}

static inline void print_content_line(table* t, char** con) {
    printf("\n|");
    print_content(t, con);
}

void table_print(table* t) {
    print_headline(t);
    print_content_line(t, t->columns);
    printf("\n");
    print_headline(t);
    for(size_t i=0; i<t->row_count; ++i)
        print_content_line(t, t->rows[i]);
    printf("\n");
    print_headline(t);
    printf("\n");
}

void table_free(table* t) {
    for(size_t i=0; i<t->column_count; ++i)
        if(t->columns[i]) free(t->columns[i]), t->columns[i] = NULL;
    if(t->columns) free(t->columns), t->columns = NULL;
    for(size_t i=0; i<t->row_count; ++i) {
        if(t->rows[i]) {
            for(size_t j=0; j<t->column_count; ++j)
                free(t->rows[i][j]), t->rows[i][j] = NULL;
            free(t->rows[i]), t->rows[i] = NULL;
        }
    }
    if(t->rows) free(t->rows), t->rows = NULL;
    if(t->col_max_width) free(t->col_max_width), t->col_max_width = NULL;
    free(t);
}
