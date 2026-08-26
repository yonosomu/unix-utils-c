#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int n;
  int b;
  int s;
  int v;
  int e;
  int t;
} Flags;

static const struct option long_options[] = {
    {"number", no_argument, NULL, 'n'},
    {"number-nonblank", no_argument, NULL, 'b'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {0, 0, 0, 0}};

void print_char(unsigned char c, Flags* flags);
void process_file(FILE* file, Flags* flags);
void init_flags(Flags* flags);