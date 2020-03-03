#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct PIXEL{
	int r, g, b;
} PIXEL;

/* text struct holds strings of comments */
typedef struct TEXT {
	char * text;
	struct text * next;
} TEXT;

struct PPM {
	char * format;
	struct text * comment;
	int cn;
	int width, height, max;
	struct pixel ** pixels;
};

char * readLine(FILE * f){

	char buffer[255];
	int i, j;
	char character;
	char * line;

	i = 0;
	character = getc(f);
	if (character == EOF)
		return NULL;
	while (character !='\n' && i < (255 -1)){
		buffer[i] = ch;
		i++;
		character = getc(f);
	}

	if (character != '\n')
		while (ch != '\n')
			character = getc(f);
	buffer[i] = '\0';
	line = malloc((i+1) * sizeof(char));

	for (j = 0; j <= i; k++)
		line[i] = buffer[j];
		line[i] = '\0';

	return line;
	free(buffer);
}


/* showPPM prints information stored in PPM struct
 * in the given structural format
 */
showPPM (struct PPM * im){

  //print format
  printf("%s\n", im->format);

  //print text
  int count;
  struct text * text = im->comment;
  printf("%s\n", im->comment->text);
  for(j = 1; j<i->cn; j++){
          text = text->next;
          printf("%s\n", text->text);
  }

  //print general info
  printf("%d\n", im->width);
  printf("%d\n", im->height);
  printf("%d\n", im->max);

  //print pixels
  int i, j;
  for(i = 0; i < im -> height; i++){
    for(j = 0; j < im -> width; j++){
      struct pixel p = im->pixels[i][j];
      printf("%i %i %i\n", p.r, p.g, p.b);
    }
  }
}


/* Utilizes the above readLine function to find the comments */
void getText(FILE * f, struct PPM * im){
	char character = getc(f);
	while(character == '#') //lines beginning with "#" character
	{
		ungetc(character, f);

		//get comment
		char * line = readLine(f);

		//add comment to linked list
		if(im->cn == 0) {
			im->comment = (struct text * )malloc(sizeof(struct text));
			im->comment->text = line;
			im->cn = 1;
		}
		else {
			//previous comments
			int i;
			struct text * current = im->comment;

			for(i = 1; i < im -> cn; i++) {
  			current = current -> next;
  			current -> next = (struct text *)malloc(sizeof(struct text));
			  current -> next -> text = line;
			  im -> cn = im->cn + 1;
		}

		character = getc(f);
	}

	//Restore to starting position
	ungetc(character, f);
}

void getPixels(FILE * f, struct PPM * im){

	//allocate rows
	im-> pixels = (struct pixel ** ) malloc(sizeof(struct pixel *) * im -> height);

	int i, j;
	for(i = 0; i < im -> height; i++) {
		//allocate columns
		im -> pixels[j] = (struct pixel * ) malloc(sizeof(struct pixel) * im -> width);
		for (j = 0; j < im -> width; j++) {
			//get pixels for each
			fscanf (f, "%d", &(im -> pixels[i][j].r)); //get memory address
			fscanf (f, "%d", &(im -> pixels[i][j].g));
			fscanf (f, "%d", &(im -> pixels[i][j].b));
		}
	}
}


/* Return PPM image file from fin */
struct PPM * getPPM(FILE * f){
  struct PPM * im = malloc(sizeof(struct PPM));

  //Type
  im->type = malloc(2*sizeof(char));
  im->type = readLine(f);

  im -> cn = 0;
  getText(f, im);

  //Size
  fscanf (f, "%d", &im -> width);
  fscanf (f, "%d", &im -> height);
  fscanf (f, "%d", &im -> max);

  //Pixels
  getPixels(f, im);

  return im;
}

/* Return PPM image i with message (hidden within r field) */
struct PPM * encode (struct PPM * im, char * message, unsigned int mSize, unsigned intsecret){
        srand(time(NULL)); //Randomize seed. Seed set to current time.
        int i, width, sum, random;

        width = im -> width;
        sum = 0;

        //Check for compatability
        if((mSize * 75) < (width * im -> height)){
          printf("steg: File is appropriate for encoding. \n");
        }
        else {
          printf("steg: Error- Image size not acceptable. \n");
          exit(0);
        }

        //for each char
        for (i = 0; i < mSize;) {
          random = (rand() % 100);
          sum = sum + random;

          //calc the pixel in terms of row and col
		      int row, column;
          row = sum / (width);
          column = sum - (row * width);

          //get pixel
          struct pixel * pixel = &(im->pixels[row][column]); //Use memory address

          //check red pixel val != asci of letter encoded
          if(message[i] != pixel -> r){
            pixel -> r = message[i];
            i++;
          }
          else {
            fprintf(stderr, "%s\n", "Retrying");
          }
  }
  return i;
}

void writePPM(FILE * f, struct PPM * im, const char *fileName){
	FILE *pfile = NULL;
	int x,y;

	output = fopen(fileName, "w");
	fprintf(output, "P3\n%d %d\n255\n", i->w, i->h);

	for(i = 0; i < im -> height; i++) {
		for(j = 0; j < im -> width; j++){
			struct pixel pixel = im -> pixels[x][y];
			fprintf(output, "%i %i %i \n", pixel.r, pixel.g, pixel.b);		}
	}
	fclose(output);
}


char * decode(struct PPM * i1, struct PPM * i2){
	char * buffer = malloc(sizeof(char) * 255);

	//confirm that the files are the 'same'
	if(!(i1->h == i2->h && i1->w == i2->w)){
		printf("steg: Error - PPM files do not match. \n");
		exit(0);
	}

	int j, k, l, m, n;
	l = 0;

	//for each row
	for(j=0;j<i2->h;j++){
		//for each column
		for(k=0;k<i2->w;k++){
			//check if red pixels are not equal
			if(i2->p[j][k].r != i1->p[j][k].r)
				//get encoded char
				buffer[l] = i2->p[j][k].r;
				printf("%c", buffer[l]);
				l = l+1;
		}
	}

	//return txt
	m = strlen(buffer) - 1;
	if (buffer[m] == '\n')
		buffer[m] = '\0';

	char * str = malloc(sizeof(char) * (m+1));

	for(n=0; n<(m+1); n++)
		str[n] = buffer[n];

	free(buffer);
	return str;

}


void encodeFile(int argc, char const ** argv){
	char txt[255];
	int j;
	//FILE * outFin;

	//outFin = fopen("modified.ppm", "w");

	FILE * fin = fopen(argv[2], "r");
	if(fin == NULL){
		fprintf(stderr," steg: Error - Unable to open the file '%s' .\n", argv[2]);
		exit(0);
	}

	struct PPM * i = getPPM(fin);

	//get txt
	fprintf(stderr, "Message to Encode-> ");
	fgets(txt, 255, stdin);

	j = strlen(txt) - 1;
	if(txt[j] == '\n')
		txt[j] = '\0';

	fprintf(stderr, "encoding in progress...\n");

	//encode ppm
	encode(txt, i);

	//output ppm
	//fputc(showPPM, outFin);
	writePPM(fin, i, "modified.ppm");
	showPPM(i);
}

void decodeFile(int argc, char const ** argv){

	FILE * i1 = fopen(argv[2], "r");
	FILE * i2 = fopen(argv[3], "r");

	if(i1 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[2]);
		exit(0);
	}

	if(i2 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[3]);
		exit(0);
	}

	//get files
	struct PPM * oPPM = getPPM(i1);
	struct PPM * ePPM = getPPM(i2);

	//get encoded txt
	char * str = decode(oPPM, ePPM);

	//print txt
	printf("%s\n", str);
}

int main (int argc, char const *argv[]){

	//check argv
	if(argc < 3){
		printf("steg: Improper use of steg\n");
		printf("Use either:  ./steg e <file1.ppm>\n");
		printf("             ./steg d <file1.ppm> <file2.ppm>\n");
		exit(0);
	}

	if(argv[1][0] == 'e'){
		encodeFile(argc, argv);
	}
	else if(argv[1][0] == 'd'){
		if(argc != 4){
			printf("steg: Improper use of steg\n");
                	printf("Use either:  ./steg e <file1.ppm>\n");
                	printf("             ./steg d <file1.ppm> <file2.ppm>\n");
                	exit(0);
		}
		decodeFile(argc, argv);
	}
	else{
		printf("steg: Improper use of steg\n");
                printf("Use either:  ./steg e <file1.ppm>\n");
                printf("             ./steg d <file1.ppm> <file2.ppm>\n");
                exit(0);
	}

	return 0;
}
