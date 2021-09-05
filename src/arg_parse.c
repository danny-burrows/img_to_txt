#include "arg_parse.h"

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

FileJob * arg_parse(int argc, char ** argv, ImageOptions * opts) {
    // Returns a pointer to FileJobs array or NULL.

    FileJob * jobs = NULL;

    // Parse arguments
    bool stop_opt = false;
    for (int opt = 1; opt < argc; opt++) {
        
        // If not an option then add to job linked list.
        if (argv[opt][0] != '-' || stop_opt == true) {
            FileJob * job = (FileJob *) malloc(sizeof(FileJob));

            if (job == NULL) {
                perror("Couldn't allocate memory!");
                return NULL;
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
                    print_help();
                    return NULL;
                case 'v':
                    print_version();
                    return NULL;
                
                case '-':
                    //parse_long_arg(argv[opt]);
                    break;

                case 'w':
                    width = get_opt_int_param(argc, argv, opt);
                    if (width < 0) {
                        fprintf(stderr, "[ERR] Width is non-zero!\n");
                        return NULL;
                    }
                    opts->width = (unsigned int)width;
                    read_param = true;
                    break;
                case 'h':
                    height = get_opt_int_param(argc, argv, opt);
                    if (height < 0) {
                        fprintf(stderr, "[ERR] Height is non-zero!\n");
                        return NULL;
                    }
                    opts->height = (unsigned int)height;
                    read_param = true;
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
                    return NULL;
            }
        }
        if (read_param) opt++;
    }
    return jobs;
}