#ifndef PGM_H
#define PGM_H

unsigned char *read_pgm(const char *filename, int *nx, int *ny);
void write_pgm(const char *filename, const unsigned char *data, int nx, int ny);

#endif // PGM_H
