#include <stdlib.h>
typedef int (*__compar_fn_t)(const void *, const void *);
int biggerthan(const void *a, const void *b);
int smallerthan(const void *a, const void *b);

typedef struct {
  // compare(a, b) must return less than, equal, greater than if a is lt, eq, gt
  // b, respectively. Then the order is ascending: thus, it is a min-heap.
  // The order defines preceeding and subsequent element: first preceeds second
  // and third, and so on. The property of the heap is: for each node that
  // has children the parent preceeds its children.
  __compar_fn_t compare;
  // here are the datas as void *
  void **data;
  int size;
  int capacity;
} Heap;

Heap *heap_create(int (*compare)(const void *, const void *), int capacity);
void heap_insert(Heap *h, void *data);
int heap_find_elem_pos2(Heap *h, void *data);
int heap_find_elem_pos3(Heap *h, void *data);
void heap_free(Heap **hr);
void heap_print(Heap *h, char *(*format)(const void *));
char *formatint(const void *data);
