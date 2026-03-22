#ifndef MY_GREP
#define MY_GREP

#include "parse_args.h"

void add_pattern(Flags *flags, const char *pattern);
int process_pattern_file(const char *filename, Flags *flags, char **argv);

FILE *open_input_file(const char *filename);
int determine_multiple_files(int argc);

void print_match_info(const char *filename, int line_num, Flags *flags,
                      int multiple_files);
void process_whole_line(const char *line, const char *filename, int line_num,
                        Flags *flags, int multiple_files);
void process_count_output(const char *filename, int count, Flags *flags,
                          int multiple_files);

void process_grep_results(FILE *file, const char *filename, regex_t *regex,
                          Flags *flags, int argc, int regex_compiled);

#endif  // MY_GREP.H