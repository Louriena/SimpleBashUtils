#include "s21_cat.h"

#include <stdio.h>
#include <stdlib.h>

static void handle_squeeze_blank(int c, int *at_line_start,
                                 int *empty_line_count) {
  if (c == '\n') {
    if (*at_line_start) {
      (*empty_line_count)++;
      if (*empty_line_count > 1) {
        return;
      }
    }
  } else {
    *empty_line_count = 0;
  }
}

static void handle_line_numbering(int c, int *line_num, int *at_line_start,
                                  cat_options flags) {
  if (*at_line_start) {
    if (flags.flag_b && flags.flag_e && c == '\n') {
      printf("%6c\t", ' ');
    }
    if ((flags.flag_b && c != '\n') || (flags.flag_n && !flags.flag_b)) {
      printf("%6d\t", (*line_num)++);
    }
    *at_line_start = 0;
  }
}

static void handle_special_chars(int c, cat_options flags) {
  if (flags.flag_e && c == '\n') {
    putchar('$');
  }

  if (flags.flag_T && c == '\t') {
    printf("^I");
    return;
  }

  if (flags.flag_v) {
    if ((c >= 0 && c <= 31) || c == 127) {
      if (c != '\n' && c != '\t') {
        printf("^%c", c == 127 ? '?' : c + 64);
        return;
      }
    }
  }

  putchar(c);
}

static void process_file_content(FILE *f, cat_options flags) {
  int c;
  int line_num = 1;
  int at_line_start = 1;
  int empty_line_count = 0;

  while ((c = fgetc(f)) != EOF) {
    if (flags.flag_s) {
      handle_squeeze_blank(c, &at_line_start, &empty_line_count);
      if (flags.flag_s && c == '\n' && empty_line_count > 1) {
        continue;
      }
    }

    handle_line_numbering(c, &line_num, &at_line_start, flags);
    handle_special_chars(c, flags);

    if (c == '\n') {
      at_line_start = 1;
    }
  }
}

void print_file(char *file, cat_options flags) {
  FILE *f;

  if (file == NULL) {
    f = stdin;
  } else {
    f = fopen(file, "r");
    if (f == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", file);
      return;
    }
  }

  process_file_content(f, flags);

  if (file != NULL) {
    fclose(f);
  }
}