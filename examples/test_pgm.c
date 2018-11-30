#include <stdio.h>

#include "pgm.h"

int main(int argc, char **argv) {
  unsigned char *data;
  int nx,ny;

  data = read_pgm("blue_waters_logo.pgm", &nx, &ny);
  printf("nx: %d, ny: %d\n", nx, ny);
  write_pgm("bw_out.pgm", data, nx,ny);

  return 0;
}
