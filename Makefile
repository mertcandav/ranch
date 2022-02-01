# Use of this source code is governed by a MIT
# license that can be found in the LICENSE file.


GCC := gcc
GPP := g++
COMPILE := -c
CC := $(GCC) # Used compiler to compile.

LD := ld
RELOC := -relocatable

OUT := -o

EXE_OUT_NAME := ranch
DEL_FILE := rm -f
ifeq ($(OS),Windows_NT)
	EXE_OUT_NAME := $(EXE_OUT_NAME).exe
	DEL_FILE := del
endif

SRC_DIR := ./src
INC_DIR := ./include

# PHONY: clean
all: depends pack compile
depends: includes source
includes: inc ast lex terminal
inc: inc_headers
inc_headers: binopr_o eventexpr_o list_o strings_o value_o
ast: ast_headers
ast_headers: ast_o asterror_o
lex: lex_headers
lex_headers: lexer_o token_o
terminal: commands terminal_headers
commands: commands_headers
commands_headers: commands_o
terminal_headers: terminal_o
source: binopr_events_o cli_o expr_o
pack: pack_inc pack_ast pack_lex pack_terminal pack_src pack_program

binopr_o: $(INC_DIR)/binopr.c $(INC_DIR)/binopr.h
	$(CC) $(COMPILE) $< $(OUT) binopr.o

eventexpr_o: $(INC_DIR)/eventexpr.c $(INC_DIR)/eventexpr.h
	$(CC) $(COMPILE) $< $(OUT) eventexpr.o

list_o: $(INC_DIR)/list.c $(INC_DIR)/list.h
	$(CC) $(COMPILE) $< $(OUT) list.o

value_o: $(INC_DIR)/value.c $(INC_DIR)/value.h
	$(CC) $(COMPILE) $< $(OUT) value.o

ast_o: $(INC_DIR)/ast/ast.c $(INC_DIR)/ast/ast.h
	$(CC) $(COMPILE) $< $(OUT) ast.o

asterror_o: $(INC_DIR)/ast/asterror.c $(INC_DIR)/ast/asterror.h
	$(CC) $(COMPILE) $< $(OUT) asterror.o

lexer_o: $(INC_DIR)/lex/lexer.c $(INC_DIR)/lex/lexer.h
	$(CC) $(COMPILE) $< $(OUT) lexer.o

token_o: $(INC_DIR)/lex/token.c $(INC_DIR)/lex/token.h
	$(CC) $(COMPILE) $< $(OUT) token.o

strings_o: $(INC_DIR)/strings.c $(INC_DIR)/strings.h
	$(CC) $(COMPILE) $< $(OUT) strings.o

commands_o: $(INC_DIR)/terminal/commands/commands.c $(INC_DIR)/terminal/commands/commands.h
	$(CC) $(COMPILE) $< $(OUT) commands.o

terminal_o: $(INC_DIR)/terminal/terminal.c $(INC_DIR)/terminal/terminal.h
	$(CC) $(COMPILE) $< $(OUT) terminal.o

binopr_events_o: $(SRC_DIR)/binopr_events.c $(SRC_DIR)/binopr_events.h
	$(CC) $(COMPILE) $< $(OUT) binopr_events.o

cli_o: $(SRC_DIR)/cli.c $(SRC_DIR)/cli.h
	$(CC) $(COMPILE) $< $(OUT) cli.o

expr_o: $(SRC_DIR)/expr.c $(SRC_DIR)/expr.h
	$(CC) $(COMPILE) $< $(OUT) expr.o

pack_inc:
	$(LD) $(RELOC) binopr.o eventexpr.o list.o strings.o value.o $(OUT) $@.o

pack_ast:
	$(LD) $(RELOC) ast.o asterror.o $(OUT) $@.o

pack_lex:
	$(LD) $(RELOC) lexer.o token.o $(OUT) $@.o

pack_strings:
	$(LD) $(RELOC) strings.o stringsxx.o $(OUT) $@.o

pack_terminal:
	$(LD) $(RELOC) terminal.o commands.o $(OUT) $@.o

pack_src:
	$(LD) $(RELOC) binopr_events.o cli.o expr.o $(OUT) $@.o

pack_program:
	$(LD) $(RELOC) pack_src.o pack_inc.o pack_ast.o pack_lex.o pack_terminal.o $(OUT) $@.o

compile: $(SRC_DIR)/main.c
	$(GPP) $< pack_program.o $(OUT) $(EXE_OUT_NAME)

clean:
	$(DEL_FILE) *.o
