#include"geom2d.h"
#include <stdio.h>

Point set_point(double x, double y){
    Point P={x,y};
    return P;
}

bool cmp_point(Point A, Point B){
    return (A.x == B.x && A.y == B.y);
}

Point add_point(Point P1, Point P2){
    return set_point(P1.x + P2.x, P1.y + P2.y);
}

Vecteur vect_bipoint(Point A, Point B){
    Vecteur V={B.x-A.x, B.y-A.y};
    return V;
}

Vecteur add_vect(Vecteur A, Vecteur B){
    Vecteur V={A.x+B.x, A.y+B.y};
    return V;
}

Vecteur produit_vecteur(Vecteur A, double x){
    Vecteur V={x*A.x, x*A.y};
    return V;
}

Point produit_point(Point P,double a){
    return set_point(a*P.x, a*P.y);

}

double produit_scal(Vecteur A,Vecteur B){
    return A.x*B.x + A.y*B.y;
}

double norme_eucl(Vecteur V){
    return sqrt(V.x*V.x + V.y*V.y);
}

double distance(Point A, Point B) {
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));   return sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
    
}

double distance_point_segment(Point P, Point A, Point B){
	Vecteur AB = vect_bipoint(A, B);
	Vecteur AP = vect_bipoint(A, P);

	double normeAB = norme_eucl(AB);
	
	if (cmp_point(A, B) || fabs(normeAB) < 1e-10) {
		return distance(P, A);
	}

	double t = produit_scal(AP, AB) / (normeAB*normeAB);
	
	Point Q;	
	
	if (t < 0.0) {
		Q = set_point(A.x, A.y);
	}
	else if (t > 1.0) {
		Q = set_point(B.x, B.y);
	}
	else {
		Q = set_point(A.x + t*AB.x, A.y + t*AB.y);
	}
	
	return distance(Q, P);
}

/* creer une cellule de liste avec l'element v 
   renvoie le pointeur sur la cellule de liste creee
   la fonction s'arrete si la creation n'a pas pu se faire */
Cellule_Point *creer_element_liste_Point(Point v)
{
	Cellule_Point *el;
	el = (Cellule_Point *)malloc(sizeof(Cellule_Point));
	if (el==NULL)
	{
		fprintf(stderr, "creer_element_liste_Point : allocation impossible\n");
		exit(-1);
	}
	el->data = v;
	el->suiv = NULL;
	return el;
}

Contour creer_liste_Point_vide()
{
	Contour L = {0, NULL, NULL};
	return L;
}

/* ajouter l'element e en fin de la liste L, renvoie la liste L modifiee */
Contour ajouter_element_liste_Point(Contour L, Point e)
{
	Cellule_Point *el;
	
	el = creer_element_liste_Point(e);
	if (L.taille == 0)
	{
		/* premier element de la liste */
		L.first = L.last = el;
	}
	else
	{
		L.last->suiv = el;
		L.last = el;
	}
	L.taille++;

	return L;
}

/* Free all elements of a contour/list of points */
Contour free_contour(Contour L)
{
	Cellule_Point *el = L.first;
	
	while (el) 
	{
		Cellule_Point *suiv = el->suiv;
		free(el);
		el = suiv;
	}
	L.first = L.last = NULL;
	L.taille = 0;
	return L;
}

/* Free a tableau of points */
void free_tableau_point(Tableau_Point *T)
{
	if (T == NULL) return;
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}

/* Free a tableau of contours */
void free_tableau_contour(Tableau_Contour *T)
{
	if (T == NULL) return;
	for (unsigned int i = 0; i < T->taille; i++) {
		free_contour(T->tab[i]);
	}
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}

/* concatener L2 a la suite de L1, renvoie la liste L1 modifiee */
Contour concatener_liste_Point(Contour L1, Contour L2)
{
	/* cas o� l'une des deux listes est vide */
	if (L1.taille == 0) return L2; 
	if (L2.taille == 0) return L1;
	
	/* les deux listes sont non vides */
	L1.last->suiv = L2.first; /* lien entre L1.last et L2.first */
	L1.last = L2.last;        /* le dernier element de L1 est celui de L2 */
	L1.taille += L2.taille;   /* nouvelle taille pour L1 */
	return L1;
}

/* si la liste est non vide, la fonction supprime le premier element de L 
   si la liste est vide, la fonction ne fait rien 
   la fonction renvoie la liste (eventuellement) modifiee */
Contour supprimer_premier_element_liste_Point(Contour L)
{
	/* cas d'une liste L vide : ne rien faire */
	if (L.taille == 0) return L;
	
	Cellule_Point *e1=L.first;  /* pointeur vers le premier element  */ 
	Cellule_Point *e2=e1->suiv; /* pointeur vers le deuxieme element */ 
	
	/* supprimer l'element pointe par e1 */
	free(e1);
	
	/* mettre a jour la structure L */
	L.first = e2;
	if (e2 == NULL)    /* cas d'une liste L avec un seul element */
		L.last = NULL; /* --> la liste L devient une liste vide  */
	L.taille--;
	return L;
}

Contour supprimer_point_liste_Point(Contour L, Point p) {

	if (L.taille == 0) return L;

	Cellule_Point *cel = L.first;
	Cellule_Point *prec = NULL;
	while (cmp_point(cel->data, p) == false){
		prec = cel;
		cel = cel->suiv;
	}

	if (cel == NULL) {return L;}

	Cellule_Point *e2=cel->suiv; /* pointeur vers l element suivantt*/ 
	
	/* supprimer l'element pointe par e1 */
	free(cel);
	
	/* mettre a jour la structure L */
	if (cel == L.first) {
		L.first = e2;
	}
	else if (cel == L.last) {   /* cas d'une liste L avec un seul element */
		L.last = NULL;             /* --> la liste L devient une liste vide  */
	}
	else {  //cas au milieur de L
		prec->suiv = e2;
	}
	L.taille--;
	return L;

}


void ecrire_seq_contour(Contour L)
{
	//Tableau_Point TP = sequence_points_liste_vers_tableau(L);
	int k;
	int nP = L.taille;
    Cellule_Point *cel = L.first;
	if (nP == 0) {
	    printf("Contour avec %d points et %d segments\n[]\n", nP, nP);
        return;
    }
	printf("Contour avec %d points et %d segments\n[", nP, nP-1);
	for (k = 0; k < nP; k++)
	{	
		Point P = cel->data; /* recuperer le point d'indice k */
		printf("(%5.1f,%5.1f)", P.x, P.y);
        cel = cel->suiv;
	} 
	printf("]\n");
	
	//free(TP.tab); /* supprimer le tableau de point TP */
}

unsigned int compte_segments_contours(Contour C) {
	if (C.taille < 2) {
		return 0;
	}
	return C.taille-1;
}

void affiche_nb_segments_tab_contours(Tableau_Contour T){
	unsigned int nb_segments = 0;
	for (unsigned int i=0; i<T.taille; i++) {
		nb_segments += compte_segments_contours(T.tab[i]);
	}
	printf("\nLe contour possede %d segments\n", nb_segments);
}

Tableau_Contour creer_tableau_contour_vide() {
	Tableau_Contour T;
	T.taille=0;
	//T.tab = malloc(sizeof(Tableau_Contour));
	return T;

}

Tableau_Contour ajouter_element_tab_contour(Tableau_Contour T, Contour C) {
	Tableau_Contour L;
    L.taille = T.taille + 1;
    
    L.tab = malloc(L.taille * sizeof(*L.tab));
    if (!L.tab) {
        fprintf(stderr, "Erreur Allocation\n");
        exit(1);
    }
    for (unsigned i = 0; i < T.taille; i++) {
        L.tab[i] = T.tab[i];
    }

    L.tab[T.taille] = C;
    
	//free(T.tab);
    return L;
}

void ecrire_tableau_contours(Tableau_Contour T) {
	Contour C;
	if (T.taille == 0) {printf("\nTableau vide"); return;}
	for (int i=0; i<T.taille; i++) {
		C = T.tab[i];
		ecrire_seq_contour(C);
	}
	printf("\n\n");
}

Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L)
{
	Tableau_Point T;
	
	/* taille de T = taille de L */
	T.taille = L.taille;
	
	/* allocation dynamique du tableau de Point */
	T.tab = malloc(sizeof(Point) * T.taille);
	if (T.tab == NULL)
	{
		/* allocation impossible : arret du programme avec un message */
		fprintf(stderr, "sequence_points_liste_vers_tableau : ");
		fprintf(stderr, " allocation impossible\n");
		exit(-1);
	}
	
	/* remplir le tableau de points T en parcourant la liste L */
	int k = 0; /* indice de l'element dans T.tab */
	Cellule_Point *el = L.first; /* pointeur sur l'element dans L */
	while (el) 
	{
		T.tab[k] = el->data;
		k++; /* incrementer k */
		el = el->suiv; /* passer a l'element suivant dans la liste chainee */
	}
	
	return T;
}

Tableau_Point tab_cont_vers_tab_point(Tableau_Contour C){
	Tableau_Point T;
	
	/* taille de T = taille de L */
	T.taille = 0;
	for (int i = 0; i<C.taille; i++) {
		T.taille += C.tab[i].taille;
	}
	
	/* allocation dynamique du tableau de Point */
	T.tab = malloc(sizeof(Point) * T.taille);
	if (T.tab == NULL)
	{
		/* allocation impossible : arret du programme avec un message */
		fprintf(stderr, "sequence_points_liste_vers_tableau : ");
		fprintf(stderr, " allocation impossible\n");
		exit(-1);
	}
	
	/* remplir le tableau de points T en parcourant la liste L */
	int k = 0; /* indice de l'element dans T.tab */
	Cellule_Point *el;
	for (int i = 0; i<C.taille; i++) { 
		el = C.tab[i].first; 
		while (el) 
		{
			T.tab[k] = el->data;
			k++; /* incrementer k */
			el = el->suiv; /* passer a l'element suivant dans la liste chainee */
		}
	}
	
	return T;
}

Tableau_Point creer_tab_vide() {
	Tableau_Point T;
	T.taille=0;
	return T;
}

unsigned int nb_point_tab_cont(Tableau_Contour T) {
	int p = 0;
	for (int i = 0; i<T.taille; i++) {
		p += T.tab[i].taille;
	}
	return p;
}