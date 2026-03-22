#include "parse_args.h"

#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "my_grep.h"

int parse_args(int argc, char **argv, Flags *flags) {
  int opt;
  int result = 0;

  while ((opt = getopt(argc, argv, "e:ivclnhsof:")) != -1 && result == 0) {
    switch (opt) {
      case 'e':
        add_pattern(flags, optarg);
        break;
      case 'i':
        flags->flag_i = 1;
        break;
      case 'v':
        flags->flag_v = 1;
        break;
      case 'c':
        flags->flag_c = 1;
        break;
      case 'l':
        flags->flag_l = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case '?':
        if (optopt == 'e' || optopt == 'f') {
          fprintf(stderr, "Option -%c requires an argument\n", optopt);
        } else {
          fprintf(stderr, "Unknown option -%c\n", optopt);
        }
        result = 1;
        break;
      default:
        result = 1;
        break;
    }
  }

  if (result == 0 && flags->pattern_count == 0 && optind < argc) {
    add_pattern(flags, argv[optind++]);
  }

  return result;
}

void free_patterns(Flags *flags) {
  for (int i = 0; i < flags->pattern_count; i++) {
    free(flags->patterns[i]);
  }
  flags->pattern_count = 0;
}