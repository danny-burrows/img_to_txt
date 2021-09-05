#ifndef IMG_TO_TXT_ARG_H
#define IMG_TO_TXT_ARG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "options.h"

#define STR_EQUAL 0

#define VERSION "v0.1.1"

#define HELP "Usage: img_to_txt [OPTION]... [FILE]...                       \n\
Convert images to text. Either Ascii, Ansi (true colour),                   \n\
or solid background (Ansi true color).                                      \n\
                                                                            \n\
Note: Long arg (--example) not yet supported.                               \n\
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
    -w <width>          Set width and height (rows and cols) of output.     \n\
    -h <height>                                                             \n\
                                                                            \n\
    -o                  Output original size. (Size of image in text)       \n\
                                                                            \n\
    -a                  Ascii mode. (no ansi colors)                        \n\
    -s                  Solid mode. (Background colors with no ascii chars) \n\
                                                                            \n\
    -t                  Use true color for ANSI output.                     \n\
                        (Need true-color supported terminal)                \n\
                                                                            \n\
    -n                  Disable standard image squashing that tries to make \n\
                        image look less elongated by chars being taller than\n\
                        they are wide.                                      \n\
                                                                            \n\
    -q                  Quiet mode (suppress header)                        \n\
                                                                            \n\
    -?                  Print help message and exit.                        \n\
                                                                            \n\
    -v                  Print version and exit.                             \n\
                                                                            \n\
Examples:                                                                   \n\
                                                                            \n\
    img_to_txt -h 100 -w 200 -s some_img.png                                \n\
                                                                            \n\
    img_to_txt -whqt 30 some_img.png                                        \n\
                                                                            \n\
    img_to_txt some_img.jpg -a                                              \n\
                                                                            \n\
    img_to_txt -q -t -o some_img.bmp                                        \n\
"


typedef struct _FileJob {
    char * path;
    struct _FileJob * nextJob;
} FileJob;

FileJob * arg_parse(int argc, char ** argv, ImageOptions * opts);

#endif