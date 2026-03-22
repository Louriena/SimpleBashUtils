#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s [options] [file ...]\n", argv[0]);
    return 1;
  }

  cat_options flags = {0};

  parse_args(argc, argv, &flags);

  if (optind == argc) {
    print_file(NULL, flags);
  } else {
    for (int i = optind; i < argc; i++) {
      print_file(argv[i], flags);
    }
  }

  return 0;
}