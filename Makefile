GPP := g++
COMPILE := -c

LD := ld
RELOC := -relocatable

OUT := -o

EXE_OUT_NAME := ranch
ifeq ($(OS),Windows_NT)
	EXE_OUT_NAME := $(EXE_OUT_NAME).exe
endif

SRC_DIR := ./src
INC_DIR := ./include

all: depends compile
depends: includes
includes: lex strings terminal
lex: lex_headers pack_lex
lex_headers: lexer_cpp token_cpp
strings: strings_headers
strings_headers: strings_cpp
terminal: commands terminal_headers pack_terminal
commands: commands_headers
commands_headers: commands_cpp
terminal_headers: terminal_cpp

lexer_cpp: $(INC_DIR)/lex/lexer.cpp
	$(GPP) $(COMPILE) $< $(OUT) lexer.o

token_cpp: $(INC_DIR)/lex/token.cpp
	$(GPP) $(COMPILE) $< $(OUT) token.o

pack_lex:
	$(LD) $(RELOC) lexer.o token.o $(OUT) lex_pack.o

strings_cpp: $(INC_DIR)/strings/strings.cpp
	$(GPP) $(COMPILE) $< $(OUT) strings.o

commands_cpp: $(INC_DIR)/terminal/commands/commands.cpp
	$(GPP) $(COMPILE) $< $(OUT) commands.o

terminal_cpp: $(INC_DIR)/terminal/terminal.cpp
	$(GPP) $(COMPILE) $< $(OUT) terminal.o

pack_terminal:
	$(LD) $(RELOC) terminal.o commands.o $(OUT) terminal_pack.o

compile: $(SRC_DIR)/main.cpp
	$(GPP) $< lex_pack.o strings.o terminal_pack.o $(OUT) $(EXE_OUT_NAME)
