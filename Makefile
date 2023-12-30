CC=gcc

ENABLE_EXTRA_WARNINGS_AS_ERRORS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -fdiagnostics-show-option -fsanitize=signed-integer-overflow
DISABLED_WARNINGS = -Wno-unused

IDIR =./include
SDIR = ./src

ifeq ($(config), debug)
    CFLAGS = -isystem $(IDIR) -g -D DEBUG_CONFIG_SET $(ENABLE_EXTRA_WARNINGS_AS_ERRORS)
else
	CFLAGS = -isystem $(IDIR) -O3
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
