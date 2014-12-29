#include "mpc.h"

/* If we are compiling on Windows */
#ifdef _WIN32

static char buffer[2048];

/* Fake readline function */
char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add history function */
void add_history(char *unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char *argv[])
{
  /* Creaate some parsers */
  mpc_parser_t *Number   = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr     = mpc_new("expr");
  mpc_parser_t *Lispish  = mpc_new("lispish");

  /* Define them with the following language */
  mpca_lang(MPCA_LANG_DEFAULT,
   "                                                      \
    number   : /-?([0-9]*\.?[0-9]+|[0-9]+)/ ;		  \
    operator : '+' | '-' | '*' | '/' | '%' |		  \
               /add/ | /sub/ | /mul/ | /div/ | /mod/ ;	  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;	  \
    lispish  : /^/ <operator> <expr>+ /$/ ;		  \
  ",
  Number, Operator, Expr, Lispish);

  puts("\"Oh! moon old boughs LISP forth a holier din\" (Keats)");
  puts("Lispish Version 0.0.0.0.2");
  puts("Press Ctrl-c to Exit\n");

  while (1) {
    
    char *input = readline("lispish> ");
    add_history(input);

    /* Attempt to parse the user input */
    mpc_result_t result;
    if (mpc_parse("<stdin>", input, Lispish, &result)) {
      /* On success print and delete the AST */
      mpc_ast_print(result.output);
      mpc_ast_delete(result.output);
     } else {
      /* Otherwise print and delete errors */
      mpc_err_print(result.error);
      mpc_err_delete(result.error);
    }
    free(input);
  }
  /* Undefine and delete our parsers */
  mpc_cleanup(4, Number, Operator, Expr, Lispish);

  return 0;
}
