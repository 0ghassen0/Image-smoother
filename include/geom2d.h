#ifndef _GEOM_2D_H_
#define _GEOM_2D_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

/**
 * @brief Represents a point in 2D space
 */
typedef struct Point_ {
    double x, y;  /**< x and y coordinates */
} Point;

/**
 * @brief Represents a 2D vector
 */
typedef struct Vecteur_ {
    double x, y;  /**< x and y components */
} Vecteur;

/**
 * @brief Linked list cell for storing points
 */
typedef struct Cellule_Point_
{
	Point data;            /**< Point data */
	struct Cellule_Point_* suiv;  /**< Pointer to next cell */
} Cellule_Point;

/**
 * @brief Linked list of points (used for contours)
 */
typedef struct Liste_Point_
{
	unsigned int taille;        /**< Number of elements in the list */
	Cellule_Point *first;     /**< Pointer to first element */
	Cellule_Point *last;      /**< Pointer to last element */
	                            /**< first = last = NULL and taille = 0 <=> empty list */
} Liste_Point;

typedef Liste_Point Contour; /**< Contour is a list of points */

/**
 * @brief Array of points
 */
typedef struct Tableau_Point_
{
	unsigned int taille;        /**< Number of elements in the array */
	Point *tab;                 /**< Pointer to the array of points */
} Tableau_Point;

/**
 * @brief Array of contours
 */
typedef struct Tableau_Contour_
{
	unsigned int taille;        /**< Number of elements in the array */
	Contour *tab;               /**< Pointer to the array of contours */
} Tableau_Contour;

/**
 * @brief Create an empty list of points
 * @return Empty list
 */
Contour creer_liste_Point_vide();

/**
 * @brief Create an empty array of contours
 * @return Empty contour array
 */
Tableau_Contour creer_tableau_contour_vide();

/**
 * @brief Count total points in a contour array
 * @param T Array of contours
 * @return Total number of points
 */
unsigned int nb_point_tab_cont(Tableau_Contour T);

/**
 * @brief Display an array of contours
 * @param T Array of contours to display
 */
void ecrire_tableau_contours(Tableau_Contour T);

/**
 * @brief Add a contour to an array of contours
 * @param T Original array
 * @param C Contour to add
 * @return New array with contour added
 */
Tableau_Contour ajouter_element_tab_contour(Tableau_Contour T, Contour C);

/**
 * @brief Create a linked list cell with point v
 * @param v Point to store
 * @return Pointer to the new cell
 */
Cellule_Point *creer_element_liste_Point(Point v);

/**
 * @brief Display a contour (linked list)
 * @param L Contour to display
 */
void ecrire_seq_contour(Contour L);

/**
 * @brief Remove the first element from a list of points
 * @param L List to modify
 * @return Modified list
 */
Contour supprimer_premier_element_liste_Point(Contour L);

/**
 * @brief Remove a specific point from a list of points
 * @param L List to modify
 * @param p Point to remove
 * @return Modified list
 */
Contour supprimer_point_liste_Point(Contour L, Point p);

/**
 * @brief Concatenate two lists of points
 * @param L1 First list
 * @param L2 Second list
 * @return Concatenated list (L1 + L2)
 */
Contour concatener_liste_Point(Contour L1, Contour L2);

/**
 * @brief Free all elements of a contour/list of points
 * @param L List to free
 * @return Empty list
 */
Contour free_contour(Contour L);

/**
 * @brief Add a point to the end of a list
 * @param L List to modify
 * @param e Point to add
 * @return Modified list
 */
Contour ajouter_element_liste_Point(Contour L, Point e);

/**
 * @brief Convert a list of points to an array of points
 * @param L List to convert
 * @return Array of points
 */
Tableau_Point sequence_points_liste_vers_tableau(Liste_Point L);

/**
 * @brief Convert an array of contours to an array of points
 * @param C Array of contours
 * @return Array of all points
 */
Tableau_Point tab_cont_vers_tab_point(Tableau_Contour C);

/**
 * @brief Create an empty point array
 * @return Empty point array
 */
Tableau_Point creer_tab_vide();

/**
 * @brief Free a tableau of points
 * @param T Tableau to free
 */
void free_tableau_point(Tableau_Point *T);

/**
 * @brief Free a tableau of contours
 * @param T Tableau to free
 */
void free_tableau_contour(Tableau_Contour *T);

/**
 * @brief Set a point with coordinates x, y
 * @param x X coordinate
 * @param y Y coordinate
 * @return New point
 */
Point set_point(double x, double y);

/**
 * @brief Compare two points for equality
 * @param A First point
 * @param B Second point
 * @return true if points are equal, false otherwise
 */
bool cmp_point(Point A, Point B);

/**
 * @brief Add two points
 * @param P1 First point
 * @param P2 Second point
 * @return Sum of the points
 */
Point add_point(Point P1, Point P2);

/**
 * @brief Create a vector from two points (B - A)
 * @param A Start point
 * @param B End point
 * @return Vector from A to B
 */
Vecteur vect_bipoint(Point A, Point B);

/**
 * @brief Add two vectors
 * @param A First vector
 * @param B Second vector
 * @return Sum of the vectors
 */
Vecteur add_vect(Vecteur A, Vecteur B);

/**
 * @brief Multiply a vector by a scalar
 * @param A Vector to multiply
 * @param x Scalar value
 * @return Scaled vector
 */
Vecteur produit_vecteur(Vecteur A, double x);

/**
 * @brief Multiply a point by a scalar
 * @param P Point to multiply
 * @param a Scalar value
 * @return Scaled point
 */
Point produit_point(Point P, double a);

/**
 * @brief Dot product of two vectors
 * @param A First vector
 * @param B Second vector
 * @return Dot product
 */
double produit_scal(Vecteur A, Vecteur B);

/**
 * @brief Euclidean norm of a vector
 * @param V Vector
 * @return Norm (magnitude) of the vector
 */
double norme_eucl(Vecteur V);

/**
 * @brief Distance between two points
 * @param A First point
 * @param B Second point
 * @return Distance between A and B
 */
double distance(Point A, Point B);

/**
 * @brief Distance from a point to a line segment
 * @param P Point
 * @param A First endpoint of segment
 * @param B Second endpoint of segment
 * @return Distance from P to segment AB
 */
double distance_point_segment(Point P, Point A, Point B);

/**
 * @brief Count segments in a contour
 * @param C Contour
 * @return Number of segments (taille - 1)
 */
unsigned int compte_segments_contours(Contour C);

/**
 * @brief Display number of segments in an array of contours
 * @param T Array of contours
 */
void affiche_nb_segments_tab_contours(Tableau_Contour T);

/* Old name for backward compatibility */
#define supprimer_liste_Point free_contour

#endif
