
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"image.h"

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <image1.pbm> <image2.pbm>\n", argv[0]);
        return 1;
    }

    Image I = lire_fichier_image(argv[1]);
    ecrire_image(I);

    Image N_I = negatif_image(I);
    ecrire_image(N_I);
    free_image(&N_I);

    Image I2 = lire_fichier_image(argv[2]);
    ecrire_image(I2);
    Image N_I2 = negatif_image(I2);
    ecrire_image(N_I2);
    free_image(&N_I2);

    free_image(&I);
    free_image(&I2);

    return 0;
}
