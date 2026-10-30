#include "geom2d.h"
#include "bezier.h"
#include "image.h"
#include <stdio.h>

int main(int argc, char **argv){

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <mode> <image1.pbm> [image2.pbm ...]\n", argv[0]);
        fprintf(stderr, "Note: Program will prompt for threshold value\n");
        return 1;
    }

    int i = 2;
    Tableau_Contour T;
    seq_bezier2 S;
    Tableau_seq_bezier2 D;
    D.taille = 0;
    int seuil;
    printf("\nThreshold d = ");
    scanf("%d", &seuil);
    char *mode = argv[1];

    while (i < argc){
        printf("\nImage %s", argv[i]);
        Image I = lire_fichier_image(argv[i]);
        //ecrire_image(I);

        T = calcule_contours(I);
        //ecrire_tableau_contours(T);
        
        for (int j = 0; j < T.taille; j++) {
            S = simpl_douglas_peucker_bezier2_rec(T.tab[j], 0, T.tab[j].taille-1, seuil);
            D = ajouter_elem_tab_seq_bezier2(D, S);
        }

        //ecrire_seq_bezier2(S);
        //ecrire_tableau_seq_bezer2(D);

        sequence_bezier2_eps(D, argv[i], mode, hauteur_image(I), largeur_image(I));
        affiche_nb_bezier2(D); 
        
        // Free memory
        free_tableau_seq_bezier2(&D);
        free_tableau_contour(&T);
        free_image(&I);
        
        D = creer_tab_seq_bezier2_vide();
        i++;
    } 
    
    return 0;
}
