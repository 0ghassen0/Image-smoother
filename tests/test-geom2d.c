#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"geom2d.h"

/*
int main(int argc, char **argv){

    Point p = set_point(5.12, 3.42);
    Point d = set_point(7.422, 6.469);

    Point c = add_point(p, d);
    printf("p + d = (%f, %f)\n", c.x, c.y);

    Vecteur u = vect_bipoint(p,c);

    printf("u = (%f, %f)\n", u.x, u.y);

    Point x = produit_point(c, 2);
    printf("x = (%f, %f)\n", x.x, x.y);

    Vecteur v = produit_vecteur(u, 2);

    printf("v = (%f, %f)\n", v.x, v.y);

    printf("produit sclaire de u et v : %f\n", produit_scal(u, v));

    printf("La norme de v : %f\n", norme_eucl(v));

    printf("distance entre c et p : %f\n", distance(c, p));

    return 0;

} */

int main(int argc, char **argv){
    // Test fonction distance_point_segment
    Point A, B, P;
    printf("Enter P (x y): ");
    scanf("%lf %lf", &P.x, &P.y);
    printf("Enter A (x y): ");
    scanf("%lf %lf", &A.x, &A.y);
    printf("Enter B (x y): ");
    scanf("%lf %lf", &B.x, &B.y);
    double dist = distance_point_segment(P, A, B);
    printf("The distance from point P to segment AB is: %f\n", dist);
    return 0;
}
