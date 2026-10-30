/******************************************************************************
 * Main program for Image Smoothing with Bezier Curves
 * 
 * A unified CLI that replaces the individual test programs.
 * Provides a user-friendly interface for image processing with Bezier curves.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for getopt
#include <getopt.h>  // for getopt_long
#include <stdbool.h>

#include "image.h"
#include "geom2d.h"
#include "calcul_contour.h"
#include "bezier.h"

/**
 * @brief Bezier curve degree enum
 */
typedef enum { BEZIER_2, BEZIER_3 } BezierDegree;

/**
 * @brief Configuration structure for the program
 */
typedef struct Config {
    char *input_file;           /**< Input PBM file */
    char *output_file;          /**< Output file (without extension) */
    char *mode;                 /**< Drawing mode: "stroke" or "fill" */
    int threshold;              /**< Simplification threshold */
    BezierDegree degree;        /**< Bezier curve degree (2 or 3) */
    OutputFormat format;        /**< Output format */
    bool verbose;               /**< Verbose mode */
    bool help;                  /**< Show help */
    bool list_samples;          /**< List sample images */
} Config;

/**
 * @brief Default configuration values
 */
static const Config DEFAULT_CONFIG = {
    .input_file = NULL,
    .output_file = NULL,
    .mode = "stroke",
    .threshold = 1,
    .degree = BEZIER_2,
    .format = FORMAT_EPS,
    .verbose = false,
    .help = false,
    .list_samples = false
};

/**
 * @brief Print help message
 */
void print_help(const char *program_name) {
    printf("\n");
    printf("Image Smoothing with Bezier Curves - MAP401 Project\n");
    printf("==================================================\n\n");
    
    printf("Usage: %s [OPTIONS] <input.pbm>\n\n", program_name);
    
    printf("Options:\n");
    printf("  -o, --output FILE    Output file name (without extension)\n");
    printf("  -t, --threshold N    Simplification threshold (default: 1)\n");
    printf("  -d, --degree N      Bezier degree: 2 for quadratic, 3 for cubic (default: 2)\n");
    printf("  -m, --mode MODE     Drawing mode: 'stroke' or 'fill' (default: stroke)\n");
    printf("  -f, --format FMT    Output format: 'eps' or 'svg' (default: eps)\n");
    printf("  -v, --verbose       Enable verbose output\n");
    printf("  -l, --list          List available sample images\n");
    printf("  -h, --help          Show this help message\n\n");
    
    printf("Examples:\n");
    printf("  %s samples/test1.pbm -o output -t 2 -d 3 -m fill\n", program_name);
    printf("  %s samples/tete.pbm --threshold 5 --degree 2 --verbose\n", program_name);
    printf("  %s --list\n", program_name);
    printf("\n");
}

/**
 * @brief List sample images available in the samples directory
 */
void list_samples() {
    printf("\nAvailable sample images:\n");
    printf("=======================\n");
    
    // Common sample images
    const char *samples[] = {
        "test1.pbm",
        "tete.pbm",
        "cheval.pbm",
        "elephant.pbm",
        "coq.pbm",
        "gymnaste.pbm",
        "damier_4_5_1.pbm",
        "lettreZ.pbm",
        "lettre-L-cursive.pbm",
        "Asterix3.pbm",
        "Bugs_Bunny.pbm",
        "Charlot.pbm",
        "Droopy_Wolf.pbm",
        "Pink_Panther.pbm",
        "chat.pbm",
        "gai-luron.pbm",
        "ColombesDeLaPaix.pbm",
        "FranceRegions.pbm",
        "JoaquimHock-LesArbres.pbm",
        "animaux.pbm",
        "caractere2.pbm",
        "courbe_hilbert_7.pbm",
        "courbe_hilbert_8.pbm",
        "courbe_hilbert_9.pbm",
        "courbe_hilbert_10.pbm",
        "dessin-delius.pbm",
        "deux-des.pbm",
        "elephant-gotlib.pbm",
        "goudyini-A.pbm",
        "image1_poly.pbm",
        "image2_poly.pbm",
        "image_ex_poly.pbm",
        "image_poly_tache6.pbm",
        "labyrinthe.pbm",
        "map401.pbm",
        "papillon2.pbm",
        "tache5_test1.pbm",
        "tache5_test2.pbm",
        "tst_contour1.pbm",
        NULL
    };
    
    for (int i = 0; samples[i] != NULL; i++) {
        printf("  - %s\n", samples[i]);
    }
    printf("\n");
}

/**
 * @brief Parse command line arguments
 */
Config parse_arguments(int argc, char **argv) {
    Config config = DEFAULT_CONFIG;
    
    int opt;
    int option_index = 0;
    
    // Long options
    static struct option long_options[] = {
        {"output", required_argument, 0, 'o'},
        {"threshold", required_argument, 0, 't'},
        {"degree", required_argument, 0, 'd'},
        {"mode", required_argument, 0, 'm'},
        {"format", required_argument, 0, 'f'},
        {"verbose", no_argument, 0, 'v'},
        {"list", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "o:t:d:m:f:vhl", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'o':
                config.output_file = optarg;
                break;
            case 't':
                config.threshold = atoi(optarg);
                if (config.threshold < 0) {
                    fprintf(stderr, "Error: Threshold must be non-negative\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'd':
                if (strcmp(optarg, "2") == 0) {
                    config.degree = BEZIER_2;
                } else if (strcmp(optarg, "3") == 0) {
                    config.degree = BEZIER_3;
                } else {
                    fprintf(stderr, "Error: Degree must be 2 or 3\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                if (strcmp(optarg, "stroke") == 0 || strcmp(optarg, "fill") == 0) {
                    config.mode = optarg;
                } else {
                    fprintf(stderr, "Error: Mode must be 'stroke' or 'fill'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'f':
                if (strcmp(optarg, "eps") == 0) {
                    config.format = FORMAT_EPS;
                } else if (strcmp(optarg, "svg") == 0) {
                    config.format = FORMAT_SVG;
                } else {
                    fprintf(stderr, "Error: Format must be 'eps' or 'svg'\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v':
                config.verbose = true;
                break;
            case 'l':
                config.list_samples = true;
                break;
            case 'h':
                config.help = true;
                break;
            default:
                print_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    // Check for non-option arguments
    if (optind < argc) {
        config.input_file = argv[optind];
        
        // Only one input file supported for now
        if (optind + 1 < argc) {
            fprintf(stderr, "Error: Only one input file supported at a time\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return config;
}

/**
 * @brief Validate configuration
 */
bool validate_config(const Config *config) {
    if (config->help) {
        return true;
    }
    
    if (config->list_samples) {
        return true;
    }
    
    if (config->input_file == NULL) {
        fprintf(stderr, "Error: No input file specified\n");
        return false;
    }
    
    // Check if input file exists
    FILE *f = fopen(config->input_file, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", config->input_file);
        return false;
    }
    fclose(f);
    
    return true;
}

/**
 * @brief Process image with quadratic Bezier curves
 */
void process_bezier2(const Config *config, Image I) {
    Tableau_Contour T = calcule_contours(I);
    
    if (config->verbose) {
        printf("Processing with quadratic Bezier curves (degree 2)...\n");
    }
    
    seq_bezier2 S;
    Tableau_seq_bezier2 D = creer_tab_seq_bezier2_vide();
    
    for (unsigned int j = 0; j < T.taille; j++) {
        if (config->verbose) {
            printf("  Processing contour %d/%d...\r", j+1, T.taille);
            fflush(stdout);
        }
        S = simpl_douglas_peucker_bezier2_rec(T.tab[j], 0, T.tab[j].taille-1, config->threshold);
        D = ajouter_elem_tab_seq_bezier2(D, S);
    }
    
    if (config->verbose) {
        printf("  Processing contour %d/%d... Done!\n", T.taille, T.taille);
    }
    
    // Generate output filename
    char output_filename[256];
    if (config->output_file) {
        snprintf(output_filename, sizeof(output_filename), "%s_bezier2", config->output_file);
    } else {
        // Extract base name from input file
        const char *last_slash = strrchr(config->input_file, '/');
        const char *base = last_slash ? last_slash + 1 : config->input_file;
        const char *last_dot = strrchr(base, '.');
        int base_len = last_dot ? last_dot - base : strlen(base);
        snprintf(output_filename, sizeof(output_filename), "%.*s_bezier2", base_len, base);
    }
    
    // Output based on format
    if (config->format == FORMAT_EPS) {
        strcat(output_filename, ".eps");
        sequence_bezier2_eps(D, output_filename, config->mode, hauteur_image(I), largeur_image(I));
    } else if (config->format == FORMAT_SVG) {
        strcat(output_filename, ".svg");
        sequence_bezier2_svg(D, output_filename, config->mode, hauteur_image(I), largeur_image(I));
    }
    
    affiche_nb_bezier2(D);
    
    // Free memory
    free_tableau_seq_bezier2(&D);
    free_tableau_contour(&T);
}

/**
 * @brief Process image with cubic Bezier curves
 */
void process_bezier3(const Config *config, Image I) {
    Tableau_Contour T = calcule_contours(I);
    
    if (config->verbose) {
        printf("Processing with cubic Bezier curves (degree 3)...\n");
    }
    
    seq_bezier3 S;
    Tableau_seq_bezier3 D = creer_tab_seq_bezier3_vide();
    
    for (unsigned int j = 0; j < T.taille; j++) {
        if (config->verbose) {
            printf("  Processing contour %d/%d...\r", j+1, T.taille);
            fflush(stdout);
        }
        S = simpl_douglas_peucker_bezier3_rec(T.tab[j], 0, T.tab[j].taille-1, config->threshold);
        D = ajouter_elem_tab_seq_bezier3(D, S);
    }
    
    if (config->verbose) {
        printf("  Processing contour %d/%d... Done!\n", T.taille, T.taille);
    }
    
    // Generate output filename
    char output_filename[256];
    if (config->output_file) {
        snprintf(output_filename, sizeof(output_filename), "%s_bezier3", config->output_file);
    } else {
        // Extract base name from input file
        const char *last_slash = strrchr(config->input_file, '/');
        const char *base = last_slash ? last_slash + 1 : config->input_file;
        const char *last_dot = strrchr(base, '.');
        int base_len = last_dot ? last_dot - base : strlen(base);
        snprintf(output_filename, sizeof(output_filename), "%.*s_bezier3", base_len, base);
    }
    
    // Output based on format
    if (config->format == FORMAT_EPS) {
        strcat(output_filename, ".eps");
        sequence_bezier3_eps(D, output_filename, config->mode, hauteur_image(I), largeur_image(I));
    } else if (config->format == FORMAT_SVG) {
        strcat(output_filename, ".svg");
        sequence_bezier3_svg(D, output_filename, config->mode, hauteur_image(I), largeur_image(I));
    }
    
    affiche_nb_bezier3(D);
    
    // Free memory
    free_tableau_seq_bezier3(&D);
    free_tableau_contour(&T);
}

/**
 * @brief Process image with simple contour extraction (no Bezier)
 */
void process_contour(const Config *config, Image I) {
    Tableau_Contour T = calcule_contours(I);
    
    if (config->verbose) {
        printf("Processing with contour extraction only...\n");
    }
    
    // Generate output filename
    char output_filename[256];
    if (config->output_file) {
        snprintf(output_filename, sizeof(output_filename), "%s_contour.eps", config->output_file);
    } else {
        // Extract base name from input file
        const char *last_slash = strrchr(config->input_file, '/');
        const char *base = last_slash ? last_slash + 1 : config->input_file;
        const char *last_dot = strrchr(base, '.');
        int base_len = last_dot ? last_dot - base : strlen(base);
        snprintf(output_filename, sizeof(output_filename), "%.*s_contour.eps", base_len, base);
    }
    
    sequence_contour_eps(T, output_filename, config->mode, hauteur_image(I), largeur_image(I));
    
    // Free memory
    free_tableau_contour(&T);
}

/**
 * @brief Main entry point
 */
int main(int argc, char **argv) {
    Config config = parse_arguments(argc, argv);
    
    // Show help if requested
    if (config.help) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }
    
    // List samples if requested
    if (config.list_samples) {
        list_samples();
        return EXIT_SUCCESS;
    }
    
    // Validate configuration
    if (!validate_config(&config)) {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }
    
    // Load input image
    if (config.verbose) {
        printf("Loading image: %s\n", config.input_file);
    }
    
    Image I = lire_fichier_image(config.input_file);
    
    if (config.verbose) {
        printf("Image loaded: %d x %d pixels\n", 
               largeur_image(I), hauteur_image(I));
    }
    
    // Process based on degree
    switch (config.degree) {
        case BEZIER_2:
            process_bezier2(&config, I);
            break;
        case BEZIER_3:
            process_bezier3(&config, I);
            break;
    }
    
    // Free image memory
    free_image(&I);
    
    if (config.verbose) {
        printf("Processing complete!\n");
    }
    
    return EXIT_SUCCESS;
}
