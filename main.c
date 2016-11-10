#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.c"
#include "BFops.h"

#define left -99
#define right -98
#define up -97
#define down -96

Stack *stk;
char *list;
int current_direction = left;
int crow = 0, ccol = 0;
int dim;

void print_grid() {
  int i;
  for (i = 0; i < dim*dim; i++) {
    printf("%c", list[i]);

    if (((i + 1) % dim) == 0)
      printf("\n");
  }
}

void double_list() {
  int new_dim = dim * 2;
  char *new = malloc(new_dim*new_dim);
  char *ptr = list;
  char *itr = new;

  memset(new, 'x', new_dim*new_dim);

  int i;
  for (i = 0; i < dim; i++) {
    memcpy(itr, ptr, dim);
    ptr += dim;
    itr += new_dim;
  }
   
  dim = new_dim;
  list = new;
}

int main(int argc, char **argv) {
  if (argc < 2)
    fprintf(stderr, "Not enough arguments");

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fd;
  stk = malloc(sizeof(stk));
  dim = 8;
  list = malloc(dim*dim);
  memset(list, 'x', dim*dim);

  if ((fd = fopen(argv[1], "r")) == NULL) {
    perror(argv[1]);
    return 1;
  }

  int i = 0;
  while ((read = getline(&line, &len, fd)) != -1) {
    if (line[read-1] == '\n') {
      line[read-1] = '\0';
      read -= 1;
    }

    while (read > dim)
      double_list();

    strncpy((list + dim*i), line, read);
    i++;
  }

  fclose(fd);
  free(line);

  print_grid();

  return 0;
}
