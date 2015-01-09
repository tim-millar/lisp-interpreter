#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

char *readline(char *prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

/* Enumerate possible error types */
enum err_types { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* Enumerate possible lval types */
enum lval_types { LVAL_NUM, LVAL_ERR };

/* New lval struct type */
typedef struct {
  int type;
  union {
    long num;
    int err; };
} lval;

/* New number type lval */
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

/* New error type lval */
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

/* Print an "lval" */
void lval_print(lval v) {
  switch (v.type) {
    /* In the case type where is a number, print */
    /* Then break out of switch. */
  case LVAL_NUM: printf("%li", v.num);
    break;    
    /* In the case where type is an error */
  case LVAL_ERR:
    /* Check type of error and print */
    if (v.err == LERR_DIV_ZERO) {
      printf("Error: Division by Zero!");
    }
    if (v.err == LERR_BAD_OP) {
      printf("Error: Invalid Operator!");
    }
    if (v.err == LERR_BAD_NUM) {
      printf("Error: Invalid Number!");
    }
    break;
  }
}

/* Print lval followed by newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }

/* Max & Min functions */
int max(int x, int y) {
  if (x >= y) { return x; }
  else        { return y; }
}

int min(int x, int y) {
  if (x <= y) { return x; }
  else        { return y; }
}

/* New eval_op function consumes lvals */
lval eval_op(lval x, char *op, lval y) {

  /* If either value is an error return it */
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }

  /* Otherwise, perform operation */
  if (strcmp(op, "+"  ) == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "-"  ) == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "*"  ) == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "/"  ) == 0) { 
    /* If second operand is zero, return error */
    return y.num == 0
      ? lval_err(LERR_DIV_ZERO)
      : lval_num(x.num / y.num); 
  }
  if (strcmp(op, "%"  ) == 0) { return lval_num(x.num % y.num); }
  if (strcmp(op, "^"  ) == 0) { return lval_num((long) pow ((double) x.num, 
							    (double) y.num)); }
  if (strcmp(op, "add") == 0) { return lval_num(x.num + y.num); }
  if (strcmp(op, "sub") == 0) { return lval_num(x.num - y.num); }
  if (strcmp(op, "mul") == 0) { return lval_num(x.num * y.num); }
  if (strcmp(op, "div") == 0) { return lval_num(x.num / y.num); }
  if (strcmp(op, "mod") == 0) { return lval_num(x.num % y.num); }
  if (strcmp(op, "pow") == 0) { return lval_num((long) pow ((double) x.num, 
							    (double) y.num)); }
  if (strcmp(op, "max") == 0) { return lval_num(max(x.num, y.num)); }
  if (strcmp(op, "min") == 0) { return lval_num(min(x.num, y.num)); }
  
  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
  
  if (strstr(t->tag, "number")) {
    /* Check if there is some error in conversion */
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  char *op = t->children[1]->contents;
  lval x = eval(t->children[2]);

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
  puts("Lispish Version 0.0.0.0.4");
  puts("Press Ctrl-c to Exit\n");

  while (1) {
    
    char *input = readline("lispish> ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispish, &r)) {      
      lval result = eval(r.output);
      lval_println(result);
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
