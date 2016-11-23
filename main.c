#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "stack.c"
#include "BFops.h"

#define pos(row, col) ((row)*dim + (col))
#define hash(x) (x-32)

Stack *stk;
char *list;
char current;
int current_direction = RIGHT;
int crow = 0, ccol = 0;
int dim;
int string_mode = 0;

int (**functions)();

void print_grid(char *arr) {
  int i;
  for (i = 0; i < dim*dim; i++) {
    printf("%c", arr[i]);

    if (((i + 1) % dim) == 0)
      printf("\n");
  }
}

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
  free(list);
  list = new;
}

void move() {
  if (current_direction == RIGHT) {
    ccol++;
    ccol %= dim;
  }
  else if (current_direction == LEFT) {
    ccol--;
    ccol %= dim;
  }
  else if (current_direction == UP) {
    crow--;
    crow %= dim;
  }
  else if (current_direction == DOWN) {
    crow++;
    crow %= dim;
  }
}

int a, b, c;
int add_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  Stack_push(stk, a+b);
  move();
  return 0;
}

int sub_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  Stack_push(stk, b-a);
  move();
  return 0;
}
int mult_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  Stack_push(stk, a*b);
  move();
  return 0;
}
int div_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  Stack_push(stk, b/a);
  move();
  return 0;
}
int mod_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  Stack_push(stk, b%a);
  move();
  return 0;
}
int not_fn() {
  a = Stack_pop(stk);

  if (a == 0)
    Stack_push(stk, 1);
  else
    Stack_push(stk, 0);
  move();
  return 0;
}
int gt_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  if (b > a)
    Stack_push(stk, 1);
  else
    Stack_push(stk, 0);

  move();
  return 0;
}
int pcr_fn() {
  current_direction = RIGHT;
  move();
  return 0;
}
int pcl_fn() {
  current_direction = LEFT;
  move();
  return 0;
}
int pcu_fn() {
  current_direction = UP;
  move();
  return 0;
}
int pcd_fn() {
  current_direction = DOWN;
  move();
  return 0;
}
int pcrand_fn() {
  a = 0 - ((rand() % 4) + 96);
  current_direction = a;
  move();
  return 0;
}
int hif_fn() {
  a = Stack_pop(stk);

  if (a == 0)
    current_direction = RIGHT;
  else
    current_direction = LEFT;
  move();
  return 0;
}
int vif_fn() {
  a = Stack_pop(stk);

  if (a == 0)
    current_direction = DOWN;
  else
    current_direction = UP;
  move();
  return 0;
}
int dup_fn() {
  Stack_push(stk, Stack_peek(stk));
  move();
  return 0;
}
int swap_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);
  Stack_push(stk, a);
  Stack_push(stk, b);
  move();
  return 0;
}
int popr_fn() {
  Stack_pop(stk);
  move();
  return 0;
}
int popi_fn() {
  printf("%d ", Stack_pop(stk));
  move();
  return 0;
}
int popc_fn() {
  char ch = Stack_pop(stk);
  printf("%c ", ch);
  move();
  return 0;
}
int bridge_fn() {
  move();
  move();
  return 0;
}
int readi_fn() {
  scanf(" %d", &a);
  getchar();

  Stack_push(stk, a);

  move();
  return 0;
}
int readc_fn() {
  char ch;
  scanf(" %c", &ch);
  getchar();
  a = ch;
  Stack_push(stk, a);

  move();
  return 0;
}
int noop_fn() {
  move();
  return 0;
}
int end_fn() {
  return -1;
}
int num_fn() {
  Stack_push(stk, current-48);
  move();
  return 0;
}
int str_push_fn() {
  if (string_mode == 1) {
    a = current;
    Stack_push(stk, a);
    move();
  }
  return 0;
}
int string_fn() {
  string_mode ^= 1;
  move();
  return 0;
}
int get_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);

  if (a >= dim-1 || b >= dim-1)
    Stack_push(stk, 0);
  else
    Stack_push(stk, list[pos(a, b)]);

  move();
  return 0;
}
int put_fn() {
  a = Stack_pop(stk);
  b = Stack_pop(stk);
  c = Stack_pop(stk);

  while (a >= dim || b >= dim)
    double_list();

  list[pos(a,b)] = (char)c;
  move();
  return 0;
}
int ignore_fn() {
  return 0;
}

void init_functions() {
  int i;

  functions[0]  = (void *)(intptr_t)noop_fn;
  functions[1]  = (void *)(intptr_t)not_fn;
  functions[2]  = (void *)(intptr_t)string_fn;
  functions[3]  = (void *)(intptr_t)bridge_fn;
  functions[4]  = (void *)(intptr_t)popr_fn;
  functions[5]  = (void *)(intptr_t)mod_fn;
  functions[6]  = (void *)(intptr_t)readi_fn;
  functions[10] = (void *)(intptr_t)mult_fn;
  functions[11] = (void *)(intptr_t)add_fn;
  functions[12] = (void *)(intptr_t)popc_fn;
  functions[13] = (void *)(intptr_t)sub_fn;
  functions[14] = (void *)(intptr_t)popi_fn;
  functions[15] = (void *)(intptr_t)div_fn;

  for (i = 16; i <= 25; i++)
    functions[i] = (void *)(intptr_t)num_fn;

  functions[26] = (void *)(intptr_t)dup_fn;
  functions[27] = (void *)(intptr_t)ignore_fn;
  functions[28] = (void *)(intptr_t)pcl_fn;
  functions[30] = (void *)(intptr_t)pcr_fn;
  functions[31] = (void *)(intptr_t)pcrand_fn;
  functions[32] = (void *)(intptr_t)end_fn;

  for (i = 33; i <= 58; i++)
    functions[i] = (void *)(intptr_t)str_push_fn();
  
  functions[60] = (void *)(intptr_t)swap_fn;
  functions[62] = (void *)(intptr_t)pcu_fn;
  functions[63] = (void *)(intptr_t)hif_fn;
  functions[64] = (void *)(intptr_t)gt_fn;

  for (i = 65; i <= 90; i++)
    functions[i] = (void *)(intptr_t)str_push_fn();

  functions[71] = (void *)(intptr_t)get_fn;
  functions[80] = (void *)(intptr_t)put_fn;
  functions[86] = (void *)(intptr_t)pcd_fn;

  functions[92] = (void *)(intptr_t)vif_fn;
  functions[94] = (void *)(intptr_t)readc_fn;
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
  functions = malloc(sizeof(intptr_t)*100);
  init_functions();

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
  
  int val = 0;
  while (val == 0) {
    current = list[pos(crow, ccol)];
    while (string_mode == 1 && current != '"') {
      str_push_fn();
      current = list[pos(crow, ccol)];
    }
      
    val = functions[hash(current)]();
  }

  printf("\n");
  return 0;
}
