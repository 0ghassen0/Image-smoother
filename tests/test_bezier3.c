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

    // Test with 2 points, n=1
    Contour Y = creer_liste_Point_vide();

    Point a = set_point(4.0, 3.0);
    Point b = set_point(7.0, 5.0);

    Y = ajouter_element_liste_Point(Y, a);
    Y = ajouter_element_liste_Point(Y, b);

    // Test approx_bezier3 with 2 points
    approx_bezier3(Y, 0, 1);
    free_contour(Y);

    // Test with multiple points, n>=2
    Y = creer_liste_Point_vide();

    Y = ajouter_element_liste_Point(Y, set_point(0,0));
    Y = ajouter_element_liste_Point(Y, set_point(1,0));
    Y = ajouter_element_liste_Point(Y, set_point(1,1));
    Y = ajouter_element_liste_Point(Y, set_point(1,2));
    Y = ajouter_element_liste_Point(Y, set_point(2,2));
    Y = ajouter_element_liste_Point(Y, set_point(3,2));
    Y = ajouter_element_liste_Point(Y, set_point(3,3));
    Y = ajouter_element_liste_Point(Y, set_point(4,3));
    Y = ajouter_element_liste_Point(Y, set_point(5,3));

    approx_bezier3(Y, 0, 8);
    free_contour(Y);
    
    int i = 2;
    Tableau_Contour T;
    seq_bezier3 S;
    Tableau_seq_bezier3 D;
    D.taille = 0;
    char *mode = argv[1];

    int seuil;
    printf("\nThreshold d = ");
    scanf("%d", &seuil);

    while (i < argc){
        printf("\nImage %s", argv[i]);
        Image I = lire_fichier_image(argv[i]);
        //ecrire_image(I);

        T = calcule_contours(I);
        //ecrire_tableau_contours(T);

        for (int j = 0; j < T.taille; j++) {
            S = simpl_douglas_peucker_bezier3_rec(T.tab[j], 0, T.tab[j].taille-1, seuil);
            D = ajouter_elem_tab_seq_bezier3(D, S);
        }

        //ecrire_seq_bezier3(S);
        //ecrire_tableau_seq_bezer3(D);

        sequence_bezier3_eps(D, argv[i], mode, hauteur_image(I), largeur_image(I));
        affiche_nb_bezier3(D); 
        
        // Free memory
        free_tableau_seq_bezier3(&D);
        free_tableau_contour(&T);
        free_image(&I);
        
        D = creer_tab_seq_bezier3_vide();
        i++;
    } 
    
    return 0;
}
