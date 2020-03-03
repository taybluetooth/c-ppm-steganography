#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct pixel{
	int r, g, b;
} pixel;

/* text struct holds strings of comments */
typedef struct text {
	char * text;
	struct text * next;
} text;

struct PPM {
	char * type;
	struct text * text;
	int cn;
	int w, h, max;
	struct pixel ** p;
};

/* readLine used from Lab 3 */
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

/* showPPM prints information stored in PPM struct
 * in the given structural format
 */

void showPPM (struct PPM * im) {

	int i;
	printf("%s\n", "P3");

	printf("%d %d\n", im -> w, im -> h);
	printf("%d\n", im -> max);

	int j, k;
	for(j = 0; j < im -> h; j++) {
		for(k = 0; k < im -> w; k++) {
	  	struct pixel p = im -> p[j][k];
	    printf("%i %i %i\n", p.r, p.g, p.b);
	  }
	}
}


/* Utilizes the above readLine function to find the comments */
void getText(FILE * f, struct PPM * im) {
	char character = getc(f);
	while(character == '#') {
		ungetc(character, f);
		//get comment
		char * line = readLine(f);
		//add comment to linked list
		if(im -> cn == 0) {
			im -> text = (struct text * )malloc(sizeof(struct text));
			im -> text->text = line;
			im -> cn = 1;
		}

		else {
			//previous comments
			int i;
			struct text * current = im->text;

			for(i = 1; i < im -> cn; i++) {
				current = current -> next;
			}
			current -> next = (struct text *)malloc(sizeof(struct text));
			current -> next -> text = line;
			im -> cn = im -> cn + 1;
		}
		//move to next line
		character = getc(f);
	}
	//Restore to starting position
	ungetc(character, f);
}

void getPixels(FILE * f, struct PPM * im) {

	//allocate rows
	im -> p = (struct pixel ** ) malloc(sizeof(struct pixel *) * im -> h);
	int i, j;

	for(i = 0; i < im -> h; i++) {
		//allocate columns
		im -> p[i] = (struct pixel * ) malloc(sizeof(struct pixel) * im -> w);
		for(j = 0; j < im -> w; j++) {
			//get pixels for each
			fscanf (f, "%d", &(im->p[i][j].r)); //get memory address
			fscanf (f, "%d", &(im->p[i][j].g));
			fscanf (f, "%d", &(im->p[i][j].b));
		}
	}
}


/* Return PPM image file from fin */
struct PPM * getPPM(FILE * f) {
	struct PPM * im = malloc(sizeof(struct PPM));
	//Type
	im -> type = malloc(2*sizeof(char));
	im -> type = readLine(f);
	im -> cn = 0;
	getText(f, im);

	//Size
	fscanf (f, "%d", &im -> w);
	fscanf (f, "%d", &im -> h);
	fscanf (f, "%d", &im -> max);

	//Pixels
	getPixels(f, im);

	return im;
}

/* Return PPM image i with message (hidden within r field) */
struct PPM * encode (char * txt, struct PPM * im)  {
	srand(time(NULL)); //Randomize seed. Seed set to current time.
	int i, len, sum, w, random;

	w = im -> w;
	sum = 0;
	len = strlen(txt);

	//Check for compatability
	if((len * 75) < (w * im->h)) {
	  printf("steg: File is appropriate for encoding. \n");
	}
	else {
	  printf("steg: Error- Image size not acceptable. \n");
	  exit(0);
	}

	//for each char
	for(i = 0; i < len;) {
	  random = (rand() % 100);
	  sum = sum + random;
    //calc the pixel in terms of row and col
		int row, column;
    row = sum / (w);
    column = sum - (row * w);

    //get pixel
    struct pixel * p = &(im->p[row][column]); //Use memory address

    //check red pixel val != asci of letter encoded
    if(txt[i] != p->r) {
	    p->r = txt[i];
	    i++;
    }
    else {
    	fprintf(stderr, "%s\n", "Retrying");
		}

	}

	return im;
}

void writePPM(FILE * f, struct PPM * im, const char *fileName) {

	FILE *output = NULL;
	int i,j;

	output = fopen(fileName, "w");
	fprintf(output, "P3\n%d %d\n255\n", im -> w, im -> h);

	for(i = 0; i < im -> h; i++) {
		for(j = 0; j < im -> w; j++) {
			struct pixel p = im -> p[i][j];
			fprintf(output, "%i %i %i \n", p.r, p.g, p.b);		}
	}

	fclose(output);
}


char * decode(struct PPM * im1, struct PPM * im2) {

	char * buffer = malloc(sizeof(char) * 255);

	//confirm that the files are the 'same'
	if(!(im1 -> h == im2 -> h && im1 -> w == im2 -> w)){
		printf("steg: Error - PPM files do not match. \n");
		exit(0);
	}

	int j, k, l, m, n;
	l = 0;

	//for each row
	for(j = 0; j < im2 -> h; j++) {
		//for each column
		for(k = 0; k < im2 -> w; k++) {
			//check if red pixels are not equal
			if(im2 -> p[j][k].r != im1 -> p[j][k].r)
				//get encoded char
				buffer[l] = im2 -> p[j][k].r;
				printf("%c", buffer[l]);
				l = l+1;
		}
	}

	//return txt
	m = strlen(buffer) - 1;
	if (buffer[m] == '\n') {
		buffer[m] = '\0';
	}

	char * str = malloc(sizeof(char) * (m + 1));

	for(n = 0; n < (m + 1); n++) {
		str[n] = buffer[n];
	}

	free(buffer);
	return str;

}


void encodeFile(int argc, char const ** argv) {
	char txt[255];
	char secret[10];
	int i;

	FILE * f = fopen(argv[2], "r");
	if(f == NULL){
		fprintf(stderr," steg: Error - Unable to open the file '%s' .\n", argv[2]);
		exit(0);
	}

	struct PPM * im = getPPM(f);

	//get txt
	fprintf(stderr, "Message to Encode-> ");
	fgets(txt, 255, stdin);
	fprintf(stderr, "Secret to Decode (integer)-> ");
	fgets(secret, 10, stdin);

	i = strlen(txt) - 1;
	if(txt[i] == '\n')
		txt[i] = '\0';

	//encode ppm
	encode(txt, im);

	//output ppm
	//fputc(showPPM, outFin);
	writePPM(f, im, argv[3]);
	showPPM(im);

	fprintf(stderr, "PPM Encoding Finished.\n");
	fprintf(stderr, "%s %s", "Secret To Decode Is", secret);
}

void decodeFile(int argc, char const ** argv){

	FILE * im1 = fopen(argv[2], "r");
	FILE * im2 = fopen(argv[3], "r");

	if(im1 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[2]);
		exit(0);
	}

	if(im2 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[3]);
		exit(0);
	}

	//get files
	struct PPM * oPPM = getPPM(im1);
	struct PPM * ePPM = getPPM(im2);

	//get encoded txt
	char * str = decode(oPPM, ePPM);

	//print txt
	printf("%s\n", str);
}

int main (int argc, char const *argv[]){

	if(argv[1][0] == 'e'){
		encodeFile(argc, argv);
	}
	else if(argv[1][0] == 'd'){
		decodeFile(argc, argv);
	}

	return 0;
}
