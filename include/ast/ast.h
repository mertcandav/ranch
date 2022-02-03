// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#ifndef __RANCH_AST_H
#define __RANCH_AST_H

#include "asterror.h"
#include "../list.h"
#include "../lex/token.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// AST model builder.
typedef struct astbuilder {
  struct list *tokens;
  struct list *errors;
} astbuilder;

// Create new astbuilder instance allocated from heap.
struct astbuilder *astbuilder_new(struct list *tokens);
// Free astbuilder instance allocated from heap.
void astbuilder_free(struct astbuilder *astb);
// Builds binary operation AST model.
// Models like: [[expr], [operator], [expr], [operator], [expr]]
// If exists any error, pushed to errors.
// Check errors after build, AST build failed if exist any error.
struct list *astbuilder_build(struct astbuilder *astb);
// Push error to astbuilder instance.
void astbuilder_pusherr(struct astbuilder *astb,
                        struct token *tok,
                        wchar_t *message);
// Logs errors of astbuilder instance if has errors.
// Returns 1 if has errors and logged, returns 0 if not.
// Frees errors after log if freeErr is not zero.
unsigned char astbuilder_logerr(struct astbuilder *astb,
                                const unsigned char freeErr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RANCH_AST_H
