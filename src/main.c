/*
NOTE: Eventually true color should not be the default.

TODO:
    - Debug what happens when piped to a file.
    - Add support for long args.
    - Test all image types in stb_image.
    - Improve the scale for plane ASCII output.
    - Windows support.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Disable some warnings for stb headers.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ext/stb_image_resize.h"
#pragma GCC diagnostic pop

#include "terminal.h"


#define VERSION "v0.1.0"
#define MACRO_STRLEN(s) (sizeof(s) / sizeof(s[0]))
#define STR_EQUAL 0

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

// Some preprocessor magic to generate an enum and string array with the same items.
#define OUTPUT_MODES(MODE) \
        MODE(ANSI)         \
        MODE(SOLID_ANSI)   \
        MODE(ASCII)        \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum OutputModes {OUTPUT_MODES(GENERATE_ENUM)};
static const char * OutputModeStr[] = {OUTPUT_MODES(GENERATE_STRING)}; // Strings used for output.

typedef struct {
    unsigned int width;
    unsigned int height;

    enum OutputModes output_mode;

    bool original_size;
    bool true_color;
    bool squashing_enabled;
    bool suppress_header;
} ImageOptions;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} PixelData;

typedef struct _FileJob {
    char * path;
    struct _FileJob * nextJob;
} FileJob;

char scale[] = "$@&B%8WM#ZO0QoahkbdpqwmLCJUYXIjft/\\|()1{}[]l?zcvunxr!<>i;:*-+~_,\"^`'. ";
unsigned int brightness_levels = MACRO_STRLEN(scale) - 2;


unsigned char luminanceFromRGB(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned char) (0.2126 * r + 0.7152 * g + 0.0722 * b);
}


unsigned char calc_ascii_char(PixelData * p) {
    // Calc luminace and use to find Ascii char.
    unsigned char ch = luminanceFromRGB(p->r, p->g, p->b);
    int rescaled = ch * brightness_levels / 256;
    return scale[brightness_levels - rescaled];
}


int read_and_convert(char * filename, ImageOptions * options) {
    int rwidth, rheight, rchannels;
    unsigned char * read_data = stbi_load(filename, &rwidth, &rheight, &rchannels, 3);

    if (read_data == NULL) {
        fprintf(stderr, "Error reading image data!\n\n");
        return -1;
    }

    unsigned int desired_width, desired_height;
    if (options->original_size) {
        desired_width = rwidth;
        desired_height = rheight;
    } else {
        int calc_w, calc_h;
        get_ideal_image_size(&calc_w, &calc_h, rwidth, rheight, options->squashing_enabled);

        desired_width = options->width == 0 ? (unsigned)calc_w : options->width;
        desired_height = options->height == 0 ? (unsigned)calc_h : options->height;
    }

    // Check for and do any needed image resizing...
    PixelData * data;
    if (desired_width != (unsigned)rwidth || desired_height != (unsigned)rheight) {
                                        //3 * uint8 for RGB!
        unsigned char * new_data = malloc(3 * sizeof(unsigned char) * desired_width * desired_height);
        int r = stbir_resize_uint8(
            read_data, rwidth, rheight, 0,
            new_data, desired_width, desired_height, 0, 3
        );

        if (r == 0) {
            perror("Error resizing image:");
            return -1;
        }
        stbi_image_free(read_data); // Free read_data.
        data = (PixelData *)new_data;
    } else {
        data = (PixelData *)read_data; 
    }

    // Print header if required...
    if (options->suppress_header == false) {
        printf("Img Type: %s\nTrue Color: %d\nSize: %dx%d\n", OutputModeStr[options->output_mode], options->true_color, desired_width, desired_height);
    }

    for (unsigned int d = 0; d < desired_width * desired_height; d++)
    {
        if (d % desired_width == 0){
            printf("\n");
        }

        PixelData * c = data + d;

        switch (options->output_mode)
        {
        case ASCII:
            printf("%c", calc_ascii_char(c));
            break;
        case ANSI:
            printf("\033[38;2;%03u;%03u;%03um%c", c->r, c->g, c->b, calc_ascii_char(c));
            break;
        case SOLID_ANSI:
            // NOTE: Printing two chars instead of one to help with narrow chars.
            printf("\033[48;2;%03u;%03u;%03um \033[0m", c->r, c->g, c->b);
            break;
        default:
            break;
        }
        printf("\033[0m");
    }
    printf("\n");

    stbi_image_free(data);
    return 0;
}


void version_and_exit(void) {
    printf("img_to_text %s\n", VERSION);
    exit(EXIT_SUCCESS);
}


void help_and_exit(void) {
    printf("%s\n", HELP);
    exit(EXIT_SUCCESS);
}


int get_opt_int_param(int argc, char ** argv, int opt) {
    int scan_result;
    int param;

    if (opt == argc - 1) {
        fprintf(stderr, "Not enough arguments to read option parameter!\n");
        return -1;
    }

    scan_result = sscanf( argv[opt + 1], "%d", &param );
    if (!scan_result) {
        fprintf(stderr, "Couldn't read option parameter!\n");
        return -1;
    }

#ifdef DEBUG_CONFIG_SET
    printf("[DEBUG] Arg: %d\n", param);
#endif

    return param;
}


int main(int argc, char ** argv) {

#ifdef DEBUG_CONFIG_SET
    printf("[DEBUG] ASCII Brightness levels: %d\n", brightness_levels);
#endif

    FileJob * jobs = NULL;

    // Default options
    ImageOptions opts = {
        .output_mode = ANSI,
        .original_size = false,
        .true_color = true,
        .squashing_enabled = true,
        .suppress_header = false
    };

    // Parse arguments
    bool stop_opt = false;
    for (int opt = 1; opt < argc; opt++) {
        
        // If not an option then add to job linked list.
        if (argv[opt][0] != '-' || stop_opt == true) {
            FileJob * job = (FileJob *) malloc(sizeof(FileJob));

            if (job == NULL) {
                perror("Couldn't allocate memory!");
                return EXIT_FAILURE;
            }

            job->path = argv[opt];
            job->nextJob = jobs;

            jobs = job;
            continue;
        }

        if ( strcmp(argv[opt], "--") == STR_EQUAL ) {
            stop_opt = true;
            continue;
        }

#ifdef DEBUG_CONFIG_SET
        printf("[DEBUG] Opt[%d]: %s\n", opt, argv[opt]);
#endif

        int width;
        int height;
        bool read_param = false;
        for (size_t c = 1; c < strlen(argv[opt]); c++) {
            switch (argv[opt][c]) {
                
                case '?':
                    help_and_exit();
                    return EXIT_SUCCESS;
                case 'v':
                    version_and_exit();
                    return EXIT_SUCCESS;
                
                case '-':
                    //parse_long_arg(argv[opt]);
                    break;

                case 'w':
                    width = get_opt_int_param(argc, argv, opt);
                    if (width < 0) {
                        fprintf(stderr, "[ERR] Width is non-zero!\n");
                        return EXIT_FAILURE;
                    }
                    opts.width = (unsigned int)width;
                    read_param = true;
                    break;
                case 'h':
                    height = get_opt_int_param(argc, argv, opt);
                    if (height < 0) {
                        fprintf(stderr, "[ERR] Height is non-zero!\n");
                        return EXIT_FAILURE;
                    }
                    opts.height = (unsigned int)height;
                    read_param = true;
                    break;

                case 'a':
                    opts.output_mode = ASCII;
                    opts.true_color = false;
                    break;
                case 's':
                    opts.output_mode = SOLID_ANSI;
                    opts.true_color = true;
                    break;
                case 'o':
                    opts.original_size = true;
                    break;
                case 't':
                    opts.true_color = true;
                    break;
                case 'n':
                    opts.squashing_enabled = false;
                    break;
                case 'q':
                    opts.suppress_header = true;
                    break;
                default:
                    printf("Unknown option: %s\n", argv[opt]);
                    return EXIT_FAILURE;
            }
        }
        if (read_param) opt++;
    }

    if (jobs == NULL) {
         fprintf(stderr, "No files to convert! (-? for help text)\n");
         return EXIT_FAILURE;
    }

    // Parse file paths and do all jobs.
    for (FileJob * read_jobs = jobs; read_jobs != NULL; read_jobs = read_jobs->nextJob) {
        if (opts.suppress_header == false) {printf("Converting File: %s\n", read_jobs->path);}
    
        int r = read_and_convert(read_jobs->path, &opts);
        if (r == -1) {
            fprintf(stderr, "Failed to convert image: %s\n", read_jobs->path);
        }
        printf("\n");
    }

    // Free job memory!
    FileJob * f = jobs;
    while (f != NULL) {
        FileJob * next = f->nextJob;
        free(f);
        f = next;
    }

    return EXIT_SUCCESS;
}
