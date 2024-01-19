// gears?
//
//

#define BUFFER_SIZE 65536

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * can optimize this fully linear count with buffer by "finding first '\\n' then
 * using its index to find the rest
 * */
int lineCount(FILE *file) {

  char buf[BUFFER_SIZE];
  int lineCount = 1;

  while (1) {
    size_t readSize = fread(&buf, sizeof(char), BUFFER_SIZE, file);

    if (ferror(file)) {
      // file read error
      return -1;
    }

    for (int i = 0; i < readSize; i++) {
      if (buf[i] == '\n') {
        lineCount++;
      }
    }

    if (feof(file)) {
      break;
    }
  }

  return lineCount;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s [gear_filename]", argv[0]);
    return 1;
  }

  FILE *input_file = fopen(argv[1], "r");

  if (input_file == NULL) {
    fprintf(stderr, "no file found: %s", argv[1]);
    return 2;
  }

  size_t buffsize = 0;
  size_t linelen = 0;
  char *line;
  int lines = lineCount(input_file);

  // if we don't have a line count, then fuck off
  if (lines == -1) {
    return -1;
  }

  // back to the beginning after getting the line count.
  rewind(input_file);
  linelen = getline(&line, &buffsize, input_file);

  if (linelen <= 0) {
    return -2;
  }

  const int maxLineLen = linelen-1;

  // we have the length and the line count, make the matrix
  char matrix[lines][maxLineLen];
  int lineidx = 0;
  while (1) {
    //just printing the lines as I go through to see.
    fprintf(stdout, "%s", line);

    //up to linelen-1 to ignore the \n
    for (int i = 0; i < linelen - 1; i++) {
      matrix[lineidx][i] = line[i];
    }
    
    //nextline
    linelen = getline(&line, &buffsize, input_file);
    lineidx++;

    if (feof(input_file)) {
      break;
    }
  }

  fprintf(stdout, "\nmatrix dimensions: x=%d, y=%d\n\n%s\n\n", maxLineLen, lines, *matrix);

  return 0;
}
