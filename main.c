/*
 * ASCIICONV
 *
 * Reads from a .ppm image (path at argv[1]).
 *
 * The color infromation from the ppm (at each pixel) is transleted into 
 * brightness values which then are used to create a text file with an ascii 
 * representation of the image.
 *
 * USAGE:
 * 	asciiconv file.ppm
 * 	cat out.txt
 *
 * This file is under General Public License. This means: do with it as you
 * please, but let it remain of public access under the same license.
 *
 * Author: 
 * 	trmaa <trmaayt@gmail.com, 689 93 75 95> (Pablo Trik Marín)
 *	21/2/2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void fatal(char *msg, void (*cb)(int), int status)
{
	fprintf(stderr, "\e[1;31m[fatal]: %s\n\e[0m", msg);
	cb(status);
	fatal("Error on callback, at fatal func!!", exit, EXIT_FAILURE);
}

void help(int status)
{
	fprintf(stderr, "USAGE: asciiconv your.ppm\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The P3 format:\n");
	fprintf(stderr, "\tP3\n\tW H\n\t255\n\tR G B\n\tR G B\n\tR G B\n\t...\n");
	exit(status);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		fatal("Wrong args...", help, EXIT_FAILURE);

	FILE *in = fopen(argv[1], "r");

	if (!in)
		fatal("File does not exist.", exit, EXIT_FAILURE);

	int width, height;
	bool valid_format = fscanf(in, "P3\n%d %d\n255", &width, &height);
	int len = width * height;

	if (!valid_format)
		fatal("Wrong image format.\nImage shoud be ppm (P3).", help, EXIT_FAILURE);

	FILE *out = fopen("out.txt", "w");

	unsigned char r, g, b;
	float bright;

	char bvals[] = { ' ', '.', ':', '-', '+', '|', '(', 'I', 'W' };
	int bvlen = sizeof(bvals);
	int bvindex;

	int iter = 0;

	int prog_per = 0;
	char prog_bar[101];
	for (int i = 0; i < 100; i++)
		prog_bar[i] = '-';
	prog_bar[100] = 0;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int read = fscanf(in, "%hhu %hhu %hhu\n", &r, &g, &b);	

			if (read == -1)
				fatal("Image corrupted.", exit, EXIT_FAILURE);

			bright = (float)(r + g + b) / (0xff * 3);
			bvindex = bright * (bvlen-1);

			for (int i = 0; i < 2; i++) // ascii's height is 4*widht 
				putc(bvals[bvindex], out);

			iter++;
			prog_per = 99 * ((float)iter / len);
			prog_bar[prog_per] = '#';
			//fprintf(stderr, "\e[1;1H\e[2J");
			fprintf(stderr, "[%s]%d%%\n", prog_bar, prog_per);
		}
		putc('\n', out);
	}
}
