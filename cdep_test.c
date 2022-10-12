#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define OK_DEPTH 3
#define BLUE_COLOR "\e[32m"
#define YELLOW_COLOR "\e[33m"
#define RED_COLOR "\e[31m"
#define DEFAULT_COLOR "\e[0m"


int
print_file_depth(const char* filename);


int
main(int argc, const char** argv)
{
  if (argc < 2) {
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    if (print_file_depth(argv[i]) != 0) {
      return 1;
    }
  }

  return 0;
}


int
print_file_depth(const char* filename)
{
  FILE* file = fopen(filename, "r");
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
         maxdep >= OK_DEPTH ? (maxdep == OK_DEPTH ? YELLOW_COLOR : RED_COLOR)
                            : BLUE_COLOR,
         maxdep,
         maxdepline,
         filename,
         DEFAULT_COLOR);

  return 0;
}
