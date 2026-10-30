#include "calcul_contour.h"
#include "geom2d.h"
#include <stdio.h>

Point depart_contour(Image I) {

    int l = largeur_image(I);
	int h = hauteur_image(I);
	UINT x = 1;
	UINT y = 1;

	while (y<=h)
	{
		while (x<=l){

			if (get_pixel_image(I, x, y) == NOIR && get_pixel_image(I, x, y-1) == BLANC)  {
                return set_point(x-1, y-1);
			}
			x++;
		}
		y++;
		x=1;
	}

    return set_point(0, 0); //Pas de point depart trouve
}

Tableau_Contour calcule_contours(Image I) {
    //Calcul des pixels candidats
    int l = largeur_image(I);
	int h = hauteur_image(I);
	UINT x = 1;
	UINT y = 1;
    Tableau_Contour T = creer_tableau_contour_vide();

    Image M = creer_image(l, h);  //Image masque
    
	while (y<=h)
	{
        while (x<=l){
            
            if (get_pixel_image(I, x, y) == NOIR && get_pixel_image(I, x, y-1) == BLANC)  {
                set_pixel_image(M, x, y, NOIR);

			}
			x++;
		}
		y++;
		x=1;
	}

    //Calcul des contours
    int nb_contours = 0;
    int points=0;
    int segments = 0;
    bool masque_vide = image_blanche(M);
    Contour C;

    while (!masque_vide) {

        Point depart = depart_contour(M);
        nb_contours ++;
       // printf("\nContour %d : ", nb_contours);
        C = creer_liste_Point_vide();

        C = memoriser_position(I, depart, C, M);
        T = ajouter_element_tab_contour(T, C);
        // ecrire_contour(C);
        points += C.taille;
        
        masque_vide = image_blanche(M); 
        
    }
    segments = points - nb_contours;
    printf("\nL'image possede %d contours, avec %d points et %d segments.\n\n", nb_contours, points, segments);

    // Free the mask image
    free_image(&M);

    return T;
}

Point avancer(Point P, Orientation O) {

    switch (O) {
        case Nord : return set_point(P.x, P.y-1);
            break;
        case Est : return set_point(P.x+1, P.y);
            break;
        case Sud : return set_point(P.x, P.y+1);
            break;
        case Ouest : return set_point(P.x-1, P.y);
            break;
    }
}

Contour memoriser_position(Image I, Point depart, Contour C, Image M) {

    Point position = depart;
    Orientation orient = Est;
    bool boucle = true;
    //int i = 0;
    while (boucle) {
        //printf("\n(%f, %f) ", position.x, position.y);
        if (orient == Est) {
            set_pixel_image(M, position.x+1, position.y+1, BLANC);
        } 
        C = ajouter_element_liste_Point(C, position);
        position = avancer(position, orient);
        orient = nouvelle_orientation(I, position, orient);

        if (cmp_point(depart, position) && orient == Est) {
            boucle = false;
        }
    }
    //printf("\n(%f, %f) \n", position.x, position.y);
    C = ajouter_element_liste_Point(C, position);

    return C;
}

void sequence_contour_eps(Tableau_Contour C, char* nom_fichier, char* mode, int h, int l){

    int i = 0;
    char nv_fichier[256];
    nv_fichier[0] = '\0';
    while (nom_fichier[i]!='.') {
        nv_fichier[i] = nom_fichier[i];
        i++;
    }
    nv_fichier[i] = '\0';
    strcat(nv_fichier, "_");
    strcat(nv_fichier, mode);
    strcat(nv_fichier, ".eps\0");

    printf("\nCreation de : %s\n", nv_fichier);

    FILE* f = fopen(nv_fichier, "w");

    fprintf(f, "%s\n", "%!PS-Adobe-3.0 EPSF-3.0");
    fprintf(f, "%s %d %d %d %d\n", "%%BoundingBox:", 0, 0, l, h);

    Contour A;

    for (int c=0; c < C.taille ; c++) {
        A = creer_liste_Point_vide();
        A = C.tab[c];
        Cellule_Point *cel = A.first;

        fprintf(f, "%5.1f %5.1f %s\n", cel->data.x, h-cel->data.y, "moveto");
        cel = cel->suiv;
        while (cel->suiv != NULL) {
            fprintf(f, "%5.1f %5.1f %s\n", cel->data.x, h-cel->data.y, "lineto");
            cel = cel->suiv;
        }
    }

    fprintf(f, "%s %s\n", "0 setlinewidth", mode);
    fprintf(f, "%s\n", "showpage");
    fclose(f);

}

void contour_eps(Contour C, char* nom_fichier, char* mode, int h, int l)
{
    int i = 0;
    char nv_fichier[256];
    nv_fichier[0] = '\0';
    while (nom_fichier[i]!='.') {
        nv_fichier[i] = nom_fichier[i];
        i++;
    }
    nv_fichier[i] = '\0';
    strcat(nv_fichier, "_");
    strcat(nv_fichier, mode);
    strcat(nv_fichier, ".eps\0");

    printf("\nCreation de : %s", nv_fichier);

    FILE* f = fopen(nv_fichier, "w");
    
    fprintf(f, "%s\n", "%!PS-Adobe-3.0 EPSF-3.0");
    fprintf(f, "%s %d %d %d %d\n", "%%BoundingBox:", 0, 0, l, h);

    Cellule_Point *cel = C.first;

    fprintf(f, "%5.1f %5.1f %s\n", cel->data.x, h-cel->data.y, "moveto");
    cel = cel->suiv;
    while (cel->suiv != NULL) {
        fprintf(f, "%5.1f %5.1f %s\n", cel->data.x, h-cel->data.y, "lineto");
        cel = cel->suiv;
    }
    fprintf(f, "%s %s\n", "0 setlinewidth", mode);
    fprintf(f, "%s\n", "showpage");
    
    fclose(f);
}

/**
 * @brief Save an array of contours as SVG
 */
void sequence_contour_svg(Tableau_Contour C, char* nom_fichier, char* mode, int h, int l) {
    printf("\nCreating: %s\n", nom_fichier);

    FILE* f = fopen(nom_fichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Error: Cannot create SVG file %s\n", nom_fichier);
        return;
    }

    // SVG header
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(f, "<!-- Created by Image Smoother -->\n");
    fprintf(f, "<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" xmlns=\"http://www.w3.org/2000/svg\">\n", l, h, l, h);
    fprintf(f, "  <g transform=\"translate(0, %d) scale(1, -1)\">\n", h);

    // Draw contours
    Contour A;
    for (int c = 0; c < C.taille; c++) {
        A = C.tab[c];
        Cellule_Point *cel = A.first;

        if (cel == NULL) continue;

        // Start path
        fprintf(f, "    <path d=\"M %5.1f %5.1f", cel->data.x, cel->data.y);
        cel = cel->suiv;

        while (cel != NULL) {
            fprintf(f, " L %5.1f %5.1f", cel->data.x, cel->data.y);
            cel = cel->suiv;
        }

        // Close path
        if (strcmp(mode, "fill") == 0) {
            fprintf(f, "\" fill=\"black\" stroke=\"none\" />\n");
        } else {
            fprintf(f, "\" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />\n");
        }
    }

    // Close SVG
    fprintf(f, "  </g>\n");
    fprintf(f, "</svg>\n");
    fclose(f);
}

Orientation nouvelle_orientation(Image I, Point P, Orientation O) {

    Pixel pG;
    Pixel pD;
    switch (O) {
        case Nord : 
            pG = get_pixel_image(I, P.x, P.y);
            pD = get_pixel_image(I, P.x+1, P.y);
            break;
        case Est: 
            pG = get_pixel_image(I, P.x+1, P.y);
            pD = get_pixel_image(I, P.x+1, P.y+1);
            break;
        case Sud: 
            pG = get_pixel_image(I, P.x+1, P.y+1);
            pD = get_pixel_image(I, P.x, P.y+1);
            break;
        case Ouest : 
            pG = get_pixel_image(I, P.x, P.y+1);
            pD = get_pixel_image(I, P.x, P.y);
            break;
    } 
    
    if (pG == NOIR) {

        switch (O) {
            case Est: return Nord;
                break;
            case Ouest : return Sud;
                break;
            case Sud: return Est;
                break;
            case Nord : return Ouest;
                break;
        }
    }

    else if (pD == BLANC) {

        switch (O) {
            case Est: return Sud;
                break;
            case Ouest : return Nord;
                break;
            case Sud: return Ouest;
                break;
            case Nord : return Est;
                break;
        }
    }

    else {
        return O;
    }
}


Contour simplification_douglas_peucker_rec(Contour Cont, int j1, int j2, double d) {

    Contour L = creer_liste_Point_vide();
    Tableau_Point P = sequence_points_liste_vers_tableau(Cont);
    
    if (d==0.0) {
        for (int j=j1; j<=j2; j++) {
            L = ajouter_element_liste_Point(L, P.tab[j]);
        }
        return L;
    }
    
    Point A = P.tab[j1];
    Point B = P.tab[j2];

    double dmax=0.0;
    int k = j1;

    for (int j = j1+1; j<=j2; j++) {
        double dj = distance_point_segment(P.tab[j],  A, B);
        if (dmax < dj) {
            dmax = dj;
            k=j;
        }
    }

    if (dmax <= d) {
        L = ajouter_element_liste_Point(L, A);
        L = ajouter_element_liste_Point(L, B);
    }
    else {
        Contour L1 = simplification_douglas_peucker_rec(Cont, j1, k, d);

        Contour L2 = simplification_douglas_peucker_rec(Cont,k, j2, d);

        L = concatener_liste_Point(L1, L2);
    }
    free_tableau_point(&P);
    return L;

}

void simpl_doug(Tableau_Contour Cont, double d) {

    Contour C = creer_liste_Point_vide();
    for (int j = 0; j< Cont.taille; j++) {

        C = Cont.tab[j];
        //printf("\nFLAG simpl\n");
        
        Cont.tab[j] = simplification_douglas_peucker_rec(C, 0, C.taille-1, d);
    }
}
