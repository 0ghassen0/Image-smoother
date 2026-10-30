#include "calcul_contour.h"
#include "image.h"
#include "geom2d.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <mode> <image1.pbm> [image2.pbm ...]\n", argv[0]);
        fprintf(stderr, "Note: Program will prompt for threshold value\n");
        return 1;
    }

    // Usage: d image1 image2 ...
    int i = 2;
    Tableau_Contour T;

    int seuil;
    printf("\nThreshold d = ");
    scanf("%d", &seuil);
    char *mode = argv[1];

    while (i < argc) {
        printf("\nImage %s", argv[i]);
        Image I = lire_fichier_image(argv[i]);
        T = calcule_contours(I);
        //ecrire_tableau_contours(T);

        simpl_doug(T, seuil);

        sequence_contour_eps(T, argv[i], mode, hauteur_image(I), largeur_image(I));
        //affiche_nb_segments_tab_contours(T);
        
        // Free memory
        free_tableau_contour(&T);
        free_image(&I);
        
        i++;
    } 
    
    return 0;
}
