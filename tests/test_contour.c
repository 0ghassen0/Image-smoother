#include "calcul_contour.h"
#include "image.h"
#include "geom2d.h"
#include<stdio.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <mode> <image1.pbm> [image2.pbm ...]\n", argv[0]);
        return 1;
    }

    // TESTs Task 5
    int i = 2;
    Tableau_Contour T;
    char *mode = argv[1];

    while (i < argc) {
        printf("\nImage %s", argv[i]);
        Image I = lire_fichier_image(argv[i]);
        //ecrire_image(I);
        T = calcule_contours(I);
        //ecrire_tableau(T);
        sequence_contour_eps(T, argv[i], mode, hauteur_image(I), largeur_image(I));
        
        // Free memory
        free_tableau_contour(&T);
        free_image(&I);
        
        i++;
    } 
    
    return 0;
}
