#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "binopr.h"
#include "lex/tokens.h"

static inline struct value *solve_plus(const struct binopr *bop);
static inline struct value *solve_minus(const struct binopr *bop);
static inline struct value *solve_star(const struct binopr *bop);
static struct value *solve_slash(const struct binopr *bop);

struct binopr *binopr_new(void) {
  struct binopr *bop = (struct binopr*)calloc(1, sizeof(struct binopr));
  if (bop == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
  }
  bop->events = (struct expr_events*)calloc(1, sizeof(struct expr_events));
  if (bop->events == NULL) {
    printf("error: memory allocation failed!\n");
    exit(1);
  }
  bop->left = NULL;
  bop->right = NULL;
  return bop;
}

void binopr_free(struct binopr *bop) {
  free(bop);
  bop = NULL;
}

static inline struct value *solve_plus(const struct binopr *bop) {
  struct value *val = value_new();
  val->data = bop->left->data + bop->right->data;
  return val;
}

static inline struct value *solve_minus(const struct binopr *bop) {
  struct value *val = value_new();
  val->data = bop->left->data - bop->right->data;
  return val;
}

static inline struct value *solve_star(const struct binopr *bop) {
  struct value *val = value_new();
  val->data = bop->left->data * bop->right->data;
  return val;
}

static struct value *solve_slash(const struct binopr *bop) {
  struct value *val = value_new();
  if (bop->left->data == 0 || bop->right->data == 0) {
    expr_events_invoke(bop->events->divied_by_zero);
    val->data = .0;
    return val;
  }
  val->data = bop->left->data / bop->right->data;
  return val;
}

struct value *binopr_solve(struct binopr *bop) {
       if (wcscmp(bop->opr, TOKEN_PLUS) == 0)  { return solve_plus(bop); }
  else if (wcscmp(bop->opr, TOKEN_MINUS) == 0) { return solve_minus(bop); }
  else if (wcscmp(bop->opr, TOKEN_STAR) == 0)  { return solve_star(bop); }
  else if (wcscmp(bop->opr, TOKEN_SLASH) == 0) { return solve_slash(bop); }
  expr_events_invoke(bop->events->failed);
  return NULL;
}
