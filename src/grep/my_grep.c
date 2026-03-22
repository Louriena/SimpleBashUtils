#include "my_grep.h"

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_pattern(Flags *flags, const char *pattern) {
  char *copy = malloc(strlen(pattern) + 1);
  if (!copy) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  strcpy(copy, pattern);
  flags->patterns[flags->pattern_count++] = copy;
}

FILE *open_input_file(const char *filename) {
  FILE *file = stdin;
  if (filename) {
    file = fopen(filename, "r");
    if (!file) {
      perror("fopen");
    }
  }
  return file;
}

int determine_multiple_files(int argc) {
  static int multiple_files = -1;
  if (multiple_files == -1) {
    multiple_files = (optind < argc - 1) ? 1 : 0;
  }
  return multiple_files;
}

void print_match_info(const char *filename, int line_num, Flags *flags,
                      int multiple_files) {
  if (!flags && multiple_files && filename) {
    printf("%s:", filename);
  }

  if (flags->flag_n) {
    printf("%d:", line_num);
  }
}

void process_whole_line(const char *line, const char *filename, int line_num,
                        Flags *flags, int multiple_files) {
  print_match_info(filename, line_num, flags, multiple_files);
  printf("%s\n", line);
}

void process_count_output(const char *filename, int count, Flags *flags,
                          int multiple_files) {
  if (multiple_files && filename && !flags) {
    printf("%s:", filename);
  }
  printf("%d\n", count);
}

void process_grep_results(FILE *file, const char *filename, regex_t *regex,
                          Flags *flags, int multiple_files,
                          int regex_compiled) {
  char line[MAX_LINE_LENGTH];
  int line_num = 1;
  int match_count = 0;

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    int match = 0;
    for (int i = 0; i < flags->pattern_count; i++) {
      if (regexec(&regex[i], line, 0, NULL, 0) == 0) {
        match = 1;
        break;
      }
    }

    if (flags->flag_v) match = !match;

    if (match) {
      match_count++;

      if (flags->flag_l) {
        printf("%s\n", filename ? filename : "(standard input)");
        break;
      }

      if (!flags->flag_c) {
        if (flags->flag_n) {
          print_match_info(filename, line_num, flags, multiple_files);
          printf("%s\n", line);
        }

        else {
          process_whole_line(line, filename, line_num, flags, multiple_files);
        }
      }
    }

    line_num++;
  }

  if (flags->flag_c) {
    process_count_output(filename, match_count, flags, multiple_files);
  }

  if (file != stdin) fclose(file);

  for (int i = 0; i < regex_compiled; i++) {
    regfree(&regex[i]);
  }
}

void grep_file(const char *filename, Flags *flags, int multiple_files) {
  regex_t regex[MAX_PATTERNS];
  int regex_compiled = 0;

  for (int i = 0; i < flags->pattern_count; i++) {
    int ret = regcomp(&regex[i], flags->patterns[i],
                      flags->flag_i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);
    if (ret) {
      char error_msg[100];
      regerror(ret, &regex[i], error_msg, sizeof(error_msg));
      fprintf(stderr, "Regex compilation failed: %s\n", error_msg);
      for (int j = 0; j < regex_compiled; j++) {
        regfree(&regex[j]);
      }
      return;
    }
    regex_compiled++;
  }

  FILE *file = open_input_file(filename);
  if (!file) {
    for (int i = 0; i < regex_compiled; i++) {
      regfree(&regex[i]);
    }
    return;
  }

  process_grep_results(file, filename, regex, flags, multiple_files,
                       regex_compiled);
}