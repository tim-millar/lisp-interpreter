#include <stdio.h>
#include <stdlib.h>

/* Declare a buffer for user input of size 2048 */
static char input[2048];

int main(int argc, char *argv[])
{
  /* Print Version and Exit Information */
  puts("LISPish Version 0.0.0.0.1");
  puts("Press Ctrl-c to Exit\n");

  /* Never-ending loop */
  while (1) {
    
    /* Outputs prompt */
    fputs("lispish> ", stdout);

    /* Read line of user input of maximum size 2048 */
    fgets(input, 2048, stdin);

    /* Echo input back to user */
    printf("No you're a %s", input);
  }

  return 0;
}
