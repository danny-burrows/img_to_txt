#ifndef ARG_H
#define ARG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "options.h"
#include "list.h"

#define VERSION "v0.3.0"

#define HELP "Usage: img_to_txt [OPTION]... [FILE]...                       \n\
Convert images to text. Either Ascii, Ansi (true colour),                   \n\
or solid background (Ansi true color).                                      \n\
                                                                            \n\
Available input image types:                                                \n\
    - jpg/jpeg                                                              \n\
    - png                                                                   \n\
    - bmp                                                                   \n\
    - tga                                                                   \n\
    - hdr                                                                   \n\
    - gif (static)                                                          \n\
    - pic (not tested yet)                                                  \n\
    - ppm/pnm/pgm                                                           \n\
                                                                            \n\
Options:                                                                    \n\
    -w --width          Set width and height (rows and cols) of output.     \n\
    -h --height                                                             \n\
                                                                            \n\
    -o --original-size  Output original size. (Size of image in text)       \n\
                                                                            \n\
    -a --ascii          Ascii mode. (no ansi colors)                        \n\
    -s --solid          Solid mode. (Background colors with no ascii chars) \n\
                                                                            \n\
    -t --true-color     Use true color for ANSI output.                     \n\
                        (Need true-color supported terminal)                \n\
                                                                            \n\
    -n --no-squashing   Disable standard image squashing that tries to make \n\
                        image look less elongated by chars being taller than\n\
                        they are wide.                                      \n\
                                                                            \n\
    -q --quiet          Quiet mode (suppress header)                        \n\
                                                                            \n\
    -? --help           Print help message and exit.                        \n\
                                                                            \n\
    -v --version        Print version and exit.                             \n\
                                                                            \n\
Examples:                                                                   \n\
                                                                            \n\
    img_to_txt -h 100 -w 200 -s some_img.png                                \n\
                                                                            \n\
    img_to_txt -whqt 30 some_img.png                                        \n\
                                                                            \n\
    img_to_txt -a some_img.jpg                                              \n\
                                                                            \n\
    img_to_txt -q -t -o some_img.bmp                                        \n\
"


struct FileJob {
    char * file_path;
    struct ImageOptions * file_opts;
    struct list_node list;
};

void free_job_memory(struct list_node * job_list);
int arg_parse(int argc, char ** argv, struct list_node * jobs);

#endif  /* ARG_H */
