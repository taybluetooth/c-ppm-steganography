#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PIXEL {
  unsigned int r, g, b;
} PIXEL;

typedef struct HEADER {
  char format[3];
  int width;
  int height;
  int max;
} HEADER;

typedef struct PPM {
  HEADER header;
  PIXEL** pixels;
} PPM;

PPM* mem_alloc(HEADER header){ //take header info to allocate memory for the new files

	int i = 0;
	PPM* image = (PPM *)malloc(sizeof(PPM));

	PIXEL** pixels = (PIXEL **)malloc(header.height * sizeof(PIXEL *));
	for(i = 0; i < header.height; i++){
		pixels[i] = (PIXEL *)malloc(header.width * sizeof(PIXEL));
	}

	image->pixels = pixels;
	return image;
}

struct HEADER getHeader(FILE * f) {
  struct HEADER header;
  fscanf(f, "%s %i ", header.format, &header.width);
  fscanf(f, "%i %i ", &header.height, &header.max);

  return header;
}

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

struct PPM getPPM(FILE * f) {

  struct PPM *ppm;

  HEADER header = getHeader(f);
	PPM* image = getPixels(f, header);

  ppm = &image;

  fprintf(stderr, "%s\n", "PPM Structure Successfully Populated");

	return *ppm; //image pointer return

};

/*showPPM(struct PPM * im) {

};

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned intsecret) {

};

char * decode(struct PPM * im, unsigned int secret) {

};*/

int main(int argc, char *argv[]) {
  FILE* f = fopen(argv[1], "r");
  fprintf(stderr, "%s\n", "Stenagography Program");
  getPPM(f);
};
