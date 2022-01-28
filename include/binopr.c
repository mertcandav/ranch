#include <stdlib.h>
#include <string.h>

#include "binopr.h"
#include "lex/tokens.h"

inline value *solve_plus(binopr *bop);
inline value *solve_minus(binopr *bop);
inline value *solve_star(binopr *bop);
inline value *solve_minus(binopr *bop);

binopr *binopr_new(void) {
  binopr *bop = (binopr*)calloc(1, sizeof(binopr));
  return bop;
}

void binopr_free(binopr *bop) {
  free(bop);
  bop = NULL;
}

inline value *solve_plus(binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data + bop->right->data;
  return val;
}

inline value *solve_minus(binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data - bop->right->data;
  return val;
}

inline value *solve_star(binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data * bop->right->data;
  return val;
}

inline value *solve_slash(binopr *bop) {
  value *val = value_new();
  if (bop->left->data == 0 || bop->right->data == 0) {
    val->data = .0;
    return val;
  }
  val->data = bop->left->data / bop->right->data;
  return val;
}

value *binopr_solve(binopr *bop) {
       if (wcscmp(bop->opr, TOKEN_PLUS) == 0)  { return solve_plus(bop); }
  else if (wcscmp(bop->opr, TOKEN_MINUS) == 0) { return solve_minus(bop); }
  else if (wcscmp(bop->opr, TOKEN_STAR) == 0)  { return solve_star(bop); }
  else if (wcscmp(bop->opr, TOKEN_SLASH) == 0) { return solve_slash(bop); }
  bop->fail = TRUE;
  return NULL;
}
