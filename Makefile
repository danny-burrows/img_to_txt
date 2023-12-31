CC=gcc

extra_warnings_as_errors = -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef -fdiagnostics-show-option -fsanitize=signed-integer-overflow

ifeq ($(config), debug)
	flags = -isystem ./include -g -D DEBUG_CONFIG_SET $(extra_warnings_as_errors)
else
	flags = -isystem ./include -O3
endif

source_dir = ./src
headers = $(wildcard $(source_dir)/*.h)
object_dir = ./obj
objects = $(patsubst $(source_dir)/%.c,$(object_dir)/%.o,$(wildcard $(source_dir)/*.c))
libs = -lm

img_to_txt: $(objects)
	$(CC) -o $@ $^ $(flags) $(libs)

$(object_dir)/%.o: $(source_dir)/%.c $(headers) | $(object_dir)
	$(CC) -c -o $@ $< $(flags)

$(object_dir):
	mkdir $(object_dir)

.PHONY: test clean

test:
	$(MAKE) clean img_to_txt config=debug
	./tests/test.sh

clean:
	rm -rf $(object_dir) img_to_txt
