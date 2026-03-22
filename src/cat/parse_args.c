#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_cat.h"

void parse_args(int argc, char **argv, cat_options *flags) {
  int flg;
  int option_index = 0;

  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};

  while ((flg = getopt_long(argc, argv, "+bevnstET", long_options,
                            &option_index)) != -1) {
    switch (flg) {
      case 'b':
        flags->flag_b = 1;
        break;
      case 'e':
        flags->flag_e = 1;
        flags->flag_v = 1;
        break;
      case 'n':
        flags->flag_n = 1;
        break;
      case 's':
        flags->flag_s = 1;
        break;
      case 'v':
        flags->flag_v = 1;
        break;
      case 't':
        flags->flag_T = 1;
        flags->flag_v = 1;
        break;
      case 'E':
        flags->flag_e = 1;
        break;
      case 'T':
        flags->flag_T = 1;
        break;
      default:
        printf("Usage: %s [-belnstuv] [file ...]\n", argv[0]);
        exit(1);
    }
  }

  if (flags->flag_b) flags->flag_n = 0;
}
