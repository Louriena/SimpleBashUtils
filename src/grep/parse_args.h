#ifndef PARSE_ARGS
#define PARSE_ARGS

#include <regex.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PATTERNS 100

typedef struct {
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int pattern_count;
  char *patterns[MAX_PATTERNS];
} Flags;

int parse_args(int argc, char **argv, Flags *flags);
void grep_file(const char *filename, Flags *flags, int argc);
void free_patterns(Flags *flags);

#endif  // GREP_H