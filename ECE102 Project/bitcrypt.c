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
int red_pixel[WIDTH][HEIGHT];
int green_pixel[WIDTH][HEIGHT];
int blue_pixel[WIDTH][HEIGHT];

int main (int argc, char * argv[]) 
{
	int x, y;				// Source coordinates of particle being tracked

	char filename[200];
	int mcount;
	int scount;
	FILE* input;
	int rkey;
	int gkey;
	int bkey;
    	int mlength;
    	mlength=atoi(argv[2]);
    	char message[mlength+1];
    	char temp[500];
	srand(time(NULL));

	for (x = 0; x < WIDTH; x++)
	{
		for (y = 0; y < HEIGHT; y++)
		{
			red_pixel[x][y] = rand()%255;
			green_pixel[x][y] =rand()%255;
			blue_pixel[x][y] = rand()%255;
		}
	}
	green_pixel[0][1]=10;
    	input=fopen(argv[1],"r");
    	while(fgets(temp,mlength+1,input))
    	{	
        	sprintf(message,"%s%s",message,temp);
    	}	
	mcount=0;
	scount=1;
	rkey=atoi(argv[3]);
	gkey=atoi(argv[4]);
	bkey=atoi(argv[5]);
	for (x = 0; x < WIDTH&& mcount<mlength; x++)
	{
		for (y = 0; y < HEIGHT&& mcount<mlength; y++)
		{
			if(scount%rkey==0)
			{
				red_pixel[x][y] = message[mcount];
				mcount++;
			}
			if(scount%gkey==0)
			{
				green_pixel[x][y] =message[mcount];
				mcount++;
			}
			if(scount%bkey==0)
			{
				blue_pixel[x][y] = message[mcount];
				mcount++;
			}
			scount++;
		}
	}
	
	sprintf(filename, "bitcrypted.bmp");
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



