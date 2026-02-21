#include <stdio.h>
#include <stdlib.h>

void fatal(char *msg, void (*cb)(int), int status)
{
	fprintf(stderr, "\e[1;31m[fatal]: %s\n\e[0m", msg);
	cb(status);
	fatal("Error on callback, at fatal func!!", exit, EXIT_FAILURE);
}

void help(int status)
{
	fprintf(stderr, "USAGE: asciiconv your.ppm\n");
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
	int valid_format = fscanf(in, "P3\n%d %d\n255", &width, &height);

	if (valid_format == -1)
		fatal("Wrong image format.", help, EXIT_FAILURE);

	FILE *out = fopen("out.txt", "w");

	unsigned char r, g, b;
	float bright;

	char bvals[] = { ' ', '.', ':', '-', '+', '|', '(', 'I', 'W' };
	int bvlen = sizeof(bvals);
	int bvindex;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int read = fscanf(in, "%hhu %hhu %hhu\n", &r, &g, &b);	

			if (read == -1)
				fatal("Image corrupted.", exit, EXIT_FAILURE);

			bright = (float)(r + g + b) / (0xff * 3);
			bvindex = bright * (bvlen-1);

			for (int i = 0; i < 2; i++)
				putc(bvals[bvindex], out);
		}
		putc('\n', out);
	}
}
