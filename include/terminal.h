#ifndef IMG_TO_TXT_TERMINAL_H
#define IMG_TO_TXT_TERMINAL_H

#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>

void get_term_size(int *restrict width, int *restrict height);

void get_ideal_image_size(int *restrict width, int *restrict height, const int image_width, const int image_height, bool squashing_enabled);

#endif
