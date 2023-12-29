CC=gcc

# Basically writing code on insane survival mode...
ALL_WARNINGS_ARE_ERRORS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option

IDIR =./include
SDIR = ./src

ifeq ($(config), debug)
    CFLAGS = -I $(IDIR) -g -D DEBUG_CONFIG_SET $(ALL_WARNINGS_ARE_ERRORS)
else
	CFLAGS = -I $(IDIR) -O3
endif

ODIR=obj
LDIR =./lib

LIBS=-lm

_DEPS = terminal.h options.h arg_parse.h
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ  = main.o terminal.o arg_parse.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

img_to_txt: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR): 
	mkdir obj

.PHONY: test clean

test: img_to_txt
	./tests/test.sh

clean:
	rm -rf $(ODIR) img_to_txt
