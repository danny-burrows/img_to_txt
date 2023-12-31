CC=gcc

ENABLE_EXTRA_WARNINGS_AS_ERRORS = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -fdiagnostics-show-option -fsanitize=signed-integer-overflow

ifeq ($(config), debug)
	CFLAGS = -isystem ./include -g -D DEBUG_CONFIG_SET $(ENABLE_EXTRA_WARNINGS_AS_ERRORS)
else
	CFLAGS = -isystem ./include -O3
endif

SDIR=./src
ODIR=./obj
LIBS=-lm

HDRS = $(wildcard $(SDIR)/*.h)
OBJS = $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(wildcard $(SDIR)/*.c))

img_to_txt: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c $(HDRS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir $(ODIR)

.PHONY: test clean

test:
	$(MAKE) clean img_to_txt config=debug
	./tests/test.sh

clean:
	rm -rf $(ODIR) img_to_txt
