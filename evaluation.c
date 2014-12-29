#include "mpc.h"
#include <math.h>

#ifdef _WIN32

static char buffer[2048];

char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

/* Max & Min functions */
int max(int x, int y) {
  if (x >= y) { return x; }
  else        { return y; }
}

int min(int x, int y) {
  if (x <= y) { return x; }
  else        { return y; }
}

/* Use operator string to see which operation to perform */
long eval_op(long x, char *op, long y) {
  if (strcmp(op, "+"  ) == 0) { return x + y; }
  if (strcmp(op, "-"  ) == 0) {
    if (!y) { return -x; }
    else    { return x - y;}}
  if (strcmp(op, "*"  ) == 0) { return x * y; }
  if (strcmp(op, "/"  ) == 0) { return x / y; }
  if (strcmp(op, "%"  ) == 0) { return x % y; }
  if (strcmp(op, "^"  ) == 0) { return (int) pow ((double) x, (double) y); }
  if (strcmp(op, "add") == 0) { return x + y; }
  if (strcmp(op, "sub") == 0) { return x - y; }
  if (strcmp(op, "mul") == 0) { return x * y; }
  if (strcmp(op, "div") == 0) { return x / y; }
  if (strcmp(op, "mod") == 0) { return x % y; }
  if (strcmp(op, "pow") == 0) { return (int) pow ((double) x, (double) y); }
  if (strcmp(op, "max") == 0) { return max(x, y); }
  if (strcmp(op, "min") == 0) { return min(x, y); }
  return 0;
}

long eval(mpc_ast_t *t) {

  /* If tagged as number return it directly */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  /* The operator is always a second child */
  char *op = t->children[1]->contents;

  /* We store the third child in `x` */
  long x = eval(t->children[2]);

  /* We store the remaining children and combine */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  return x;
}

int main(int argc, char *argv[])
{
  /* Create some parsers */
  mpc_parser_t *Number   = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr     = mpc_new("expr");
  mpc_parser_t *Lispish  = mpc_new("lispish");

  /* Define them with the following language */
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                             \
      number   : /-?[0-9]+/ ;					  \
      operator : '+' | '-' | '*' | '/' | '%' | '^' |		  \
                 /add/ | /sub/ | /mul/ | /div/ | /mod/ |	  \
                 /pow/ | /max/ | /min/ ;			  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;	  \
      lispish  : /^/ <operator> <expr>+ /$/ ;			  \
    ",
    Number, Operator, Expr, Lispish);

  puts("\"Oh! moon old boughs LISP forth a holier din\" (Keats)");
  puts("Lispish Version 0.0.0.0.3");
  puts("Press Ctrl-c to Exit\n");

  while (1) {
    
    char *input = readline("lispish> ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispish, &r)) {
      
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);

    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }    
    free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Lispish);

  return 0;
}