#include"bezier.h"
#include "geom2d.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Point calcul_bezier2(Bezier2 C, double t) {
    if (t<0 || t>1) {
        printf("\nErreur : t doit etre entre 0 et 1\n");
        exit(1);
    }
    double x = pow(1-t, 2)*C.c0.x + 2*t*(1-t)*C.c1.x + pow(t, 2)*C.c2.x;
    double y = pow(1-t, 2)*C.c0.y + 2*t*(1-t)*C.c1.y + pow(t, 2)*C.c2.y;

    return set_point(x, y);
}

Point calcul_bezier3(Bezier3 C, double t) {
    if (t<0 || t>1) {
        printf("\nErreur : t doit etre entre 0 et 1\n");
        exit(1);
    }
    double x = pow(1-t, 3)*C.c0.x + 3*t*pow((1-t), 2)*C.c1.x + 3*pow(t, 2)*(1-t)*C.c2.x + pow(t, 3)*C.c3.x;
    double y = pow(1-t, 3)*C.c0.y + 3*t*pow((1-t), 2)*C.c1.y + 3*pow(t, 2)*(1-t)*C.c2.y + pow(t, 3)*C.c3.y;
    return set_point(x, y);
}

Bezier3 passage_Bezier2v3(Bezier2 C){
    Bezier3 B;

    B.c0 = set_point(C.c0.x, C.c0.y);
    B.c1 = set_point((C.c0.x + 2*C.c1.x)/3, (C.c0.y + 2*C.c1.y)/3);
    B.c2 = set_point((C.c2.x + 2*C.c1.x)/3, (C.c2.y + 2*C.c1.y)/3);
    B.c3 = set_point(C.c2.x, C.c2.y);

    return B;
}

Bezier2 approx_bezier2(Contour C, int j1, int j2) {

    Bezier2 B;
    Tableau_Point T = sequence_points_liste_vers_tableau(C);
    /*
    for (int x=0; x<T.taille; x++) {
        printf("(%5.1f, %5.1f) ", T.tab[x].x, T.tab[x].y);
    }
    */ 
    
    B.c0 = T.tab[j1];
    B.c2 = T.tab[j2];
    float n = j2-j1;

    if (n==1) {
        B.c1 = set_point((T.tab[j1].x + T.tab[j2].x)*1/2, (T.tab[j1].y + T.tab[j2].y)*1/2);
        free_tableau_point(&T);
        return B;
    }

    else if (n>=2) {
        float alpha = (3*n)/(pow(n, 2)-1);
        float beta = (1-2*n)/(2*(n+1));
        //printf("\nalpha : %f, beta : %f\n", alpha, beta);
        B.c1 = set_point(beta*(T.tab[j1].x + T.tab[j2].x), beta*(T.tab[j1].y + T.tab[j2].y));
        for (int i = 1; i<n; i++) {
            B.c1.x += T.tab[i+j1].x * alpha;
            B.c1.y += T.tab[i+j1].y * alpha;
        }
        free_tableau_point(&T);
        return B;
    }

    else {
        free_tableau_point(&T);
        printf("\nErreur : j2-j1 <= 0\n");
        exit(1);
    }

}

double distance_point_bezier2(Point P, Bezier2 B, double ti) {
    Point Q = calcul_bezier2(B, ti);
    return distance(P, Q);
}

double distance_point_bezier3(Point P, Bezier3 B, double ti) {
    Point Q = calcul_bezier3(B, ti);
    return distance(P, Q);
}


seq_bezier2 creer_liste_Bezier2_vide() {
    seq_bezier2 S = {0, NULL, NULL};
    return S;
}

seq_bezier3 creer_liste_Bezier3_vide() {
    seq_bezier3 S = {0, NULL, NULL};
    return S;
}

Cellule_bezier2 *creer_element_liste_Bezier2(Bezier2 v)
{
	Cellule_bezier2 *el;
	el = (Cellule_bezier2 *)malloc(sizeof(Cellule_bezier2));
	if (el==NULL)
	{
		fprintf(stderr, "creer_element_liste_Bezier : allocation impossible\n");
		exit(-1);
	}
	el->data = v;
	el->suiv = NULL;
	return el;
}

Cellule_bezier3 *creer_element_liste_Bezier3(Bezier3 v)
{
	Cellule_bezier3 *el;
	el = (Cellule_bezier3 *)malloc(sizeof(Cellule_bezier3));
	if (el==NULL)
	{
		fprintf(stderr, "creer_element_liste_Bezier : allocation impossible\n");
		exit(-1);
	}
	el->data = v;
	el->suiv = NULL;
	return el;
}

/* ajouter l'element b en fin de la liste L, renvoie la liste L modifiee */
seq_bezier2 ajouter_element_liste_Bezier2(seq_bezier2 L, Bezier2 b)
{
	Cellule_bezier2 *el;
	
	el = creer_element_liste_Bezier2(b);
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

seq_bezier3 ajouter_element_liste_Bezier3(seq_bezier3 L, Bezier3 b)
{
	Cellule_bezier3 *el;
	
	el = creer_element_liste_Bezier3(b);
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


seq_bezier2 concatener_liste_Bezier2(seq_bezier2 L1, seq_bezier2 L2)
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

/* Free a sequence of Bezier2 curves */
void free_seq_bezier2(seq_bezier2 *L)
{
	if (L == NULL) return;
	Cellule_bezier2 *current = L->first;
	while (current != NULL) {
		Cellule_bezier2 *next = current->suiv;
		free(current);
		current = next;
	}
	L->first = NULL;
	L->last = NULL;
	L->taille = 0;
}

/* Free a tableau of Bezier2 curves */
void free_tableau_bezier2(Tableau_Bezier2 *T)
{
	if (T == NULL) return;
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}

/* Free a tableau of sequences of Bezier2 curves */
void free_tableau_seq_bezier2(Tableau_seq_bezier2 *T)
{
	if (T == NULL) return;
	for (unsigned int i = 0; i < T->taille; i++) {
		free_seq_bezier2(&T->tab[i]);
	}
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}


seq_bezier3 concatener_liste_Bezier3(seq_bezier3 L1, seq_bezier3 L2)
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

/* Free a sequence of Bezier3 curves */
void free_seq_bezier3(seq_bezier3 *L)
{
	if (L == NULL) return;
	Cellule_bezier3 *current = L->first;
	while (current != NULL) {
		Cellule_bezier3 *next = current->suiv;
		free(current);
		current = next;
	}
	L->first = NULL;
	L->last = NULL;
	L->taille = 0;
}

/* Free a tableau of Bezier3 curves */
void free_tableau_bezier3(Tableau_Bezier3 *T)
{
	if (T == NULL) return;
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}

/* Free a tableau of sequences of Bezier3 curves */
void free_tableau_seq_bezier3(Tableau_seq_bezier3 *T)
{
	if (T == NULL) return;
	for (unsigned int i = 0; i < T->taille; i++) {
		free_seq_bezier3(&T->tab[i]);
	}
	free(T->tab);
	T->tab = NULL;
	T->taille = 0;
}


void ecrire_seq_bezier2(seq_bezier2 L)
{
	//Tableau_Point TP = sequence_points_liste_vers_tableau(L);
	int k;
	int nP = L.taille;
    Cellule_bezier2 *cel = L.first;
	if (nP == 0) {
	    printf("seq bezier avec vide\n");
        return;
    }
	printf("seq bezier avec %d courbes :\n{", nP);
	for (k = 0; k < nP; k++)
	{	
		Bezier2 P = cel->data;
		printf("[(%5.1f,%5.1f) (%5.1f,%5.1f) (%5.1f,%5.1f)];; ", P.c0.x, P.c0.y, P.c1.x, P.c1.y, P.c2.x, P.c2.y);
        cel = cel->suiv;
	} 
	printf("}\n");
	
	//free(cel); /* supprimer la cellule utilisee */
}

void ecrire_seq_bezier3(seq_bezier3 L)
{
	//Tableau_Point TP = sequence_points_liste_vers_tableau(L);
	int k;
	int nP = L.taille;
    Cellule_bezier3 *cel = L.first;
	if (nP == 0) {
	    printf("seq bezier avec vide\n");
        return;
    }
	printf("seq bezier avec %d courbes :\n{", nP);
	for (k = 0; k < nP; k++)
	{	
		Bezier3 P = cel->data; /* recuperer le point d'indice k */
		printf("[(%5.1f,%5.1f) (%5.1f,%5.1f) (%5.1f,%5.1f) (%5.1f,%5.1f)];; ", P.c0.x, P.c0.y, P.c1.x, P.c1.y, P.c2.x, P.c2.y, P.c3.x, P.c3.y);
        cel = cel->suiv;
	} 
	printf("}\n");
	
	//free(cel); /* supprimer le tableau de point TP */
}


Tableau_Bezier2 sequence_bezier2_liste_vers_tableau(seq_bezier2 L)
{
	Tableau_Bezier2 T;
	
	/* taille de T = taille de L */
	T.taille = L.taille;
	
	/* allocation dynamique du tableau de Point */
	T.tab = malloc(sizeof(Bezier2) * T.taille);
	if (T.tab == NULL)
	{
		/* allocation impossible : arret du programme avec un message */
		fprintf(stderr, "sequence_points_liste_vers_tableau : ");
		fprintf(stderr, " allocation impossible\n");
		exit(-1);
	}
	
	/* remplir le tableau de points T en parcourant la liste L */
	int k = 0; /* indice de l'element dans T.tab */
	Cellule_bezier2 *el = L.first; /* pointeur sur l'element dans L */
	while (el) 
	{
		T.tab[k] = el->data;
		k++; /* incrementer k */
		el = el->suiv; /* passer a l'element suivant dans la liste chainee */
	}
	//free(el);
	return T;
}

Tableau_seq_bezier2 ajouter_elem_tab_seq_bezier2(Tableau_seq_bezier2 T, seq_bezier2 S) {
    Tableau_seq_bezier2 L;
    L.taille = T.taille + 1;
    
    L.tab = malloc(L.taille * sizeof(*L.tab));
    if (!L.tab) {
        fprintf(stderr, "Erreur Allocation\n");
        exit(1);
    }
    for (unsigned i = 0; i < T.taille; i++) {
        L.tab[i] = T.tab[i];
    }

    L.tab[T.taille] = S;
    
    //free(T.tab);
    return L;
}

Tableau_seq_bezier3 ajouter_elem_tab_seq_bezier3(Tableau_seq_bezier3 T, seq_bezier3 S) {
    Tableau_seq_bezier3 L;
    L.taille = T.taille + 1;
    
    L.tab = malloc(L.taille * sizeof(*L.tab));
    if (!L.tab) {
        fprintf(stderr, "Erreur Allocation\n");
        exit(1);
    }
    for (unsigned i = 0; i < T.taille; i++) {
        L.tab[i] = T.tab[i];
    }

    L.tab[T.taille] = S;
    
    return L;
}

void ecrire_tableau_seq_bezer2(Tableau_seq_bezier2 T) {
	seq_bezier2 S;
	if (T.taille == 0) {printf("\nTableau vide"); return;}
	for (int i=0; i<T.taille; i++) {
		S = T.tab[i];
		ecrire_seq_bezier2(S);
	}
    //free(S.first);
	printf("\n\n");
}


void ecrire_tableau_seq_bezer3(Tableau_seq_bezier3 T) {
	seq_bezier3 S;
	if (T.taille == 0) {printf("\nTableau vide"); return;}
	for (int i=0; i<T.taille; i++) {
		S = T.tab[i];
		ecrire_seq_bezier3(S);
	}
    //free(S.first);
	printf("\n\n");
}


Tableau_seq_bezier2 creer_tab_seq_bezier2_vide() {
    Tableau_seq_bezier2 T;
    T.taille =0;
    return T;
}


Tableau_seq_bezier3 creer_tab_seq_bezier3_vide() {
    Tableau_seq_bezier3 T;
    T.taille =0;
    return T;
}

seq_bezier2 simpl_douglas_peucker_bezier2_rec(Contour Cont, int j1, int j2, int d) {

    Tableau_Point T = sequence_points_liste_vers_tableau(Cont);
     
    double n = j2-j1;

    Bezier2 B = approx_bezier2(Cont, j1, j2);
    seq_bezier2 L = creer_liste_Bezier2_vide();

    int dmax=0;
    int k = j1;
    for (int j = j1+1; j<=j2; j++) {
        int i =j-j1;
        double ti = i/n;
        double dj = distance_point_bezier2(T.tab[j], B, ti);

        if (dmax < dj) {
            dmax = dj;
            k=j;
        }
    }

    if (dmax <= d) {
        L = ajouter_element_liste_Bezier2(L, B);
    }
    else {
        seq_bezier2 L1 = creer_liste_Bezier2_vide();
        seq_bezier2 L2 =  creer_liste_Bezier2_vide();
        L1 = simpl_douglas_peucker_bezier2_rec(Cont, j1, k, d);
        L2 = simpl_douglas_peucker_bezier2_rec(Cont, k, j2, d);
        L = concatener_liste_Bezier2(L1, L2);
    }
    free_tableau_point(&T);
    return L;

}

void sequence_bezier2_eps(Tableau_seq_bezier2 C, char* nom_fichier, char* mode, int h, int l) {

    int i = 0;
    char nv_fichier[256];
    nv_fichier[0] = '\0';
    while (nom_fichier[i]!='.'  && nom_fichier[i]!='\0') {
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

    seq_bezier2 S;

    for (int s=0; s<C.taille; s++) {
        S = creer_liste_Bezier2_vide();
        S=C.tab[s];
        Cellule_bezier2 *cel = S.first;

        Bezier3 b = passage_Bezier2v3(cel->data);
        fprintf(f, "%5.1f %5.1f moveto\n", b.c0.x, h-b.c0.y);
    
        cel = cel->suiv;

        while (cel != NULL && cel->suiv != NULL) {
            Bezier3 b = passage_Bezier2v3(cel->data);
            fprintf(f, "%5.1f %5.1f %5.1f %5.1f %5.1f %5.1f curveto\n", b.c1.x, h-b.c1.y, b.c2.x, h-b.c2.y, b.c3.x, h-b.c3.y);
            cel = cel->suiv; 
        }
        //free(cel);
    }
   
    fprintf(f, "%s %s\n", "0 setlinewidth", mode);
    fprintf(f, "%s\n", "showpage");
    fclose(f);
    //free(S.first);

}

void affiche_nb_bezier2 (Tableau_seq_bezier2 T) {
    unsigned int nB = 0;

    for (int i=0; i<T.taille; i++) {
        nB += T.tab[i].taille;
    }

    printf("\nL'image possede %d courbes Beziers\n", nB);
}


void affiche_nb_bezier3 (Tableau_seq_bezier3 T) {
    unsigned int nB = 0;

    for (int i=0; i<T.taille; i++) {
        nB += T.tab[i].taille;
    }

    printf("\nL'image possede %d courbes Beziers\n", nB);
}

float gamma_bezier3(int k, float n) {
    return 6*pow(k,4) - 8*n*pow(k, 3) + 6*k*k - 4*n*k + pow(n, 4) - n*n;
}

Bezier3 approx_bezier3(Contour C, int j1, int j2) {

    Bezier3 B;
    Tableau_Point T = sequence_points_liste_vers_tableau(C);
    /*
    for (int x=0; x<T.taille; x++) {
        printf("(%5.1f, %5.1f) ", T.tab[x].x, T.tab[x].y);
    }
    */ 
    
    B.c0 = T.tab[j1];
    B.c3 = T.tab[j2];
    float n = j2-j1;

    if (n<3) {
        Bezier2 I = approx_bezier2(C, j1, j2);
        B = passage_Bezier2v3(I);
        free_tableau_point(&T);
        return B;
    }

    else if (n>=3) {
        float alpha = (-15*pow(n, 3) + 5*pow(n, 2) + 2*n +4)/(3*(n+2)*(3*pow(n, 2)+1));
        float beta = (10*pow(n, 3) - 15*pow(n, 2) + n +2)/(3*(n+2)*(3*pow(n, 2)+1));
        float lambda = (70*n)/(3*(pow(n, 2) -1)  * (pow(n, 2) -4) * (3* pow(n,2) +1));
        //printf("\nalpha : %f, beta : %f\n", alpha, beta);
        B.c1 = set_point(0, 0);
        B.c2 = set_point(0, 0);
        for (int i = 1; i<n; i++) {
            B.c1.x += lambda * gamma_bezier3(i, n) * T.tab[j1+i].x; 
            B.c1.y += lambda * gamma_bezier3(i, n) * T.tab[j1+i].y;

            B.c2.x += lambda * gamma_bezier3(n-i, n) * T.tab[j1+i].x; 
            B.c2.y += lambda * gamma_bezier3(n-i, n) * T.tab[j1+i].y;            
        }

        B.c1.x += alpha*T.tab[j1].x + beta*T.tab[j2].x;
        B.c1.y += alpha*T.tab[j1].y + beta*T.tab[j2].y;

        B.c2.x += alpha*T.tab[j2].x + beta*T.tab[j1].x;
        B.c2.y += alpha*T.tab[j2].y + beta*T.tab[j1].y;
        free_tableau_point(&T);
        return B;
    }

    else {
        free_tableau_point(&T);
        printf("\nErreur : j2-j1 <= 0\n");
        exit(1);
    }

}

seq_bezier3 simpl_douglas_peucker_bezier3_rec(Contour Cont, int j1, int j2, int d) {

    Tableau_Point T = sequence_points_liste_vers_tableau(Cont);
     
    double n = j2-j1;

    Bezier3 B = approx_bezier3(Cont, j1, j2);
    seq_bezier3 L = creer_liste_Bezier3_vide();

    int dmax=0;
    int k = j1;
    for (int j = j1+1; j<=j2; j++) {
        int i =j-j1;
        double ti = i/n;
        double dj = distance_point_bezier3(T.tab[j], B, ti);

        if (dmax < dj) {
            dmax = dj;
            k=j;
        }
    }

    if (dmax <= d) {
        L = ajouter_element_liste_Bezier3(L, B);
    }
    else {
        seq_bezier3 L1 = creer_liste_Bezier3_vide();
        seq_bezier3 L2 =  creer_liste_Bezier3_vide();
        L1 = simpl_douglas_peucker_bezier3_rec(Cont, j1, k, d);
        L2 = simpl_douglas_peucker_bezier3_rec(Cont, k, j2, d);
        L = concatener_liste_Bezier3(L1, L2);
    }

    free_tableau_point(&T);
    return L;

}



void sequence_bezier3_eps(Tableau_seq_bezier3 C, char* nom_fichier, char* mode, int h, int l) {

    int i = 0;
    char nv_fichier[256];
    nv_fichier[0] = '\0';
    while (nom_fichier[i]!='.'  && nom_fichier[i]!='\0') {
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

    seq_bezier3 S;

    for (int s=0; s<C.taille; s++) {
        S = creer_liste_Bezier3_vide();
        S=C.tab[s];
        Cellule_bezier3 *cel = S.first;

        Bezier3 b = cel->data;
        fprintf(f, "%5.1f %5.1f moveto\n", b.c0.x, h-b.c0.y);
    
        cel = cel->suiv;

        while (cel != NULL && cel->suiv != NULL) {
            Bezier3 b = cel->data;
            fprintf(f, "%5.1f %5.1f %5.1f %5.1f %5.1f %5.1f curveto\n", b.c1.x, h-b.c1.y, b.c2.x, h-b.c2.y, b.c3.x, h-b.c3.y);
            cel = cel->suiv; 
        }
    }
   
    fprintf(f, "%s %s\n", "0 setlinewidth", mode);
    fprintf(f, "%s\n", "showpage");
    fclose(f);

}

/**
 * @brief Save an array of sequences of Bezier2 curves as SVG
 */
void sequence_bezier2_svg(Tableau_seq_bezier2 C, char* nom_fichier, char* mode, int h, int l) {
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

    // Draw Bezier curves
    seq_bezier2 S;
    for (int s = 0; s < C.taille; s++) {
        S = C.tab[s];
        Cellule_bezier2 *cel = S.first;

        if (cel == NULL) continue;

        // Convert Bezier2 to Bezier3 for SVG (which uses cubic Bezier)
        Bezier3 b = passage_Bezier2v3(cel->data);
        fprintf(f, "    <path d=\"M %5.1f %5.1f C %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f\"",
                b.c0.x, b.c0.y, b.c1.x, b.c1.y, b.c2.x, b.c2.y, b.c3.x, b.c3.y);
        cel = cel->suiv;

        while (cel != NULL) {
            b = passage_Bezier2v3(cel->data);
            fprintf(f, " C %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f",
                    b.c1.x, b.c1.y, b.c2.x, b.c2.y, b.c3.x, b.c3.y);
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

/**
 * @brief Save an array of sequences of Bezier3 curves as SVG
 */
void sequence_bezier3_svg(Tableau_seq_bezier3 C, char* nom_fichier, char* mode, int h, int l) {
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

    // Draw Bezier curves
    seq_bezier3 S;
    for (int s = 0; s < C.taille; s++) {
        S = C.tab[s];
        Cellule_bezier3 *cel = S.first;

        if (cel == NULL) continue;

        // Start path with first curve
        Bezier3 b = cel->data;
        fprintf(f, "    <path d=\"M %5.1f %5.1f C %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f\"",
                b.c0.x, b.c0.y, b.c1.x, b.c1.y, b.c2.x, b.c2.y, b.c3.x, b.c3.y);
        cel = cel->suiv;

        while (cel != NULL) {
            b = cel->data;
            fprintf(f, " C %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f",
                    b.c1.x, b.c1.y, b.c2.x, b.c2.y, b.c3.x, b.c3.y);
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
