// taken from K&R "The C programming language"

#define _GNU_SOURCE // for strdup

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>

struct node {
    char *word;
    int count;
    struct node *left;
    struct node *right;
};
struct node *addNode(struct node *p, const char *w);
struct node *findMostCommonNode(struct node *p);

int main(void) {
    char buf[1024];
    struct node *root = NULL;
    
    // add some words to binary tree
    const char *fn = "hamlet.txt";
    FILE *fh = fopen(fn, "r");
    if(fh == NULL) {
      fprintf(stderr, "Failed to open %s: %s\n", fn, strerror(errno));
      return 1;
    }
    while(fgets(buf, sizeof(buf), fh)) {
      const char delim[] = " \n\t()[]{}+-*/<>=%;:'\"\\,.";
      for(char *p = strtok(buf, delim) ; p != NULL ;
          p = strtok(NULL, delim)) {
        root = addNode(root, p);
      }
    }
   
    // find the most frequent word
    double start = omp_get_wtime();
    struct node *maxfreq;
    #pragma omp parallel
    {
      #pragma omp single
      maxfreq = findMostCommonNode(root);
    }
    double end = omp_get_wtime();

    printf("The most common word in %s is '%s' with %d occurances\n", fn,
	   maxfreq->word, maxfreq->count);
    printf("Took %g ms to compute\n", (end-start)*1e3);

    return 0;
}

// recursively walk tree and find most common word, in the subtree  p
struct node *findMostCommonNode(struct node *p) {
  if(p == NULL)
    return NULL;
  
  struct node *left_most_common, *right_most_common;
  #pragma omp task shared(left_most_common) firstprivate(p)
  left_most_common = findMostCommonNode(p->left);
  #pragma omp task shared(right_most_common) firstprivate(p)
  right_most_common = findMostCommonNode(p->right);
  #pragma omp taskwait

  struct node *retval = p;
  if(left_most_common && left_most_common->count > retval->count)
    retval = left_most_common;
  if(right_most_common && right_most_common->count > retval->count)
    retval = right_most_common;

  return retval;
}

struct node *addNode(struct node *p, const char *w) {
  int cond;

  if(p == NULL) {
    p = malloc(sizeof(struct node)); /* allocates memory for the new node */
    p -> word = strdup(w);
    p -> count = 1;
    p -> left = p -> right = NULL;
  } else if ((cond = strcmp(w, p -> word)) == 0) {
    p -> count++;
  } else if(cond < 0) {
    p -> left = addNode(p -> left, w);
  } else {
    p -> right = addNode(p -> right, w);
  }

  return p;
}
