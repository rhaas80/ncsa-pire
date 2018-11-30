// utility code to read / write PGM image files

#include "pgm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// a very simple minded routine to read a PGM image
unsigned char *read_pgm(const char *filename, int *nx, int *ny)
{
  unsigned char *retval = NULL;

  FILE *fh = fopen(filename, "r");
  if(fh) {
    char buf[200];
    if(fgets(buf, sizeof(buf), fh)) {
      if(strcmp("P2\n", buf) == 0) {
        if(fscanf(fh, "%d %d\n", nx, ny) == 2) {
          retval = malloc((size_t)(*nx * *ny));
          if(retval != NULL) {
            int maxval = -1;
            if(fscanf(fh, "%d\n", &maxval) == 1) {
              for(int j = 0 ; j < *ny ; j++) {
                for(int i = 0 ; i < *nx ; i++) {
                  int val = -1;
                  if(fscanf(fh, "%d", &val) == 1) {
                    retval[i + j * *nx] = (unsigned char)(255*val/maxval);
                  } else {
                    fprintf(stderr, "Could not read from '%s': %s\n",
                            filename, strerror(errno));
                    fclose(fh);
                    free(retval);
                    return NULL;
                  } // if fscanf
                } // for i
              } // for j
            } else {
              fprintf(stderr, "Could not read maxval from '%s': %s\n",
                      filename, strerror(errno));
            } // if fscanf
          } else { 
            fprintf(stderr, "Could not allocate %d bytes", *nx * *ny);
          } // if retval
        } else {
          fprintf(stderr, "Could not read dimensions from '%s'\n", filename);
        } // if fscanf
      } else {
        fprintf(stderr, "Did not find P2 header in '%s'\n", filename);
      }// if strcmp
    } else {
      fprintf(stderr, "Could not read from '%s': %s\n", filename,
              strerror(errno));
    } // if fgets
    fclose(fh);
  } else {
    fprintf(stderr, "Could not open file '%s':%s\n", filename, strerror(errno));
  }
  return retval;
}

// a very simple minded routine to write a PGM image to disk
void write_pgm(const char *filename, const unsigned char *data, int nx, int ny)
{
  FILE *fh = fopen(filename, "w");
  if(fh == NULL) {
    fprintf(stderr, "Could not open '%s' for writing: %s\n", filename,
            strerror(errno));
    return;
  }

  // we ignore all errors from now on...
  fputs("P2\n", fh);
  fprintf(fh, "%d %d\n", nx, ny);
  fprintf(fh, "%d\n", 255);
  for(int j = 0 ; j < ny ; j++) {
    for(int i = 0 ; i < nx ; i++) {
      fprintf(fh, "%d%c", data[i + j*nx], i==nx-1 ? '\n' : ' ');
    }
  }

  fclose(fh);
}
