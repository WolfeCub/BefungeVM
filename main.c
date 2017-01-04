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

intptr_t (**functions)();

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
    if (ccol >= dim)
      ccol = 0;
  }
  else if (current_direction == LEFT) {
    ccol--;
    if (ccol < 0)
      ccol = dim-1;
  }
  else if (current_direction == UP) {
    crow--;
    if (crow < 0)
      crow = dim-1;
  }
  else if (current_direction == DOWN) {
    crow++;
    if (crow >= dim)
      crow = 0;
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
  a = current;
  Stack_push(stk, a);
  move();
  return 0;
}
int string_fn() {
  move();
  while ((current = list[pos(crow, ccol)]) != '"') {
    str_push_fn();
  }
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
  move();
  while ((current = list[pos(crow, ccol)]) != ';') {
    move();
  }
  move();
  return 0;
}

int letter_num_fn() {
  a = current;
  Stack_push(stk, a - 87);
  move();
  return 0;
}

void init_functions() {
  int i;

  functions[0]  = (void *)noop_fn;
  functions[1]  = (void *)not_fn;
  functions[2]  = (void *)string_fn;
  functions[3]  = (void *)bridge_fn;
  functions[4]  = (void *)popr_fn;
  functions[5]  = (void *)mod_fn;
  functions[6]  = (void *)readi_fn;
  functions[10] = (void *)mult_fn;
  functions[11] = (void *)add_fn;
  functions[12] = (void *)popc_fn;
  functions[13] = (void *)sub_fn;
  functions[14] = (void *)popi_fn;
  functions[15] = (void *)div_fn;

  for (i = 16; i <= 25; i++) {
    functions[i] = (void *)num_fn;
  }

  functions[26] = (void *)dup_fn;
  functions[27] = (void *)ignore_fn;
  functions[28] = (void *)pcl_fn;
  functions[30] = (void *)pcr_fn;
  functions[31] = (void *)pcrand_fn;
  functions[32] = (void *)end_fn;

  // Uppercase Letters
  functions[33] = (void *)str_push_fn;
  functions[34] = (void *)str_push_fn;
  functions[35] = (void *)str_push_fn;
  functions[36] = (void *)str_push_fn;
  functions[37] = (void *)str_push_fn;
  functions[38] = (void *)str_push_fn;
  functions[39] = (void *)str_push_fn;
  functions[40] = (void *)str_push_fn;
  functions[41] = (void *)str_push_fn;
  functions[42] = (void *)str_push_fn;
  functions[43] = (void *)str_push_fn;
  functions[44] = (void *)str_push_fn;
  functions[45] = (void *)str_push_fn;
  functions[46] = (void *)str_push_fn;
  functions[47] = (void *)str_push_fn;
  functions[48] = (void *)str_push_fn;
  functions[49] = (void *)str_push_fn;
  functions[50] = (void *)str_push_fn;
  functions[51] = (void *)str_push_fn;
  functions[52] = (void *)str_push_fn;
  functions[53] = (void *)str_push_fn;
  functions[54] = (void *)str_push_fn;
  functions[55] = (void *)str_push_fn;
  functions[56] = (void *)str_push_fn;
  functions[57] = (void *)str_push_fn;
  functions[58] = (void *)str_push_fn;
  
  functions[60] = (void *)swap_fn;
  functions[62] = (void *)pcu_fn;
  functions[63] = (void *)hif_fn;
  functions[64] = (void *)gt_fn;

  // Lowercase a-f
  functions[65] = (void *)letter_num_fn;
  functions[66] = (void *)letter_num_fn;
  functions[67] = (void *)letter_num_fn;
  functions[68] = (void *)letter_num_fn;
  functions[69] = (void *)letter_num_fn;
  functions[70] = (void *)letter_num_fn;

  // Lowercase h onwards
  //functions[72] = (void *)str_push_fn;
  //functions[73] = (void *)str_push_fn;
  //functions[74] = (void *)str_push_fn;
  //functions[75] = (void *)str_push_fn;
  //functions[76] = (void *)str_push_fn;
  //functions[77] = (void *)str_push_fn;
  //functions[78] = (void *)str_push_fn;
  //functions[79] = (void *)str_push_fn;
  //functions[80] = (void *)str_push_fn;
  //functions[81] = (void *)str_push_fn;
  //functions[82] = (void *)str_push_fn;
  //functions[83] = (void *)str_push_fn;
  //functions[84] = (void *)str_push_fn;
  //functions[85] = (void *)str_push_fn;
  //functions[87] = (void *)str_push_fn;
  //functions[88] = (void *)str_push_fn;
  //functions[89] = (void *)str_push_fn;
  //functions[90] = (void *)str_push_fn;

  functions[71] = (void *)get_fn;
  functions[80] = (void *)put_fn;
  functions[86] = (void *)pcd_fn;

  functions[92] = (void *)vif_fn;
  functions[94] = (void *)readc_fn;
}

int main(int argc, char **argv) {
  if (argc < 2)
    fprintf(stderr, "Not enough arguments");

  int i, val;
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

  i = 0;
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
  
  val = 0;
  while (val == 0) {
    current = list[pos(crow, ccol)];
    //Stack_print(stk);
    //printf("%c  ", current);
    val = functions[hash(current)]();
  }

  printf("\n");
  return 0;
}
