GPP := g++
COMPILE := -c
OUT := -o

EXE_OUT_NAME := ranch
ifeq ($(OS),Windows_NT)
	EXE_OUT_NAME := $(EXE_OUT_NAME).exe
endif

SRC_DIR := ./src
INC_DIR := ./include

all: depends compile
depends: includes
includes: strings terminal
strings: strings_cpp
terminal: commands terminal_headers
commands: commands_headers
commands_headers: commands_cpp
terminal_headers: terminal_cpp

strings_cpp: $(INC_DIR)/strings/strings.cpp
	$(GPP) $(COMPILE) $< $(OUT) strings.o

commands_cpp: $(INC_DIR)/terminal/commands/commands.cpp
	$(GPP) $(COMPILE) $< $(OUT) commands.o

terminal_cpp: $(INC_DIR)/terminal/terminal.cpp
	$(GPP) $(COMPILE) $< $(OUT) terminal.o

compile: $(SRC_DIR)/main.cpp
	$(GPP) $< strings.o commands.o terminal.o $(OUT) $(EXE_OUT_NAME)
