#define _GNU_SOURCE

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 1024
#define MAX_FILES 256

typedef struct
{
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;

    int pattern_count;
    char *patterns[MAX_PATTERNS];
    regex_t compiled_regexes[MAX_PATTERNS];

    int file_count;
    char *files[MAX_FILES];
} Flags;