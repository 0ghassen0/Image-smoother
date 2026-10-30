#ifndef _CALCUL_CONTOUR_H_
#define _CALCUL_CONTOUR_H_

#include "image.h"
#include "geom2d.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

/**
 * @brief Orientation enum for contour tracing
 */
typedef enum {Nord, Est, Sud, Ouest} Orientation;

/**
 * @brief Find the starting point for contour tracing
 * @param I Input image
 * @return Starting point for contour tracing
 */
Point depart_contour(Image I);

/**
 * @brief Move a point in the given orientation
 * @param P Point to move
 * @param O Orientation to move in
 * @return New position after moving
 */
Point avancer(Point P, Orientation O);

/**
 * @brief Calculate all contours in an image
 * @param I Input image
 * @return Array of contours found in the image
 */
Tableau_Contour calcule_contours(Image I);

/**
 * @brief Trace and memorize a contour starting from a point
 * @param I Input image
 * @param depart Starting point
 * @param C Initial contour (empty)
 * @param M Mask image for tracking visited pixels
 * @return Traced contour
 */
Contour memoriser_position(Image I, Point depart, Contour C, Image M);

/**
 * @brief Save a single contour as EPS file
 * @param C Contour to save
 * @param nom_fichier Output filename
 * @param modef Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void contour_eps(Contour C, char* nom_fichier, char* modef, int h, int l);

/**
 * @brief Save an array of contours as EPS file
 * @param C Array of contours
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_contour_eps(Tableau_Contour C, char* nom_fichier, char* mode, int h, int l);

/**
 * @brief Determine new orientation during contour tracing
 * @param I Input image
 * @param P Current position
 * @param O Current orientation
 * @return New orientation based on neighboring pixels
 */
Orientation nouvelle_orientation(Image I, Point P, Orientation O);

/**
 * @brief Apply Douglas-Peucker simplification to a contour
 * @param Cont Input contour
 * @param j1 Start index
 * @param j2 End index
 * @param d Maximum allowed distance
 * @return Simplified contour
 */
Contour simplification_douglas_peucker_rec(Contour Cont, int j1, int j2, double d);

/**
 * @brief Apply Douglas-Peucker simplification to all contours in an array
 * @param Cont Array of contours
 * @param d Maximum allowed distance
 */
void simpl_doug(Tableau_Contour Cont, double d);

/**
 * @brief Save an array of contours as SVG
 * @param C Array of contours
 * @param nom_fichier Output filename
 * @param mode Drawing mode ("stroke" or "fill")
 * @param h Image height
 * @param l Image width
 */
void sequence_contour_svg(Tableau_Contour C, char* nom_fichier, char* mode, int h, int l);

#endif
