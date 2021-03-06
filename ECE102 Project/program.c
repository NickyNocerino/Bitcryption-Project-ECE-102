// Nebulabrot / Buddhabrot generator.
// Brought to you by Wikipedia...
// Written by User:Evercat
//
// Released under the GNU Free Documentation License
// or the GNU Public License, whichever you prefer:
// November 23, 2004
//
// This code is lame and confusing. I apologise.
// As I like to point out, my C is self-taught.
//
// Note: some folk mention possible improvements on the talk page:
// http://en.wikipedia.org/wiki/User_talk:Evercat/Buddhabrot.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 1024
#define HEIGHT 768

void drawbmp (char * filename);
void boxdraw(unsigned int r, unsigned int b, unsigned int g,unsigned int x_left,unsigned int y_bottom,unsigned int x_size,unsigned int y_size);
int red_pixel[WIDTH][HEIGHT];
int green_pixel[WIDTH][HEIGHT];
int blue_pixel[WIDTH][HEIGHT];

int main (int argc, char * argv[]) 
{
	int x, y;				// Source coordinates of particle being tracked

	char filename[200];

	srand(time(NULL));

	for (x = 0; x < WIDTH; x++)
	{
		for (y = 0; y < HEIGHT; y++)
		{
			red_pixel[x][y] = atoi(argv[1]);
			green_pixel[x][y] =atoi( argv[2]);
			blue_pixel[x][y] = atoi(argv[3]);
		}
	}

	/* implement this functon */
	boxdraw(atoi(argv[4]),atoi(argv[5]), atoi(argv[6]), 37, 400, 20, 300);	
	boxdraw(atoi(argv[4]),atoi(argv[5]),atoi( argv[6]), 57, 600, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]),atoi( argv[6]), 77, 500, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]),atoi( argv[6]), 97, 400, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]),atoi( argv[6]), 117, 400, 20, 300);
	boxdraw(atoi(argv[4]),atoi(argv[5]),atoi( argv[6]), 237, 400, 20, 300);
	boxdraw(atoi(argv[4]),atoi(argv[5]), atoi(argv[6]), 257, 600, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]), atoi(argv[6]), 277, 500, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]), atoi(argv[6]), 297, 400, 20, 100);
	boxdraw(atoi(argv[4]),atoi(argv[5]), atoi(argv[6]), 317, 400, 20, 300);
	sprintf(filename, "initials.bmp");
	drawbmp(filename);
	
	printf("Done.\n");
	return 0;
}

void drawbmp (char * filename) 
{
	unsigned int headers[13];
	FILE * outfile;
	int extrabytes;
	int paddedsize;
	int x; int y; int n;
	int red, green, blue;
	
	extrabytes = 4 - ((WIDTH * 3) % 4);                 // How many bytes of padding to add to each
	                                                    // horizontal line - the size of which must
	                                                    // be a multiple of 4 bytes.
	if (extrabytes == 4)
		extrabytes = 0;
	
	paddedsize = ((WIDTH * 3) + extrabytes) * HEIGHT;
	
	// Headers...
	// Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".
	
	headers[0]  = paddedsize + 54;      // bfSize (whole file size)
	headers[1]  = 0;                    // bfReserved (both)
	headers[2]  = 54;                   // bfOffbits
	headers[3]  = 40;                   // biSize
	headers[4]  = WIDTH;  // biWidth
	headers[5]  = HEIGHT; // biHeight
	
	// Would have biPlanes and biBitCount in position 6, but they're shorts.
	// It's easier to write them out separately (see below) than pretend
	// they're a single int, especially with endian issues...
	
	headers[7]  = 0;                    // biCompression
	headers[8]  = paddedsize;           // biSizeImage
	headers[9]  = 0;                    // biXPelsPerMeter
	headers[10] = 0;                    // biYPelsPerMeter
	headers[11] = 0;                    // biClrUsed
	headers[12] = 0;                    // biClrImportant
	
	outfile = fopen(filename, "wb");
	
	//
	// Headers begin...
	// When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
	//
	
	fprintf(outfile, "BM");
	
	for (n = 0; n <= 5; n++)
	{
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	// These next 4 characters are for the biPlanes and biBitCount fields.
	
	fprintf(outfile, "%c", 1);
	fprintf(outfile, "%c", 0);
	fprintf(outfile, "%c", 24);
	fprintf(outfile, "%c", 0);
	
	for (n = 7; n <= 12; n++)
	{
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}
	
	//
	// Headers done, now write the data...
	//
	
	for (y = 0; y < HEIGHT; y++)     // BMP image format is written from bottom to top...
	{
		for (x = 0; x < WIDTH; x++)  // left to right
		{
			red = red_pixel[x][y];
			green = green_pixel[x][y];
			blue = blue_pixel[x][y];
	
			if (red > 255) 
				red = 255; 
			if (red < 0) 
				red = 0;
			if (green > 255) 
				green = 255; 
			if (green < 0) 
				green = 0;
			if (blue > 255) 
				blue = 255; 
			if (blue < 0) 
				blue = 0;
	
			// Also, it's written in (b,g,r) format...
	
			fprintf(outfile, "%c", blue);
			fprintf(outfile, "%c", green);
			fprintf(outfile, "%c", red);
		}
		if (extrabytes)      // See above - BMP lines must be of lengths divisible by 4.
		{
			for (n = 1; n <= extrabytes; n++)
			{
				fprintf(outfile, "%c", 0);
			}
		}
	}
	
	fclose(outfile);
	return;
}
void boxdraw(unsigned int red,unsigned int blue, unsigned int green,unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	int i, j;
	for(i=x;i<x+width; i++)
	{
		for(j=y;j<y+height;j++)
		{
			red_pixel[i][j]=red;
			green_pixel[i][j]=green;
			blue_pixel[i][j]=blue;
		}
	}

}

