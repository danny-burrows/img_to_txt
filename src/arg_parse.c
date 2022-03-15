#include "arg_parse.h"

#define MAX_IMG_SIZE 25000

static void print_version(void) {
    printf("img_to_text %s\n", VERSION);
}

static void print_help(void) {
    printf("%s\n", HELP);
}

static int get_opt_int_param(int argc, char ** argv, int opt) {
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

void free_job_memory(FileJob * job_list) {
    while (job_list != NULL) {
        free(job_list->file_opts);
        FileJob * next = job_list->next_job;
        free(job_list);
        job_list = next;
    }
}

static int parse_short_args(int argc, char ** argv, int opt, ImageOptions * opts) {
    unsigned int width;
    unsigned int height;
    int read_param = 0;

    for (size_t c = 1; c < strlen(argv[opt]); c++) {
        switch (argv[opt][c]) {

            case '?':
                print_help();
                return -1;
            case 'v':
                print_version();
                return -1;

            case 'w':
                width = get_opt_int_param(argc, argv, opt);
                if (width > MAX_IMG_SIZE) {
                    fprintf(stderr, "[ERR] Width exceeds maximum image size!\n");
                    return -1;
                }
                opts->width = width;
                read_param = 1;
                break;
            case 'h':
                height = get_opt_int_param(argc, argv, opt);
                if (height > MAX_IMG_SIZE) {
                    fprintf(stderr, "[ERR] Height exceeds maximum image size!\n");
                    return -1;
                }
                opts->height = height;
                read_param = 1;
                break;

            case 'a':
                opts->output_mode = ASCII;
                opts->true_color = false;
                break;
            case 's':
                opts->output_mode = SOLID_ANSI;
                opts->true_color = true;
                break;
            case 'o':
                opts->original_size = true;
                break;
            case 't':
                opts->true_color = true;
                break;
            case 'n':
                opts->squashing_enabled = false;
                break;
            case 'q':
                opts->suppress_header = true;
                break;
            default:
                printf("Unknown option: %s\n", argv[opt]);
                return -1;
        }
    }
    return read_param;
}

static int parse_long_arg(int argc, char ** argv, int opt, ImageOptions * opts) {
    unsigned int width;
    unsigned int height;
    int read_param = 0;

    if ( !strcmp("--help", argv[opt]) ) {
        print_help();
        return -1;

    } else if ( !strcmp("--version", argv[opt]) ) {
        print_version();
        return -1;

    } else if ( !strcmp("--width", argv[opt]) ) {
        width = get_opt_int_param(argc, argv, opt);
        if (width > MAX_IMG_SIZE) {
            fprintf(stderr, "[ERR] Width exceeds maximum image size!\n");
            return -1;
        }
        opts->width = width;
        read_param = 1;

    } else if ( !strcmp("--height", argv[opt]) ) {
        height = get_opt_int_param(argc, argv, opt);
        if (height > MAX_IMG_SIZE) {
            fprintf(stderr, "[ERR] Height exceeds maximum image size!\n");
            return -1;
        }
        opts->height = height;
        read_param = 1;
        
    } else if ( !strcmp("--original-size", argv[opt]) ) {
        opts->original_size = true;
        
    } else if ( !strcmp("--ascii", argv[opt]) ) {
        opts->output_mode = ASCII;
        opts->true_color = false;
        
    } else if ( !strcmp("--solid", argv[opt]) ) {
        opts->output_mode = SOLID_ANSI;
        opts->true_color = true;
        
    } else if ( !strcmp("--true-color", argv[opt]) || !strcmp("--true-colour", argv[opt]) ) {
        opts->true_color = true;
        
    } else if ( !strcmp("--no-squashing", argv[opt]) ) {
        opts->squashing_enabled = false;
        
    } else if ( !strcmp("--quiet", argv[opt]) ) {
        opts->suppress_header = true;

    } else {
        printf("Unknown option: %s\n", argv[opt]);
        return -1;
    }

    return read_param;
}

/**
 * Returns a pointer to FileJobs linked list or NULL.
 */
FileJob * arg_parse(int argc, char ** argv) {
    // HEAD of FileJob struct list
    FileJob * jobs = NULL;
    int read_param = 0;

    // If a user give mutiple files without any given options before,
    // then files that have not any options would get a default_opts.
    ImageOptions default_opts = {
        .output_mode = ANSI,
        .original_size = false,
        .true_color = true,
        .squashing_enabled = true,
        .suppress_header = false
    };
    ImageOptions opts = default_opts;

    // Go through all args after program name.
    for (int opt = 1; opt < argc; opt++) {
#ifdef DEBUG_CONFIG_SET
        printf("[DEBUG] Opt[%d]: %s\n", opt, argv[opt]);
#endif

        // If not an opt then add to linked list.
        if (argv[opt][0] != '-') {
            FileJob * job = (FileJob *) malloc(sizeof(FileJob));
            if (job == NULL) {
                perror("arg_parse: FileJob - Couldn't allocate memory!");
                return NULL;
            }

            // Initialize the ImageOptions with opts
            job->file_opts = (ImageOptions *) malloc(sizeof(ImageOptions));
            if (job->file_opts == NULL) {
                perror("arg_parse: ImageOptions - Couldn't allocate memory!");
                return NULL;
            }
            
            memcpy(job->file_opts, &opts, sizeof(ImageOptions));
            opts = default_opts;    // Prevent options from remaining prior opts value

            job->file_path = argv[opt];
            job->next_job = jobs;
            jobs = job;
            continue;
        } else if (argv[opt][0] == '-' && argv[opt][1] == '-') {
            read_param = parse_long_arg(argc, argv, opt, &opts);
        } else if (argv[opt][0] == '-' && argv[opt][1] != '-') {
            read_param = parse_short_args(argc, argv, opt, &opts);
        }

        if (read_param == -1) {
            return NULL;
        } else {
            // Skip arguments if there were additionally used one.
            opt += read_param;
        }
    }

    if (jobs == NULL)
        fprintf(stderr, "No files to convert! (-? for help text)\n");
    return jobs;
}
