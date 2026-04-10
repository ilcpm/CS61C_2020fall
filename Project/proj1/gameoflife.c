/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				ilcpm
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *new_color = (Color *)malloc(sizeof(Color));
	*new_color = (Color){0, 0, 0};
	int R[8] = {0};
	int G[8] = {0};
	int B[8] = {0};
	for (int bit = 0; bit < 8; bit++) {
		int current_bit_R = (image->image[row][col].R >> bit) & 1;
		int current_bit_G = (image->image[row][col].G >> bit) & 1;
		int current_bit_B = (image->image[row][col].B >> bit) & 1;
		// count RGB in grid
		for (int delta_row = -1; delta_row <= 1; delta_row++) {
			for (int delta_col = -1; delta_col <= 1; delta_col++) {
				if (!(delta_row == 0 && delta_col == 0)) {
					int _row = (row + delta_row + image->rows) % image->rows;
					int _col = (col + delta_col + image->cols) % image->cols;
					R[bit] += (image->image[_row][_col].R >> bit) & 1;
					G[bit] += (image->image[_row][_col].G >> bit) & 1;
					B[bit] += (image->image[_row][_col].B >> bit) & 1;
				}
			}
		}
		// generate new bit
		new_color->R |= ((rule >> (current_bit_R ? 9 : 0) >> R[bit]) & 1) << bit;
		new_color->G |= ((rule >> (current_bit_G ? 9 : 0) >> G[bit]) & 1) << bit;
		new_color->B |= ((rule >> (current_bit_B ? 9 : 0) >> B[bit]) & 1) << bit;
	}

	return new_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *new_image = (Image *)malloc(sizeof(Image));
	new_image->rows = image->rows;
	new_image->cols = image->cols;
	new_image->image = (Color **)malloc(sizeof(Color *) * new_image->rows);

	for (int row = 0; row < image->rows; row++) {
		new_image->image[row] = (Color *)malloc(sizeof(Color) * new_image->cols);
		for (int col = 0; col < image->cols; col++) {
			Color *temp = evaluateOneCell(image, row, col, rule);
			new_image->image[row][col] = *temp;
			free(temp);
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3)
		return -1;

	char **temp = NULL;
	uint32_t rule = strtol(argv[2], temp, 16);
	if (temp != NULL)
		return -1;

	Image *image = readData(argv[1]);
	if (image == NULL)
		return -1;

	Image *new_image = life(image, rule);
	freeImage(image);
	writeData(new_image);
	freeImage(new_image);
	return 0;
}
