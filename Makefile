# Use of this source code is governed by a MIT
# license that can be found in the LICENSE file.

GCC := gcc
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
depends: includes source
includes: inc ast lex strings terminal
inc: inc_headers
inc_headers: binopr_o eventexpr_o value_o
ast: ast_headers
ast_headers: ast_o asterror_o
lex: lex_headers
lex_headers: lexer_o token_o
strings: strings_headers
strings_headers: strings_o
terminal: commands terminal_headers
commands: commands_headers
commands_headers: commands_o
terminal_headers: terminal_o
source: binopr_events_o
pack: pack_inc pack_ast pack_lex pack_terminal pack_src pack_program

binopr_o: $(INC_DIR)/binopr.c $(INC_DIR)/binopr.h
	$(GCC) $(COMPILE) $< $(OUT) binopr.o

eventexpr_o: $(INC_DIR)/eventexpr.c $(INC_DIR)/eventexpr.h
	$(GCC) $(COMPILE) $< $(OUT) eventexpr.o

value_o: $(INC_DIR)/value.c $(INC_DIR)/value.h
	$(GCC) $(COMPILE) $< $(OUT) value.o

ast_o: $(INC_DIR)/ast/ast.cpp $(INC_DIR)/ast/ast.hpp
	$(GPP) $(COMPILE) $< $(OUT) ast.o

asterror_o: $(INC_DIR)/ast/asterror.c $(INC_DIR)/ast/asterror.h
	$(GCC) $(COMPILE) $< $(OUT) asterror.o

lexer_o: $(INC_DIR)/lex/lexer.cpp $(INC_DIR)/lex/lexer.hpp
	$(GPP) $(COMPILE) $< $(OUT) lexer.o

token_o: $(INC_DIR)/lex/token.cpp $(INC_DIR)/lex/token.hpp
	$(GPP) $(COMPILE) $< $(OUT) token.o

strings_o: $(INC_DIR)/strings/strings.cpp $(INC_DIR)/strings/strings.hpp
	$(GPP) $(COMPILE) $< $(OUT) strings.o

commands_o: $(INC_DIR)/terminal/commands/commands.cpp $(INC_DIR)/terminal/commands/commands.cpp
	$(GPP) $(COMPILE) $< $(OUT) commands.o

terminal_o: $(INC_DIR)/terminal/terminal.c $(INC_DIR)/terminal/terminal.h
	$(GCC) $(COMPILE) $< $(OUT) terminal.o

binopr_events_o: $(SRC_DIR)/binopr_events.c $(SRC_DIR)/binopr_events.h
	$(GCC) $(COMPILE) $< $(OUT) binopr_events.o

pack_inc:
	$(LD) $(RELOC) binopr.o eventexpr.o value.o $(OUT) $@.o

pack_ast:
	$(LD) $(RELOC) ast.o asterror.o $(OUT) $@.o

pack_lex:
	$(LD) $(RELOC) lexer.o token.o $(OUT) $@.o

pack_terminal:
	$(LD) $(RELOC) terminal.o commands.o $(OUT) $@.o

pack_src:
	$(LD) $(RELOC) binopr_events.o $(OUT) $@.o

pack_program:
	$(LD) $(RELOC) pack_src.o pack_inc.o pack_ast.o pack_lex.o pack_terminal.o strings.o $(OUT) $@.o

compile: $(SRC_DIR)/main.cpp
	$(GPP) $< pack_program.o $(OUT) $(EXE_OUT_NAME)

clean:
	$(DEL_FILE) *.o
