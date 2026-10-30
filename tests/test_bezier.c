#include "geom2d.h"
#include "bezier.h"
#include <stdio.h>

int main(int argc, char **argv){

    // Test with 2 points, n=1
    Contour Y = creer_liste_Point_vide();

    Point a = set_point(4.0, 3.0);
    Point b = set_point(7.0, 5.0);

    Y = ajouter_element_liste_Point(Y, a);
    Y = ajouter_element_liste_Point(Y, b);

    Bezier2 B = approx_bezier2(Y, 0, 1);
    //printf("C0 : (%f, %f), C1 : (%f, %f), C2 : (%f, %f)\n", B.c0.x, B.c0.y, B.c1.x, B.c1.y, B.c2.x, B.c2.y);
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

    B = approx_bezier2(Y, 0, 8);
    printf("C0 : (%f, %f), C1 : (%f, %f), C2 : (%f, %f)\n", B.c0.x, B.c0.y, B.c1.x, B.c1.y, B.c2.x, B.c2.y);
    free_contour(Y);
    
    return 0;
}
