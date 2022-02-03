#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "binopr.h"
#include "lex/tokens.h"
#include "messages.h"

// Addition.
static inline struct value *solve_plus(const struct binopr *bop);
// Subtraction.
static inline struct value *solve_minus(const struct binopr *bop);
// Multiplication.
static inline struct value *solve_star(const struct binopr *bop);
// Division.
static struct value *solve_slash(const struct binopr *bop);
// Exponentiation (power).
static inline struct value *solve_exponentiation(const struct binopr *bop);
// Modulo.
static struct value *solve_modulo(const struct binopr *bop);
// Division with greatest.
static struct value *solve_reverse_slash(const struct binopr *bop);

struct binopr *binopr_new(void) {
  struct binopr *bop = (struct binopr*)malloc(sizeof(struct binopr));
  if (!bop) {
    wprintf(ERROR_ALLOCATION_FAILED L"\n");
    exit(EXIT_FAILURE);
  }
  bop->events = eventexpr_new();
  bop->left = NULL;
  bop->right = NULL;
  return bop;
}

void binopr_free(struct binopr *bop) {
  if (!bop) { return; }
  expr_events_free(bop->events);
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

static inline struct value *solve_exponentiation(const struct binopr *bop) {
  struct value *val = value_new();
  val->data = pow(bop->left->data, bop->right->data);
  return val;
}

static struct value *solve_modulo(const struct binopr *bop) {
  struct value *val = value_new();
  if (bop->left->data == 0 || bop->right->data == 0) {
    expr_events_invoke(bop->events->modulo_by_zero);
    val->data = .0;
    return val;
  }
  val->data = modf(bop->left->data, &bop->right->data);
  return val;
}

static struct value *solve_reverse_slash(const struct binopr *bop) {
  struct value *val = value_new();
  if (bop->left->data == 0 || bop->right->data == 0) {
    expr_events_invoke(bop->events->divied_by_zero);
    val->data = .0;
    return val;
  }
  if (bop->left->data > bop->right->data)
  { val->data = bop->left->data / bop->right->data; }
  else
  { val->data = bop->right->data / bop->left->data; }
  return val;
}

struct value *binopr_solve(struct binopr *bop) {
  if (!bop->left || !bop->right) {
    expr_events_invoke(bop->events->failed);
    return NULL;
  }
  if (wcscmp(bop->opr, TOKEN_PLUS) == 0)
  { return solve_plus(bop); }
  else if (wcscmp(bop->opr, TOKEN_MINUS) == 0)
  { return solve_minus(bop); }
  else if (wcscmp(bop->opr, TOKEN_STAR) == 0)
  { return solve_star(bop); }
  else if (wcscmp(bop->opr, TOKEN_SLASH) == 0)
  { return solve_slash(bop); }
  else if (wcscmp(bop->opr, TOKEN_CARET) == 0)
  { return solve_exponentiation(bop); }
  else if (wcscmp(bop->opr, TOKEN_PERCENT) == 0)
  { return solve_modulo(bop); }
  else if (wcscmp(bop->opr, TOKEN_REVERSE_SLASH) == 0)
  { return solve_reverse_slash(bop); }
  expr_events_invoke(bop->events->failed);
  return NULL;
}
