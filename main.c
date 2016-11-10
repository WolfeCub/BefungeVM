#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stack.c"
#include "BFops.h"

#define pos(row, col) (row*dim + col)

Stack *stk;
char *list;
int current_direction = RIGHT;
int crow = 0, ccol = 0;
int dim;
int string_mode = 0;

void double_list() {
  int new_dim = dim * 2;
  char *new = malloc(new_dim*new_dim);
  char *ptr = list;
  char *itr = new;

  memset(new, ' ', new_dim*new_dim);

  int i;
  for (i = 0; i < dim; i++) {
    memcpy(itr, ptr, dim);
    ptr += dim;
    itr += new_dim;
  }
   
  dim = new_dim;
  list = new;
}

void move() {
  if (current_direction == RIGHT) {
    if (ccol + 1 >= dim)
      ccol = 0;
    else
      ccol += 1;
  }
  else if (current_direction == LEFT) {
    if (ccol - 1 < 0)
      ccol = dim-1;
    else
      ccol -= 1;
  }
  else if (current_direction == UP) {
    if (crow - 1 < 0)
      crow = dim-1;
    else
      crow -= 1;
  }
  else if (current_direction == DOWN) {
    if (crow + 1 >= dim)
      crow = 0;
    else
      crow += 1;
  }
} 

int process() {
  char current = list[pos(crow, ccol)];
  int a, b;

  //printf("dim: %d\t", dim);
  //printf("row: %d, col: %d\n", crow, ccol);

  Stack_print(stk);
  //printf("\n");
  //printf("%c", current);

  if (current == STRING) {
    string_mode ^= 1;
    move();
    return 0;
  } 

  if (string_mode == 0) {
    if (current == ADD) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      Stack_push(stk, a+b);
      move();
    } else if (current == SUB) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      Stack_push(stk, b-a);
      move();
    } else if (current == MULT) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      Stack_push(stk, a*b);
      move();
    } else if (current == DIV) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      Stack_push(stk, b/a);
      move();
    } else if (current == MOD) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      Stack_push(stk, b%a);
      move();
    } else if (current == NOT) {
      a = Stack_pop(stk);

      if (a == 0)
        Stack_push(stk, 1);
      else
        Stack_push(stk, 0);
      move();
    } else if (current == GT) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      if (b > a)
        Stack_push(stk, 1);
      else
        Stack_push(stk, 0);

      move();
    } else if (current == PCR) {
      current_direction = RIGHT;
      move();
    } else if (current == PCL) {
      current_direction = LEFT;
      move();
    } else if (current == PCU) {
      current_direction = UP;
      move();
    } else if (current == PCD) {
      current_direction = DOWN;
      move();
    } else if (current == PCRAND) {
      a = 0 - ((rand() % 4) + 96);
      current_direction = a;
      move();
    } else if (current == HIF) {
      a = Stack_pop(stk);

      if (a == 0)
        current_direction = RIGHT;
      else
        current_direction = LEFT;
      move();
    } else if (current == VIF) {
      a = Stack_pop(stk);

      if (a == 0)
        current_direction = DOWN;
      else
        current_direction = UP;
      move();
    } else if (current == PEEK) {
      Stack_push(stk, Stack_peek(stk));
      move();
    } else if (current == SWAP) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);
      Stack_push(stk, a);
      Stack_push(stk, b);
      move();
    } else if (current == POPR) {
      Stack_pop(stk);
      move();
    } else if (current == POPI) {
      printf("%d", Stack_pop(stk));
      move();
    } else if (current == POPC) {
      char ch = Stack_pop(stk);
      printf("%c", ch);
      move();
    } else if (current == BRIDGE) {
      move();
      move();
    } else if (current == GET && string_mode == 0) {
      a = Stack_pop(stk);
      b = Stack_pop(stk);

      if (a >= dim-1 || b >= dim-1)
        Stack_push(stk, 0);
      else
        Stack_push(stk, pos(b, a));
      move();
    } else if (current == PUT && string_mode == 0) {
      int c;
      a = Stack_pop(stk);
      b = Stack_pop(stk);
      c = Stack_pop(stk);

      while (a >= dim || b >= dim)
        double_list();

      list[pos(b,a)] = c;
      move();
    } else if (current == READI) {
      scanf(" %d", &a);
      getchar();

      Stack_push(stk, a);

      move();
    } else if (current == READC) {
      char ch;
      scanf(" %c", &ch);
      getchar();
      a = ch;
      Stack_push(stk, a);

      move();
    } else if (current == NOOP) {
      move();
    }
    else if (current == END) {
      return -1;
    } else {
      Stack_push(stk, current-48);
      move();
    }
  }
  else if (string_mode == 1) {
    a = current;
    Stack_push(stk, a);
    move();
  }

  return 0;
}

void print_grid() {
  int i;
  for (i = 0; i < dim*dim; i++) {
    printf("%c", list[i]);

    if (((i + 1) % dim) == 0)
      printf("\n");
  }
}

int main(int argc, char **argv) {
  if (argc < 2)
    fprintf(stderr, "Not enough arguments");

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fd;
  srand(time(NULL));
  stk = malloc(sizeof(stk));
  dim = 8;
  list = malloc(dim*dim);
  //memset(list, 'x', dim*dim);

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

  int val = process();
  while (val == 0)
    val = process();

  return 0;
}
