#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  if (line)
    free(line);

  printf("%s", list);

  return 0;
}
