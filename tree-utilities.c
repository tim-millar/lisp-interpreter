#include "mpc.h"

int number_of_nodes(mpc_ast_t *t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total = total + number_of_nodes(t->children[i]);
    }
    return total;
  }
  return -1;
}

int number_of_leaves(mpc_ast_t *t) {
  if (t->children_num == 0) { return 1; }
  if (t->children_num >= 1) {
    int total = 0;
    for (int i = 0; i < t->children_num; i++) {
      total += number_of_leaves(t->children[i]);
    }
    return total;
  }
  return -1; 
}

int number_of_branches(mpc_ast_t *t) { 
  if (t->children_num == 0) { return 0; }
  if (t->children_num >= 1) {
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total += number_of_branches(t->children[i]);
    }
    return total;
  }
  return -1; 
}

int number_of_children_on_branch(mpc_ast_t *t, int b) { 
  int total = 0;
  for (int i = 0; i < t->children[b]->children_num; i++) {
    total += number_of_children_on_branch(t->children[b], i)
  }
  return total; 
}
