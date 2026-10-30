#ifndef _BEZIER_H_
#define _BEZIER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include"geom2d.h"
#include"calcul_contour.h"

/**
 * @brief Output format enum
 */
typedef enum { FORMAT_EPS, FORMAT_SVG } OutputFormat;

/**
 * @brief Quadratic Bezier curve with 3 control points
 */
typedef struct Bezier2_ {
    Point c0, c1, c2;  /**< Control points */
} Bezier2;

/**
 * @brief Cubic Bezier curve with 4 control points
 */
typedef struct Bezier3_ {
    Point c0, c1, c2, c3;  /**< Control points */
} Bezier3;

/**
 * @brief Linked list cell for Bezier2 curves
 */
typedef struct Cellule_bezier2_
{
	Bezier2 data;                    /**< Bezier2 curve data */
	struct Cellule_bezier2_* suiv;   /**< Pointer to next cell */
} Cellule_bezier2;

/**
 * @brief Sequence (linked list) of Bezier2 curves
 */
typedef struct seq_bezier2_ {
    unsigned int taille;            /**< Number of curves in sequence */
    Cellule_bezier2 *first;         /**< Pointer to first curve */
	Cellule_bezier2 *last;          /**< Pointer to last curve */
} seq_bezier2;

/**
 * @brief Array of Bezier2 curves
 */
typedef struct Tableau_Bezier2_
{
	unsigned int taille;            /**< Number of elements in array */
	Bezier2 *tab;                   /**< Pointer to array of curves */
} Tableau_Bezier2;

/**
 * @brief Array of sequences of Bezier2 curves
 */
typedef struct Tableau_seq_bezier2_ {
	unsigned int taille;            /**< Number of sequences */
	seq_bezier2 *tab;               /**< Pointer to array of sequences */
} Tableau_seq_bezier2;

/**
 * @brief Linked list cell for Bezier3 curves
 */
typedef struct Cellule_bezier3_
{
	Bezier3 data;                    /**< Bezier3 curve data */
	struct Cellule_bezier3_* suiv;   /**< Pointer to next cell */
} Cellule_bezier3;

/**
 * @brief Sequence (linked list) of Bezier3 curves
 */
typedef struct seq_bezier3_ {
    unsigned int taille;            /**< Number of curves in sequence */
    Cellule_bezier3 *first;         /**< Pointer to first curve */
	Cellule_bezier3 *last;          /**< Pointer to last curve */
} seq_bezier3;

/**
 * @brief Array of Bezier3 curves
 */
typedef struct Tableau_Bezier3_
{
	unsigned int taille;            /**< Number of elements in array */
	Bezier3 *tab;                   /**< Pointer to array of curves */
} Tableau_Bezier3;

/**
 * @brief Array of sequences of Bezier3 curves
 */
typedef struct Tableau_seq_bezier3_ {
	unsigned int taille;            /**< Number of sequences */
	seq_bezier3 *tab;               /**< Pointer to array of sequences */
} Tableau_seq_bezier3;

/**
 * @brief Calculate a point on a quadratic Bezier curve
 * @param C Bezier2 curve
 * @param t Parameter (0 <= t <= 1)
 * @return Point on the curve at parameter t
 */
Point calcul_bezier2(Bezier2 C, double t);

/**
 * @brief Calculate a point on a cubic Bezier curve
 * @param C Bezier3 curve
 * @param t Parameter (0 <= t <= 1)
 * @return Point on the curve at parameter t
 */
Point calcul_bezier3(Bezier3 C, double t);

/**
 * @brief Convert a quadratic Bezier curve to cubic
 * @param C Quadratic Bezier curve
 * @return Equivalent cubic Bezier curve
 */
Bezier3 passage_Bezier2v3(Bezier2 C);

/**
 * @brief Approximate a contour segment with a quadratic Bezier curve
 * @param C Contour
 * @param j1 Start index
 * @param j2 End index
 * @return Approximating Bezier2 curve
 */
Bezier2 approx_bezier2(Contour C, int j1, int j2);

/**
 * @brief Create an empty sequence of Bezier2 curves
 * @return Empty sequence
 */
seq_bezier2 creer_liste_Bezier2_vide();

/**
 * @brief Add a Bezier2 curve to the end of a sequence
 * @param L Sequence to modify
 * @param b Bezier2 curve to add
 * @return Modified sequence
 */
seq_bezier2 ajouter_element_liste_Bezier2(seq_bezier2 L, Bezier2 b);

/**
 * @brief Create a linked list cell for a Bezier2 curve
 * @param v Bezier2 curve
 * @return Pointer to the new cell
 */
Cellule_bezier2 *creer_element_liste_Bezier2(Bezier2 v);

/**
 * @brief Concatenate two sequences of Bezier2 curves
 * @param L1 First sequence
 * @param L2 Second sequence
 * @return Concatenated sequence (L1 + L2)
 */
seq_bezier2 concatener_liste_Bezier2(seq_bezier2 L1, seq_bezier2 L2);

/**
 * @brief Convert a sequence of Bezier2 curves to an array
 * @param L Sequence to convert
 * @return Array of Bezier2 curves
 */
Tableau_Bezier2 sequence_bezier2_liste_vers_tableau(seq_bezier2 L);

/**
 * @brief Add a sequence of Bezier2 curves to an array
 * @param T Array to modify
 * @param S Sequence to add
 * @return Modified array
 */
Tableau_seq_bezier2 ajouter_elem_tab_seq_bezier2(Tableau_seq_bezier2 T, seq_bezier2 S);

/**
 * @brief Create an empty array of sequences of Bezier2 curves
 * @return Empty array
 */
Tableau_seq_bezier2 creer_tab_seq_bezier2_vide();

/**
 * @brief Display an array of sequences of Bezier2 curves
 * @param T Array to display
 */
void ecrire_tableau_seq_bezer2(Tableau_seq_bezier2 T);

/**
 * @brief Display a sequence of Bezier2 curves
 * @param L Sequence to display
 */
void ecrire_seq_bezier2(seq_bezier2 L);

/**
 * @brief Calculate distance from a point to a Bezier2 curve at parameter t
 * @param P Point
 * @param B Bezier2 curve
 * @param ti Parameter value
 * @return Distance from P to curve at ti
 */
double distance_point_bezier2(Point P, Bezier2 B, double ti);

/**
 * @brief Apply Douglas-Peucker simplification with Bezier2 curves
 * @param Cont Contour to simplify
 * @param j1 Start index
 * @param j2 End index
 * @param d Maximum allowed distance
 * @return Sequence of approximating Bezier2 curves
 */
seq_bezier2 simpl_douglas_peucker_bezier2_rec(Contour Cont, int j1, int j2, int d);

/**
 * @brief Save an array of sequences of Bezier2 curves as EPS
 * @param C Array of sequences
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_bezier2_eps(Tableau_seq_bezier2 C, char* nom_fichier, char* mode, int h, int l);

/**
 * @brief Display the number of Bezier2 curves
 * @param T Array of sequences
 */
void affiche_nb_bezier2(Tableau_seq_bezier2 T);

/**
 * @brief Calculate gamma coefficient for Bezier3 approximation
 * @param k Index
 * @param n Number of points
 * @return Gamma coefficient
 */
float gamma_bezier3(int k, float n);

/**
 * @brief Approximate a contour segment with a cubic Bezier curve
 * @param C Contour
 * @param j1 Start index
 * @param j2 End index
 * @return Approximating Bezier3 curve
 */
Bezier3 approx_bezier3(Contour C, int j1, int j2);

/**
 * @brief Create an empty sequence of Bezier3 curves
 * @return Empty sequence
 */
seq_bezier3 creer_liste_Bezier3_vide();

/**
 * @brief Add a Bezier3 curve to the end of a sequence
 * @param L Sequence to modify
 * @param b Bezier3 curve to add
 * @return Modified sequence
 */
seq_bezier3 ajouter_element_liste_Bezier3(seq_bezier3 L, Bezier3 b);

/**
 * @brief Create a linked list cell for a Bezier3 curve
 * @param v Bezier3 curve
 * @return Pointer to the new cell
 */
Cellule_bezier3 *creer_element_liste_Bezier3(Bezier3 v);

/**
 * @brief Concatenate two sequences of Bezier3 curves
 * @param L1 First sequence
 * @param L2 Second sequence
 * @return Concatenated sequence (L1 + L2)
 */
seq_bezier3 concatener_liste_Bezier3(seq_bezier3 L1, seq_bezier3 L2);

/**
 * @brief Convert a sequence of Bezier3 curves to an array
 * @param L Sequence to convert
 * @return Array of Bezier3 curves
 */
Tableau_Bezier3 sequence_bezier3_liste_vers_tableau(seq_bezier3 L);

/**
 * @brief Add a sequence of Bezier3 curves to an array
 * @param T Array to modify
 * @param S Sequence to add
 * @return Modified array
 */
Tableau_seq_bezier3 ajouter_elem_tab_seq_bezier3(Tableau_seq_bezier3 T, seq_bezier3 S);

/**
 * @brief Create an empty array of sequences of Bezier3 curves
 * @return Empty array
 */
Tableau_seq_bezier3 creer_tab_seq_bezier3_vide();

/**
 * @brief Display an array of sequences of Bezier3 curves
 * @param T Array to display
 */
void ecrire_tableau_seq_bezer3(Tableau_seq_bezier3 T);

/**
 * @brief Display a sequence of Bezier3 curves
 * @param L Sequence to display
 */
void ecrire_seq_bezier3(seq_bezier3 L);

/**
 * @brief Display the number of Bezier3 curves
 * @param T Array of sequences
 */
void affiche_nb_bezier3(Tableau_seq_bezier3 T);

/**
 * @brief Apply Douglas-Peucker simplification with Bezier3 curves
 * @param Cont Contour to simplify
 * @param j1 Start index
 * @param j2 End index
 * @param d Maximum allowed distance
 * @return Sequence of approximating Bezier3 curves
 */
seq_bezier3 simpl_douglas_peucker_bezier3_rec(Contour Cont, int j1, int j2, int d);

/**
 * @brief Save an array of sequences of Bezier3 curves as EPS
 * @param C Array of sequences
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_bezier3_eps(Tableau_seq_bezier3 C, char* nom_fichier, char* mode, int h, int l);

/**
 * @brief Save an array of sequences of Bezier2 curves as SVG
 * @param C Array of sequences
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_bezier2_svg(Tableau_seq_bezier2 C, char* nom_fichier, char* mode, int h, int l);

/**
 * @brief Save an array of sequences of Bezier3 curves as SVG
 * @param C Array of sequences
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_bezier3_svg(Tableau_seq_bezier3 C, char* nom_fichier, char* mode, int h, int l);

/**
 * @brief Free a sequence of Bezier2 curves
 * @param L Sequence to free
 */
void free_seq_bezier2(seq_bezier2 *L);

/**
 * @brief Free a tableau of Bezier2 curves
 * @param T Tableau to free
 */
void free_tableau_bezier2(Tableau_Bezier2 *T);

/**
 * @brief Free a tableau of sequences of Bezier2 curves
 * @param T Tableau to free
 */
void free_tableau_seq_bezier2(Tableau_seq_bezier2 *T);

/**
 * @brief Free a sequence of Bezier3 curves
 * @param L Sequence to free
 */
void free_seq_bezier3(seq_bezier3 *L);

/**
 * @brief Free a tableau of Bezier3 curves
 * @param T Tableau to free
 */
void free_tableau_bezier3(Tableau_Bezier3 *T);

/**
 * @brief Free a tableau of sequences of Bezier3 curves
 * @param T Tableau to free
 */
void free_tableau_seq_bezier3(Tableau_seq_bezier3 *T);

#endif
