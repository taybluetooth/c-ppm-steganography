#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  char comment;
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
  int c;
  char buffer[18];

  /*
    Read in file format P3.
  */

  if (!fgets(buffer, sizeof(buffer), f)) {
    exit(1);
  }

  /*
    Ignore all comments that start with '#'.
  */

  c = getc(f);
  while (c == '#') {
  while (getc(f) != '\n') ;
    c = getc(f);
  }

  /*
    Once line ends with no more #, unassign the character and continue
    reading file.
  */

  ungetc(c, f);

  /*
    Read in header information of PPM file.
  */

  fscanf(f, "%i %i %i", &header.width, &header.height, &header.max);

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

  fprintf(stderr, "P3\n");
  fprintf(stderr, "%i %i\n%i\n", header.width, header.height, header.max);

  for(i = 0; i < header.height; i++){
		for(j = 0; j < header.width; j++){
			fprintf(stderr,"%u %u %u\n", im->pixels[i][j].r,  im->pixels[i][j].g,  im->pixels[i][j].b);
		};
	};

  fprintf(stderr, "%s\n", (char *) "Successfully Converted PPM to Text");

  return 0;

};

void writePPM(FILE * f, struct PPM * im){
	FILE *output = NULL;
	int i,j;

	output = fopen("ppm-conversion", "w");
	fprintf(output, "P3\n%d %d\n255\n", im->header.width, im->header.height);

	for(i=0; i<im->header.height; i++) {
		for(j=0; j<im->header.width;j++){
			fprintf(output, "%i %i %i \n", im->pixels[i][j].r, im->pixels[i][j].g, im->pixels[i][j].b);
    }
	}
  fprintf(stderr, "%s\n", (char *) "Stored in File - ppm-conversion.txt");
	fclose(output);
}

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret) {

  srand(time(NULL)); // randomize seed being set to the current time for pixel placement.

  int i, random, width, sum;

  sum = 0;
  width = im->header.width;

  if((mSize * 75) < (width * im->header.height)){
    printf("steg: File is appropriate for encoding. \n");
  }
  else {
    printf("steg: Error- Image size not acceptable. \n");
    exit(0);
  }

  //for each char
  for (i=0;i<mSize;) {
         random = (rand() % 100);
         sum = sum + random;
         int row, column;
         row = sum / (width);
         column = sum - (row * width);

         //get pixel
         struct PIXEL * p = &(im->pixels[row][column]); //Use memory address

         //check red pixel val != asci of letter encoded
         if(message[i] != p->r){
           p->r = message[i];
           i++;
         }
         else {
           fprintf(stderr, "char [%i] - [%c] same. Retrying. \n", i, message[i]);
        }
  }

  return i;

};

/*
char * decode(struct PPM * im, unsigned int secret) {

};*/

void encodeFile(int argc, char const ** argv){
	char message[255];
	int i;

	FILE * f = fopen(argv[2], "r");
	if(f == NULL){
		fprintf(stderr," steg: Error - Unable to open the file '%s' .\n", argv[2]);
		exit(0);
	}

  PPM im = getPPM(f);
	fprintf(stderr, "Message to Encode-> ");
	fgets(message, 255, stdin);

	i = strlen(message) - 1;
	if(message[i] == '\n')
		message[i] = '\0';

	fprintf(stderr, "encoding in progress...\n");

	//encode ppm
  writePPM(f, &im);
	showPPM(&im);
}

/*
  Main method which calls PPM functions for usage.
*/

int main(int argc, char *argv[]) {
  FILE* f = fopen(argv[1], "r");
  PPM ppm = getPPM(f);
  fprintf(stderr, "%s\n", "Stenagography Program");
  writePPM(f, &ppm);
  showPPM(&ppm);
  fclose(f);
  return 0;
};
