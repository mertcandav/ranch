# Use of this source code is governed by a MIT
# license that can be found in the LICENSE file.

GCC := gcc
GPP := g++
CC := $(GCC) # Used compiler to compile.

# FLAGS
COMPILE := -c
OUT := -o
LM := -lm
WALL := -Wall

EXE_OUT_NAME := ranch
DEL_FILE := rm -f
ifeq ($(OS),Windows_NT)
	EXE_OUT_NAME := $(EXE_OUT_NAME).exe
	DEL_FILE := del
endif

SRC_DIR := ./src
INC_DIR := ./include

# PHONY: clean
all: depends compile
depends: includes source
includes: inc ast lex terminal
inc: binopr_o eventexpr_o list_o strings_o value_o
ast: ast_o asterror_o
lex: lexer_o token_o
terminal: commands terminal_o
commands: commands_o
source: binopr_events_o cli_o expr_o

binopr_o: $(INC_DIR)/binopr.c $(INC_DIR)/binopr.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) binopr.o

eventexpr_o: $(INC_DIR)/eventexpr.c $(INC_DIR)/eventexpr.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) eventexpr.o

list_o: $(INC_DIR)/list.c $(INC_DIR)/list.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) list.o

value_o: $(INC_DIR)/value.c $(INC_DIR)/value.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) value.o

ast_o: $(INC_DIR)/ast/ast.c $(INC_DIR)/ast/ast.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) ast.o

asterror_o: $(INC_DIR)/ast/asterror.c $(INC_DIR)/ast/asterror.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) asterror.o

lexer_o: $(INC_DIR)/lex/lexer.c $(INC_DIR)/lex/lexer.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) lexer.o

token_o: $(INC_DIR)/lex/token.c $(INC_DIR)/lex/token.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) token.o

strings_o: $(INC_DIR)/strings.c $(INC_DIR)/strings.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) strings.o

commands_o: $(INC_DIR)/terminal/commands/commands.c $(INC_DIR)/terminal/commands/commands.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) commands.o

terminal_o: $(INC_DIR)/terminal/terminal.c $(INC_DIR)/terminal/terminal.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) terminal.o

binopr_events_o: $(SRC_DIR)/binopr_events.c $(SRC_DIR)/binopr_events.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) binopr_events.o

cli_o: $(SRC_DIR)/cli.c $(SRC_DIR)/cli.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) cli.o

expr_o: $(SRC_DIR)/expr.c $(SRC_DIR)/expr.h
	$(CC) $(WALL) $(COMPILE) $< $(OUT) expr.o

compile: $(SRC_DIR)/main.c
	$(CC) $(WALL) $< binopr.o eventexpr.o list.o strings.o value.o \
		ast.o asterror.o lexer.o token.o commands.o terminal.o \
		binopr_events.o cli.o expr.o $(OUT) $(EXE_OUT_NAME) $(LM)

clean:
	$(DEL_FILE) *.o
