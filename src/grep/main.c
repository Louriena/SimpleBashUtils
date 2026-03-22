#include <getopt.h>

#include "parse_args.h"

int main(int argc, char **argv) {
  int exit_code = 0;
  Flags flags = {0};

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [options] [file ...]\n", argv[0]);
    exit_code = 1;
  } else {
    exit_code = parse_args(argc, argv, &flags);

    if (exit_code == 0) {
      if (flags.pattern_count == 0) {
        fprintf(stderr, "No pattern specified\n");
        exit_code = 1;
      } else {
        if (optind == argc) {
          grep_file(NULL, &flags, argc);
        } else {
          for (; optind < argc; optind++) {
            grep_file(argv[optind], &flags, argc);
          }
        }
      }
    }
  }

  free_patterns(&flags);
  return exit_code;
}