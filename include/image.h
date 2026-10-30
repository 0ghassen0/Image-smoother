/******************************************************************************
 * Image Module Interface
 * 
 * This module provides functions for creating, manipulating, and freeing
 * black and white PBM (Portable Bitmap) images.
 ******************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

/* Boolean type */
typedef char BOOL;
#define FALSE 0
#define TRUE  1

/* Unsigned integer type */
#ifndef UINT
#define UINT unsigned int
#endif

/* Macro to display error message and exit */
#ifndef ERREUR_FATALE
#define ERREUR_FATALE(_m_) \
{ \
    fprintf(stderr, "%s\n", _m_); \
    exit(1); \
}
#endif

/**
 * @brief Pixel type - equivalent to char with BLANC=0 and NOIR=1
 */
typedef enum {BLANC=0, NOIR=1} Pixel;

/**
 * @brief Image type - represents a black and white image
 */
typedef struct Image_
{
	UINT la_largeur_de_l_image;         /**< Image width in pixels */
	UINT la_hauteur_de_l_image;        /**< Image height in pixels */
	Pixel* pointeur_vers_le_tableau_de_pixels;  /**< Pointer to the pixel data array */
} Image;

/**
 * @brief Create a PBM image of dimensions L x H with all pixels white
 * @param L Width of the image
 * @param H Height of the image
 * @return New image with all pixels set to BLANC
 */
Image creer_image(UINT L, UINT H);

/**
 * @brief Free memory allocated for an image
 * @param p_I Pointer to image to free
 */
void free_image(Image *p_I);

/**
 * @brief Get the value of pixel (x,y) from image I
 * @param I Image
 * @param x X coordinate (1-based)
 * @param y Y coordinate (1-based)
 * @return Pixel value at (x,y), or BLANC if out of bounds
 */
Pixel get_pixel_image(Image I, int x, int y);

/**
 * @brief Set the value of pixel (x,y) in image I to value v
 * @param I Image
 * @param x X coordinate (1-based)
 * @param y Y coordinate (1-based)
 * @param v Pixel value to set (BLANC or NOIR)
 */
void set_pixel_image(Image I, int x, int y, Pixel v);

/**
 * @brief Get the width of image I
 * @param I Image
 * @return Width in pixels
 */
UINT largeur_image(Image I);

/**
 * @brief Get the height of image I
 * @param I Image
 * @return Height in pixels
 */
UINT hauteur_image(Image I);

/**
 * @brief Read an image from file
 * @param nom_f Filename to read
 * @return Image loaded from file
 * 
 * File format:
 * - Line 1: P1
 * - Zero, one, or more lines starting with #
 * - Zero, one, or more separators
 * - The width
 * - One or more separators
 * - The height
 * - One or more separators
 * - The image pixel data (0 for BLANC, 1 for NOIR)
 * 
 * If there is an error in the file, the program stops and displays a message.
 */
Image lire_fichier_image(char *nom_f);

/**
 * @brief Write image I to screen
 * @param I Image to display
 * 
 * Displays the image using '.' for BLANC pixels and '@' for NOIR pixels.
 */
void ecrire_image(Image I);

/**
 * @brief Calculate the "negative" of image I
 * @param I Input image
 * @return New image where BLANC and NOIR pixels are inverted
 * 
 * The input image I is not modified.
 */
Image negatif_image(Image I);

/**
 * @brief Check if image I is completely white (all pixels are BLANC)
 * @param I Image to check
 * @return TRUE if all pixels are BLANC, FALSE otherwise
 */
BOOL image_blanche(Image I);

/* Old name for backward compatibility */
#define supprimer_image free_image

#endif /* _IMAGE_H_ */
