
#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_T;
  int flag_v;
} cat_options;

void parse_args(int argc, char **argv, cat_options *flags);
void print_file(char *file, cat_options flags);

#endif