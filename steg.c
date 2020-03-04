#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
	Structure which holds rgb values for
	each individual pixel.
*/

typedef struct pixel{
	int r, g, b;
} pixel;

/*
	Structure which holds text intended to be used
	for PPM comments
*/

typedef struct text {
	char * text;
	struct text * next;
} text;

/*
	Main PPM structure which hold all data of the PPM,
	including the header info documenting the size and
	max colour values as well as a 2D array holding every
	pixel according to x and y coordinates.
*/

struct PPM {
	char * format;
	struct text * text;
	int cn;
	int width, height, max;
	struct pixel ** pixel;
	unsigned int secret;
};

/*
	Method which reads in a line from the PPM file
	and each character is assigned to a buffer array.
	This sequence will end when a new line is detected.
	A line pointer is also declared using malloc to allocate
	the memory appropriate for the size of the line.
*/

char * readLine(FILE * f){

	int i, j;
	char character;
	char * line;
	char buffer[255];

	i = 0;
	character = getc(f);

	if (character == EOF) {
		return NULL;
	}
	while (character != '\n' && i < (255 -1 )) {
		buffer[i] = character;
		i++;
		character = getc(f);
	}

	if (character != '\n') {
		while (character != '\n') {
			character = getc(f);
		}

		buffer[i] = '\0';
		line = malloc((i+1) * sizeof(char));

		for (j = 0; j <= i; j++) {
			line[i] = buffer[i];
			line[i] = '\0';
		}

		return line;
		free(buffer);
	}
}

/*
	Method which converts PPM file to text and prints it to
	the terminal, with a nested loop printing all pixels
	respective colour values in the 2D array. Header information
	is also printed out, including the width, height and max colour
	values.
*/

void showPPM (struct PPM * im) {

	int i;
	printf("%s\n", "P3");

	printf("%d %d\n", im -> width, im -> height);
	printf("%d\n", im -> max);

	int j, k;
	for(j = 0; j < im -> height; j++) {
		for(k = 0; k < im -> width; k++) {
	  	struct pixel pixel = im -> pixel[j][k];
	    printf("%i %i %i\n", pixel.r, pixel.g, pixel.b);
	  }
	}
}

/*
	Method which extracts comments by using the readline method,
	and assigning each line to the linked list of text that forms a single
	comment. This function is also used by getPPM.
*/

void getText(FILE * f, struct PPM * im) {
	char character = getc(f);
	while(character == '#') {
		ungetc(character, f);
		char * line = readLine(f);
		if(im -> cn == 0) {
			im -> text = (struct text * )malloc(sizeof(struct text));
			im -> text->text = line;
			im -> cn = 1;
		}

		else {
			int i;
			struct text * current = im->text;

			for(i = 1; i < im -> cn; i++) {
				current = current -> next;
			}
			current -> next = (struct text *)malloc(sizeof(struct text));
			current -> next -> text = line;
			im -> cn = im -> cn + 1;
		}
		character = getc(f);
	}
	ungetc(character, f);
}

/*
	Method which allocates rows and columns using memory allocation of the PPM's
	respective height and width, then scanning every individual pixels' rgb values
	and assigning them to the pixel structure, which in turn is stored in a 2D array
	according to the row and column it was found in.
*/

void getPixels(FILE * f, struct PPM * im) {

	im -> pixel = (struct pixel ** ) malloc(sizeof(struct pixel *) * im -> height);
	int i, j;

	for(i = 0; i < im -> height; i++) {
		im -> pixel[i] = (struct pixel * ) malloc(sizeof(struct pixel) * im -> width);
		for(j = 0; j < im -> width; j++) {
			fscanf (f, "%d", &(im->pixel[i][j].r));
			fscanf (f, "%d", &(im->pixel[i][j].g));
			fscanf (f, "%d", &(im->pixel[i][j].b));
		}
	}
}

/*
	Method which uses the getText and getPixels methods, as well as reading header
	information to parse together a structure for a PPM file,
	which is subsequently returned.
*/

struct PPM * getPPM(FILE * f) {
	struct PPM * im = malloc(sizeof(struct PPM));
	im -> format = malloc(2*sizeof(char));
	im -> format = readLine(f);
	im -> cn = 0;
	getText(f, im);

	fscanf (f, "%d", &im -> width);
	fscanf (f, "%d", &im -> height);
	fscanf (f, "%d", &im -> max);

	getPixels(f, im);

	return im;
}

/*
	Helper method for encoding PPM file. Utilises C's random method
	and time to generate a seed which the randomisation will be based
	upon. This random number is used to choose the next pixel, of which
	only the red value is impacted, this makes it easier as red is
	the first value in any RGB format.
	Bit shifting is then done using this random number.
*/

struct PPM * encode (char * txt, struct PPM * im)  {
	srand(time(NULL));
	int i, len, sum, width, random;

	width = im -> width;
	sum = 0;
	len = strlen(txt);

	if((len * 75) < (width * im->height)) {
	  printf("File is appropriate for encoding. \n");
	}
	else {
	  printf("Error- Image size not acceptable. \n");
	  exit(0);
	}

	for(i = 0; i < len;) {
	  random = (rand() % 100);
	  sum = sum + random;
		int row, column;
    row = sum / (width);
    column = sum - (row * width);

    struct pixel * pixel = &(im->pixel[row][column]);

    if(txt[i] != pixel->r) {
	    pixel->r = txt[i];
	    i++;
    }
    else {
    	fprintf(stderr, "%s\n", "Retrying");
		}

	}

	return im;
}

/*
	Method which writes a PPM file to an output PPM file after reading
	the text information. Output file name is taken as a parameter.
*/

void writePPM(FILE * f, struct PPM * im, const char *fileName) {

	FILE *output = NULL;
	int i,j;

	output = fopen(fileName, "width");
	fprintf(output, "P3\n%d %d\n255\n", im -> width, im -> height);

	for(i = 0; i < im -> height; i++) {
		for(j = 0; j < im -> width; j++) {
			struct pixel pixel = im -> pixel[i][j];
			fprintf(output, "%i %i %i \n", pixel.r, pixel.g, pixel.b);		}
	}

	fclose(output);
}

/*
	Helper method which decodes the encoded PPM file.
	Checks to see if the original PPM file and the encoded one have the same height
	so the skipping of pixels when decoding it is successful.
	Also reads in a secret however this is buggy and I have not successfully debugged yet.
*/

char * decode(struct PPM * im1, struct PPM * im2, unsigned int secret) {

	char * buffer = malloc(sizeof(char) * 100);

	if(!(im1 -> height == im2 -> height && im1 -> width == im2 -> width)){
		printf("Error - PPM files do not match. \n");
		exit(0);
	}

	// bug
	// needs repaired, explain at demo.
	/*if(!(secret == im2 -> secret)) {
		printf("Error - The Secret You Have Provided Is Incorrect. \n");
		exit(0);
	}*/

	int i, j, k, l, m;
	k = 0;

	for(i = 0; i < im2 -> height; i++) {
		for(j = 0; j < im2 -> width; j++) {
			if(im2 -> pixel[i][j].r != im1 -> pixel[i][j].r) {
				buffer[k] = im2 -> pixel[i][j].r;
				printf("%c", buffer[k]);
				k = k + 1;
			}
		}
	}

	l = strlen(buffer) - 1;
	if (buffer[l] == '\n') {
		buffer[l] = '\0';
	}

	char * str = malloc(sizeof(char) * (l + 1));

	for(m = 0; m < (l + 1); m++) {
		str[m] = buffer[m];
	}

	free(buffer);
	return str;

}

/*
	Main encoding function which is executed by the argv[1] letter e.
	Uses the encode helper method. Implemented robustness if file cannot
	be opened. Prompts for a message and secret once verification has passed
	that file can be encoded.
*/

void encodeFile(int argc, char const ** argv) {
	char txt[100];
	int i;

	FILE * f = fopen(argv[2], "r");
	if(f == NULL){
		fprintf(stderr,"Error - Unable to open the file");
		exit(0);
	}

	struct PPM * im = getPPM(f);

	fprintf(stderr, "Message to Encode: ");
	fgets(txt, 100, stdin);
	fprintf(stderr, "Secret to Decode (integer): ");
	scanf("%u", &im->secret);

	i = strlen(txt) - 1;
	if(txt[i] == '\n')
		txt[i] = '\0';

	encode(txt, im);

	writePPM(f, im, argv[3]);
	showPPM(im);

	fprintf(stderr, "PPM Encoding Finished.\n");
	fprintf(stderr, "%s %u\n", "Secret To Decode Is", im->secret);
}

/*
	Main decoding function which is executed by the argv[1] letter d.
	Uses the encode helper method. Implemented robustness if file cannot
	be opened. Prompts for a message and secret once verification has passed
	that file can be encoded. Takes in the original PPM file and the encoded file
	then proceeds to pass these to the helper function.
*/

void decodeFile(int argc, char const ** argv){

	unsigned int secret;

	fprintf(stderr, "Secret to Decode (integer)-> ");
	scanf("%u", &secret);

	FILE * im1 = fopen(argv[2], "r");
	FILE * im2 = fopen(argv[3], "r");

	if(im1 == NULL){
		printf("Error - File '%s' could not be opened. \n", argv[2]);
		exit(0);
	};

	if(im2 == NULL){
		printf("Error - File '%s' could not be opened. \n", argv[3]);
		exit(0);
	};

	struct PPM * oPPM = getPPM(im1);
	struct PPM * ePPM = getPPM(im2);

	char * str = decode(oPPM, ePPM, secret);
	printf("%s\n", str);

}

/*
	Main method which checks arguments for e and d, encoding and
	decoding respectively.
*/

int main (int argc, char const *argv[]){

	if(argv[1][0] == 'e'){
		encodeFile(argc, argv);
	}
	else if(argv[1][0] == 'd'){
		decodeFile(argc, argv);
	}

	return 0;
}
