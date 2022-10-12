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


int
print_file_depth(const char* filename, int ok_depth);


int
main(int argc, const char** argv)
{
  if (argc < 2) {
    return 1;
  }

  int ok_depth = OK_DEPTH;

  int first_file_index = 1;

  for (int i = 1; argv[i][0] == '-'; ++i) {
    first_file_index++;
    if (!strcmp(argv[i], "-t")) {
      ++i;
      if (sscanf(argv[i], "%i", &ok_depth) != 1) {
        return 3;
      }
      first_file_index++;
    }
  }


  for (int i = first_file_index; i < argc; ++i) {
    if (print_file_depth(argv[i], ok_depth) != 0) {
      return 1;
    }
  }

  return 0;
}


int
print_file_depth(const char* filename, int ok_depth)
{
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return 1;
  }

  int maxdep = 0;
  int dep = 0;
  int line = 1;
  int maxdepline = 0;
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
      if (dep > maxdep) {
        maxdep = dep;
        maxdepline = line;
      }
    } else if (c == '}') {
      --dep;
    } else if (c == '\n') {
      ++line;
    }
  }

  fclose(file);

  // non c - file
  if (maxdep == 0) {
    return 0;
  }


  // print info
  printf("%2sdepth: %i on line %5i in file %s%s\n",
         maxdep >= ok_depth ? (maxdep == ok_depth ? YELLOW_COLOR : RED_COLOR)
                            : BLUE_COLOR,
         maxdep,
         maxdepline,
         filename,
         DEFAULT_COLOR);

  return 0;
}
