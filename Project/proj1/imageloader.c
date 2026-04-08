/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				ilcpm
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object.
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
		return NULL;

	Image *img = (Image *)malloc(sizeof(Image));
	char buf[20];
	fscanf(fp, "%s", buf); // drop first line
	fscanf(fp, "%d %d", &(img->cols), &(img->rows));
	fscanf(fp, "%s", buf); // drop third line

	img->image = (Color**)malloc(sizeof(Color *) * img->rows);

	for (int row = 0; row < img->rows; row++) {
		img->image[row] = (Color*)malloc(sizeof(Color) * img->cols);
		for (int col = 0; col < img->cols; col++) {
			fscanf(
				fp, "%hhu %hhu %hhu",
				&(img->image[row][col].R),
				&(img->image[row][col].G),
				&(img->image[row][col].B)
			);
		}
	}
	fclose(fp);

	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int row = 0; row < image->rows; row++) {
		for (int col = 0; col < image->cols; col++) {
			printf(
				"%3d %3d %3d",
				image->image[row][col].R,
				image->image[row][col].G,
				image->image[row][col].B
			);
			if (col != image->cols - 1)
				printf("   ");
			else
				printf("\n");
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int row = 0; row < image->rows; row++) {
		free(image->image[row]);
	}
	free(image->image);
	free(image);
}

// int main() {
// 	Image *image = readData("./testInputs/blinkerH.ppm");
// 	writeData(image);
// 	return 0;
// }
