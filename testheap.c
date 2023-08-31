#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int main() {
  Heap *h = heap_create(biggerthan, 100);
  int *e = (int *)malloc(sizeof(int));
  *e = 190;
  heap_insert(h, (void *)e);
  int a = 9, b = 10, c = 45;  //, d=22, e=11, f=5, g=7, h=1;
  heap_insert(h, &a);
  heap_print(h, formatint);
  heap_insert(h, &b);
  heap_print(h, formatint);
  heap_insert(h, &c);
  heap_print(h, formatint);
  int B[] = {11, 22, 33, 44, 55, 66, 77, 64};
  for (int i = 0; i < 8; i++) {
    heap_insert(h, &B[i]);
  }
  printf("prova heap\n");
  heap_print(h, formatint);

  int pose = heap_find_elem_pos3(h, &a);
  printf("position of %d is %d\n", a, pose);
  int pose2 = heap_find_elem_pos2(h, &a);
  printf("2position of %d is %d\n", a, pose2);
  heap_free(&h);
  return 0;
}
