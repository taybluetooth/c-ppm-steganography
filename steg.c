#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PIXEL {
  unsigned int r, g, b;
} PIXEL;

typedef struct PPM {
  char format[3];
  int width;
  int height;
  int max;
} PPM;

typedef struct IMAGE {
  PPM ppm;
  PIXEL** pixels;
} IMAGE;

struct PPM getPPM(FILE * f) {

  struct PPM header;
  fscanf(f, "%s %i ", header.format, &header.width);
  fscanf(f, "%i %i ", &header.height, &header.max);

  return header;
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
