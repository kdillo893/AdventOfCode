// Trebuchet...

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const int DIGIT_WORD_COUNT = 10;
const char *DIGIT_WORDS[] = {"zero", "one", "two",   "three", "four",
                             "five", "six", "seven", "eight", "nine"};

/**
 * return null char if no "word digit" is found; otherwise, return 0 through 9
 */
char digitOfWord(char *linebuff, size_t linelen, size_t curridx) {
  // check if the index shows a word which is a "digit" word

  // simple is "if length minus current index is bigger than a word, ext"
  if (linelen - curridx <= 2) {
    return '\0';
  }

  for (int i = 0; i < DIGIT_WORD_COUNT; i++) {
    // i is the int value of the digit, which makes things easier.
    const char *currword = DIGIT_WORDS[i];

    // scale over the "current location" on the line until either we hit
    // currword null terminator or the end of the line.
    size_t lineidx = curridx, wordidx = 0;
    for (; lineidx < linelen; lineidx++, wordidx++) {
      if (currword[wordidx] == '\0') {
        // word match, return the thing
        return '0' + i;
      }

      if (linebuff[lineidx] != currword[wordidx]) {
        break;
      }
    }
  }

  // loop over the digit words?

  return '\0';
}

/**
 * process the line and return the 2-digit code
 * */
int getCodeFromLine(char *linebuff, size_t line_len) {

  fprintf(stdout, "linelen=%d, line=%s", (int)line_len, linebuff);
  if (line_len == 0)
    return 0;

  char firstNum = '0';

  // two loops going over n, first num from the search path put it in.

  // left loop
  for (size_t i = 0; i < line_len; i++) {
    char left = linebuff[i];
    if (isdigit(left)) {

      firstNum = left;
      break;
    }

    // not a digit, check if word.
    char test = digitOfWord(linebuff, line_len, i);
    if (test != '\0') {
      firstNum = test;
      break;
    }
  }

  // right loop
  char lastNum = firstNum;
  for (size_t i = line_len - 1; i >= 0; i--) {
    char right = linebuff[i];
    if (isdigit(right)) {

      lastNum = right;
      break;
    }

    // not a digit, check if word.
    char test = digitOfWord(linebuff, line_len, i);
    if (test != '\0') {
      lastNum = test;
      fprintf(stdout, "FOUND: %c\n", test);
      break;
    }
  }

  fprintf(stdout, "left=%c, right=%c\n", firstNum, lastNum);
  return ((int)(firstNum - '0') * 10) + (lastNum - '0');
}

int main(int argc, char *argv[]) {

  // will use args to pass the filename. only expecting one input, anything else
  // err.
  if (argc != 2) {
    // argv0 is the filename that reached this main.
    fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
    return 1;
  }

  // pull the file,
  FILE *input = fopen(argv[1], "r");

  if (!input) {
    fprintf(stderr, "unable to open file %s", argv[1]);
    return 2;
  }

  char *linebuff = NULL;
  size_t buff_size, line_len;
  unsigned int sum = 0;

  // line read, parse for first and last digit. sum.
  while ((line_len = getline(&linebuff, &buff_size, input)) != -1) {
    int value = getCodeFromLine(linebuff, line_len);

    sum += value;

    // temp print statement to show info.
    fprintf(stdout, "sum=%d, current=%d, linetext=%s\n", sum, value, linebuff);
  }

  fprintf(stdout, "the sum is: %d\n", sum);

  return 0;
}
