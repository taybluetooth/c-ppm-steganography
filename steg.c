#include <stdio.h>

typedef struct{
  unsigned char r, g, b;
} RGB;

struct PPM {
  unsigned int height;
  unsigned int width;
  unsigned int max;
  RGB *pixels;
};

struct PPM getPPM(FILE * f) {
  struct PPM p;
  RGB pixels;
  p.max = 255;

  f = fopen("star-field-ascii.ppm", "r");
  fscanf(f, "%d", &p.height);
  fclose(f);
  return p;
};

/*showPPM(struct PPM * im) {

};

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned intsecret) {

};

char * decode(struct PPM * im, unsigned int secret) {

};*/

int main(int argc, char *argv[]) {
  FILE * f;
  fprintf(stderr, "%s\n", "Stenagography Program");
  getPPM(f);
};
