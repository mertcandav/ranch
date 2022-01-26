# Use of this source code is governed by a MIT
# license that can be found in the LICENSE file.

GPP := g++
COMPILE := -c

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
depends: includes
includes: ast lex strings terminal
ast: ast_headers
ast_headers: ast_o error_o
lex: lex_headers
lex_headers: lexer_o token_o
strings: strings_headers
strings_headers: strings_o
terminal: commands terminal_headers
commands: commands_headers
commands_headers: commands_o
terminal_headers: terminal_o
pack: pack_ast pack_lex pack_terminal pack_inc

ast_o: $(INC_DIR)/ast/ast.cpp $(INC_DIR)/ast/ast.hpp
	$(GPP) $(COMPILE) $< $(OUT) ast.o

error_o: $(INC_DIR)/ast/error.cpp $(INC_DIR)/ast/error.hpp
	$(GPP) $(COMPILE) $< $(OUT) error.o

lexer_o: $(INC_DIR)/lex/lexer.cpp $(INC_DIR)/lex/lexer.hpp
	$(GPP) $(COMPILE) $< $(OUT) lexer.o

token_o: $(INC_DIR)/lex/token.cpp $(INC_DIR)/lex/token.hpp
	$(GPP) $(COMPILE) $< $(OUT) token.o

strings_o: $(INC_DIR)/strings/strings.cpp $(INC_DIR)/strings/strings.hpp
	$(GPP) $(COMPILE) $< $(OUT) strings.o

commands_o: $(INC_DIR)/terminal/commands/commands.cpp $(INC_DIR)/terminal/commands/commands.cpp
	$(GPP) $(COMPILE) $< $(OUT) commands.o

terminal_o: $(INC_DIR)/terminal/terminal.cpp $(INC_DIR)/terminal/terminal.hpp
	$(GPP) $(COMPILE) $< $(OUT) terminal.o

pack_ast:
	$(LD) $(RELOC) ast.o error.o $(OUT) $@.o

pack_lex:
	$(LD) $(RELOC) lexer.o token.o $(OUT) $@.o

pack_terminal:
	$(LD) $(RELOC) terminal.o commands.o $(OUT) $@.o

pack_inc:
	$(LD) $(RELOC) pack_ast.o pack_lex.o pack_terminal.o strings.o $(OUT) $@.o

compile: $(SRC_DIR)/main.cpp
	$(GPP) $< pack_inc.o $(OUT) $(EXE_OUT_NAME)

clean:
	$(DEL_FILE) *.o
