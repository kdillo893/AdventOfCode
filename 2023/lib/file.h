//helper file (header) to define parsing lines from input file.
//avoiding using stdio or unix things so that I can use int instead of size_t

#include <stdio.h>
void getline(char **lineptr, int *length, FILE * stream);
