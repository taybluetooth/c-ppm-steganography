#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  A structure that will hold an rgb pixel of
  max value 255.
*/

typedef struct PIXEL {
  unsigned int r, g, b;
} PIXEL;

/*
  A structure that holds the PPM header, outlining
  all important information of the file.
*/

typedef struct HEADER {
  char format[3];
  int width;
  int height;
  int max;
} HEADER;

/*
  Combined structure of PPM file that holds both
  the header and an array of all pixels in the file.
*/

typedef struct PPM {
  HEADER header;
  PIXEL** pixels;
} PPM;

/*
  Additional function which allocates memory corresponding
  to the size of the completed header to the final PPM image,
  then allocating each pixel into the array.
*/

PPM* mem_alloc(HEADER header){
	int i = 0;
	PPM* image = (PPM *)malloc(sizeof(PPM));

	PIXEL** pixels = (PIXEL **)malloc(header.height * sizeof(PIXEL *));
	for(i = 0; i < header.height; i++){
		pixels[i] = (PIXEL *)malloc(header.width * sizeof(PIXEL));
	}

	image->pixels = pixels;
	return image;
}

/*
  Function which reads in the header of the PPM file and assigns it
  to the pre-defined Header struct, returning said header.
*/

struct HEADER getHeader(FILE * f) {
  struct HEADER header;
  fscanf(f, "%s %i ", header.format, &header.width);
  fscanf(f, "%i %i ", &header.height, &header.max);

  return header;
}

/*
  Method which gets all pixels from the PPM file, and within a loop, allocates
  each pixel to an index of the pixel array, while utilising the memory allocation
  function I created.
*/

PPM* getPixels(FILE* f, HEADER header) {
  int i = 0;
	int j = 0;
  PPM* image = mem_alloc(header); //calls the allocation function
	memcpy(&(image->header), &header, sizeof(header)); //populate the image pointer with the header data.

  for(i = 0; i < header.height; i++){
		for(j = 0; j < header.width; j++){
			fscanf(f, "%u", &image->pixels[i][j].r);
			fscanf(f, "%u", &image->pixels[i][j].g);
			fscanf(f, "%u", &image->pixels[i][j].b);
		};
	};

	return image;
};

/*
  Method which returns the fully completed PPM structure after calling
  both helper functions.
*/

struct PPM getPPM(FILE * f) {

  struct PPM *ppm;

  HEADER header = getHeader(f);
	PPM* image = getPixels(f, header);

  ppm = image;

  fprintf(stderr, "%s\n", "PPM Structure Successfully Populated");

	return *ppm; //image pointer return

};

/*
  Method which returns the PPM file in a text format,
  storing it in a text file for easier viewing and analysis.
*/

int showPPM(struct PPM * im) {

  int i = 0;
  int j = 0;
  HEADER header = im -> header;
  FILE* f = fopen("PPMConvert.txt", "w");

  fprintf(f, "P3\n%i ", header.width);
  fprintf(f, "%i\n%i\n", header.height, header.max);

  for(i = 0; i < header.height; i++){
		for(j = 0; j < header.width; j++){
			fprintf(f,"%u %u %u\n", im->pixels[i][j].r,  im->pixels[i][j].g,  im->pixels[i][j].b);
		};
	};

  fprintf(stderr, "%s\n", (char *) "Successfully Converted PPM to Text");
  fprintf(stderr, "%s\n", (char *) "Stored in File - PPMConvert.txt");


  return 0;

};

/*struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned intsecret) {

};

char * decode(struct PPM * im, unsigned int secret) {

};*/

/*
  Main method which calls PPM functions for usage.
*/

int main(int argc, char *argv[]) {
  FILE* f = fopen(argv[1], "r");
  PPM ppm = getPPM(f);
  fprintf(stderr, "%s\n", "Stenagography Program");
  showPPM(&ppm);
  fclose(f);
  return 0;
};
