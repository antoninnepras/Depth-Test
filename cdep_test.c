/**
 * @file cdep_test.c
 * @author Antonin Nepras
 * @brief C depth testing
 * @version 0.1
 * @date 2022-10-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK_DEPTH 3
#define BLUE_COLOR "\e[32m"
#define YELLOW_COLOR "\e[33m"
#define RED_COLOR "\e[31m"
#define DEFAULT_COLOR "\e[0m"

#define SUCCESS 0
#define ERROR 1

#define INPUT_ERROR 101
#define OPTION_ERROR 102
#define FILE_ERROR 103


int
get_file_max_depth(FILE* file, int* max_depth, int* line);


int
print_file_depth(const char* filename, int ok_depth);


int
print_file_depth(const char* filename, int ok_depth);


void
print_help(void);


int
main(int argc, const char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Not enough arguments.\n");
    return INPUT_ERROR;
  }

  int ok_depth = OK_DEPTH;
  int first_file_index = 1;


  // read options
  for (int i = 1; argv[i][0] == '-'; ++i) {
    first_file_index++;
    if (!strcmp(argv[i], "-t")) {
      ++i;

      if (i >= argc) {
        fprintf(stderr, "Option %s needs additional argument.\n", argv[i - 1]);
        return INPUT_ERROR;
      }

      if (sscanf(argv[i], "%i", &ok_depth) != 1) {
        fprintf(stderr, "Invalid option argument.\n");
        return OPTION_ERROR;
      }

      first_file_index++;
    } else if (!strcmp(argv[i], "-h")) {
      print_help();
      return 0;
    }
  }


  for (int i = first_file_index; i < argc; ++i) {
    if (print_file_depth(argv[i], ok_depth) == ERROR) {
      return FILE_ERROR;
    }
  }

  return SUCCESS;
}


int
get_file_max_depth(FILE* file, int* max_depth, int* line)
{
  if (file == NULL || max_depth == NULL || line == NULL) {
    return ERROR;
  }

  int dep = 0;
  int current_line = 1;
  *max_depth = 0;
  int c;

  bool ignore1 = false; // ""
  bool ignore2 = false; // ''
  while (1) {
    c = getc(file);
    if (c == EOF) {
      break;
    }

    // ignore parenthes in "" ''
    if (ignore1) {
      if (c != '"') {
        continue;
      } else {
        ignore1 = false;
        continue;
      }
    } else if (ignore2) {
      if (c != '\'') {
        continue;
      } else {
        ignore2 = false;
        continue;
      }
    }

    // find parenthes
    if (c == '{') {
      ++dep;
      if (dep > *max_depth) {
        *max_depth = dep;
        *line = current_line;
      }
    } else if (c == '}') {
      --dep;
    } else if (c == '\n') {
      ++current_line;
    } else if (c == '"') {
      ignore1 = true;
    } else if (c == '\'') {
      ignore2 = true;
    }
  }

  return SUCCESS;
}


int
get_filename_max_depth(const char* filename, int* max_depth, int* line)
{
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return ERROR;
  }

  int ret = get_file_max_depth(file, max_depth, line);

  fclose(file);
  return ret;
}


int
print_file_depth(const char* filename, int ok_depth)
{
  int max_depth;
  int line;
  if (get_filename_max_depth(filename, &max_depth, &line) == ERROR) {
    return ERROR;
  }

  // non c - file - skip
  if (max_depth == 0) {
    return SUCCESS;
  }

  // print info
  printf("%s depth: %i on line %5i in file %s%s\n",
         max_depth >= ok_depth
           ? (max_depth == ok_depth ? YELLOW_COLOR : RED_COLOR)
           : BLUE_COLOR,
         max_depth,
         line,
         filename,
         DEFAULT_COLOR);

  return SUCCESS;
}


void
print_help(void)
{
  printf("Test depth of c files\n"
         "USAGE:\n"
         "    cdepth_test [options] [files]\n"
         "OPTIONS:\n"
         "    -h       |    print this help\n"
         "    -t [n]   |    treshold of OK depth\n"
         "EXAMPLE:\n"
         "    cdepth_test -t 4 main.c                |  test main.c file\n"
         "    cdepth_test (find ${dir} -name \"*.c\")  |  test all files in "
         "current directory\n");
}
