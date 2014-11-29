#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char *argv[])
{
  /* Print version & exit info */
  puts("LISPish Version 0.0.0.0.1");
  puts("Press Ctrl-c to Exit\n");

  /* In never-ending loop */
  while (1) {
    
    /* Output the prompt & get input */
    char *input = readline("lispish> ");

    /* Add input to history */
    add_history(input);

    /* Echo input back to user */
    printf("No you're a %s\n", input);

    /* Free retreived input */
    free(input);
  }
  return 0;
}
