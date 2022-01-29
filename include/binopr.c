#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "binopr.h"
#include "lex/tokens.h"

static inline value *solve_plus(const binopr *bop);
static inline value *solve_minus(const binopr *bop);
static inline value *solve_star(const binopr *bop);
static value *solve_slash(const binopr *bop);

binopr *binopr_new(void) {
  binopr *bop = (binopr*)calloc(1, sizeof(binopr));
  if (bop == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
  }
  bop->events = (expr_events*)calloc(1, sizeof(expr_events));
  if (bop->events == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
  }
  bop->left = NULL;
  bop->right = NULL;
  return bop;
}

void binopr_free(binopr *bop) {
  free(bop);
  bop = NULL;
}

static inline value *solve_plus(const binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data + bop->right->data;
  return val;
}

static inline value *solve_minus(const binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data - bop->right->data;
  return val;
}

static inline value *solve_star(const binopr *bop) {
  value *val = value_new();
  val->data = bop->left->data * bop->right->data;
  return val;
}

static value *solve_slash(const binopr *bop) {
  value *val = value_new();
  if (bop->left->data == 0 || bop->right->data == 0) {
    expr_events_invoke(bop->events->divied_by_zero);
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
  expr_events_invoke(bop->events->failed);
  return NULL;
}
