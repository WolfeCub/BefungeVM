#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ptr_to_xy(char *list, int row, int col) {
  char *ptr = list;
  int i;
  for (i = 0; i < row; i++)
    ptr = strchr(ptr, '\n');

  return (ptr + col);
}

char* process_char(char *list, char *current_pos) {
  printf("%c\n", *current_pos);
  return current_pos;
}

int main(int argc, char **argv) {
  if (argc < 2)
    fprintf(stderr, "Not enough arguments");

  FILE *fd;
  int size;
  char *list, *itr;

  if ((fd = fopen(argv[1], "r")) == NULL) {
    perror(argv[1]);
    return 1;
  }

  fseek(fd, 0L, SEEK_END);
  size = ftell(fd);
  list = malloc(sizeof(char) * size);
  rewind(fd);

  itr = list;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fd)) != -1) {
    strncpy(itr, line, read);
    itr += read;
  }

  fclose(fd);
  free(line);

  printf("%s\n", list);

  char *current_pos = ptr_to_xy(list, 0, 0);

  while (1) {
    current_pos = process_char(list, current_pos);
  }

  return 0;
}
