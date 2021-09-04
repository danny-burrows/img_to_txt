#include "terminal.h"

#define TERM_PADDING_X 8
#define TERM_PADDING_Y 4

void get_term_size(int *restrict width, int *restrict height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *height = (int)w.ws_row;
    *width = (int)w.ws_col;
}

void get_ideal_image_size(int *restrict width, int *restrict height, const int image_width, const int image_height, bool squashing_enabled) {
    *width = squashing_enabled ? image_width * 2 : image_width; // <- NOTE: This is to offset narrow chars. 
    *height = image_height;
    double aspect_ratio = (double)*width / (double)*height;

    int term_w, term_h;
    get_term_size(&term_w, &term_h);

    term_h -= TERM_PADDING_Y; // Some offsets for screen padding.
    term_w -= TERM_PADDING_X;

    bool solving = true;

    while (solving) {
        solving = false;

        if (*width > term_w) {
            *width -= TERM_PADDING_X;
            *height = (int)(((double)*width) / aspect_ratio);
            solving = true;
        }

        if (*height > term_h) {
            *height -= TERM_PADDING_Y;
            *width = (int)(((double)*height) * aspect_ratio);
            solving = true;
        }
    }

}