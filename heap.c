// #include "heap.h"
typedef int (*__compar_fn_t)(const void *, const void *);
#include <stdio.h>
#include <stdlib.h>
// this provide an heap with satellite information of void pointer.

// #define PRINT_FULLNESS

int biggerthan(const void *a, const void *b) {
  int a1 = *(int *)a;
  int b1 = *(int *)b;
  if (a1 > b1) return -1;
  if (a1 < b1) return 1;
  return 0;
}

int smallerthan(const void *a, const void *b) {
  int a1 = *(int *)a;
  int b1 = *(int *)b;
  if (a1 < b1) return -1;
  if (a1 > b1) return 1;
  return 0;
}

// The supporting data structure should have as key the same key (to extract
// the min) and the index position as satellite information, to return the pos
typedef struct {
  void *data;
  int pos;
} augmented;

augmented *augm_create(void *data, int pos) {
  augmented *a = (augmented *)malloc(sizeof(augmented));
  *a = (augmented){.data = data, .pos = pos};
  return a;
}

int smallerthan_aug(const void *a, const void *b) {
  // this compare the augmented struct
  int a1 = *(int *)(((augmented *)a)->data);
  int b1 = *(int *)(((augmented *)b)->data);
  if (a1 < b1) return -1;
  if (a1 > b1) return 1;
  return 0;
}

// this is used for storing indexes.
// compare function is always the same: min preceeds a greater value.
// Another way to state it: it is a Min-Heap
typedef struct {
  // int (*cmp)(void *, void *);
  int *data;
  int size;
  int capacity;
} MinHeapInt;

MinHeapInt *minheapint_create(int capacity) {
  MinHeapInt *h = (MinHeapInt *)malloc(sizeof(MinHeapInt));
  *h = (MinHeapInt){.capacity = capacity,
                    .data = (int *)calloc(sizeof(int *), capacity),
                    .size = 0};
  return h;
}

void minheapint_free(MinHeapInt **hp) {
  MinHeapInt *h = *hp;
  free(h->data);
  free(h);
  *hp = NULL;
}

void minheapint_heapify(MinHeapInt *h, int i) {
  // comment
  int first = i;
  int l = (i << 1) | 1;  // i * 2 + 1
  int r = (i << 1) + 2;
  if (l < h->size && h->data[l] < h->data[i]) {
    // comemnt
    first = l;
  }
  if (r < h->size && h->data[r] < h->data[first]) {
    first = r;
  }
  if (first != i) {
    int a = h->data[first];
    h->data[first] = h->data[i];
    h->data[i] = a;
    minheapint_heapify(h, first);
  }
}

void minheapint_insert(MinHeapInt *h, int e) {
  h->data[h->size] = e;
  h->size++;

  if (h->size > 1) {
    for (int i = h->size / 2 - 1; i >= 0; i--) minheapint_heapify(h, i);
  }
}

int minheapint_extract(MinHeapInt *h) {
  //
  if (h->size > 0) {
    int res = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    minheapint_heapify(h, 0);
    return res;
  }
  return -1;
}

static int block_size = 512;

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

void heap_print(Heap *h, char *(*format)(const void *)) {
  // print infos
  printf("HEAP size: %d ; capacity: %d\n", h->size, h->capacity);
  for (int i = 0; i < h->size; i++) {
    char *f = format(h->data[i]);
    printf("[%d]: %s\n", i, f);
    free(f);
  }
}

// this is used to reppresent element and its position in the heap
// for convenience to search the next
typedef struct {
  int pos;
  void *data;
} ElemPos;

Heap *heap_create(int (*compare)(const void *, const void *), int capacity) {
  Heap *h = malloc(sizeof(Heap));
  *h = (Heap){
      .compare = compare,
      .capacity = capacity,
      .data = calloc(sizeof(void *), capacity),
      .size = 0,
  };
  return h;
}

void heap_free(Heap **hr) {
  // no pici
  Heap *h = *hr;
  free(h->data);
  free(h);
  *hr = NULL;
}

void heap_heapify(Heap *h, int i) {
  // comment
  int first = i;
  int l = (i << 1) | 1;  // i * 2 + 1
  int r = (i << 1) + 2;
  if (l < h->size && h->compare(h->data[l], h->data[i]) < 0) {
    // comemnt
    first = l;
  }
  if (r < h->size && h->compare(h->data[r], h->data[first]) < 0) {
    first = r;
  }
  if (first != i) {
    void *a = h->data[first];
    h->data[first] = h->data[i];
    h->data[i] = a;
    heap_heapify(h, first);
  }
}

void heap_insert(Heap *h, void *data) {
  // do something sensible in O(log n)
  h->data[h->size] = data;
  h->size++;

  if (h->size > 1) {
    for (int i = h->size / 2 - 1; i >= 0; i--) heap_heapify(h, i);
  }
  if (h->capacity < h->size + 10) {
    // che hai?
    h->capacity += block_size;
    h->data = realloc(h->data, sizeof(void *) * h->capacity);
  }
}

void *heap_get_next(Heap *h, void *data) {
  // do something sensible in O(log n)
  printf("N: %d data %p", h->size, data);
  return NULL;
}

void *heap_get_prev(Heap *h, void *data) {
  // do something sensible in O(log n)
  printf("N: %d data %p", h->size, data);
  return NULL;
}

void *heap_rm_elem(Heap *h, void *data) {
  // do something sensible in O(log n)
  printf("N: %d data %p", h->size, data);

  return NULL;
}

void *heap_extract(Heap *h) {
  // do something sensible in O(1)
  // Not possible: O(log n)
  if (h->size > 0) {
    void *res = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    heap_heapify(h, 0);
    return res;
  }
  return NULL;
}

int heap_find_elem_pos(Heap *h, void *data) {
  // assuming min level equal 1
  int start = 0;
  int l = 1;
  for (int span = 1; span <= h->size; span *= 2) {
    // or it can be multiplied by 2 ...
    int all_after = 1;
    // after in the order defined by compare
    for (int i = start; i < l; i++) {
      //
      int cmp = h->compare(h->data[i], data);
      if (cmp == 0) {
        return i;
      }
      if (cmp < 0) {
        all_after = 0;
        // a block to include: [i*2, i*2+1] and descendants
        // it can be used an heap to store those indexes ...
      }
    }
    // when are all_after, and the element was not found in preceeding elements,
    // then it does not exists
    if (all_after) {
      return -1;
    }
    start = span;
    l++;
  }
  return -1;
}

int heap_find_elem_pos3(Heap *h, void *data) {
  // The supporting data structure should have as key the same key (to extract
  // the min) and the index position as satellite information, to return the pos
  Heap *h1 = heap_create(smallerthan_aug, h->size);
  augmented *a = augm_create(h->data[0], 0);
  heap_insert(h1, a);
  int step = 0;
  while (h1->size > 0) {
    step++;
    augmented *tocmp = heap_extract(h1);
    int idx = tocmp->pos;
    int cmp = h->compare(h->data[idx], data);
#ifdef PRINT_FULLNESS
    printf("at pos %d .. el: %d cmp: %d Fullness: %d\n", idx,
           *(int *)h->data[idx], cmp, h1->size);
#endif
    if (cmp == 0) {
      heap_free(&h1);
      printf("STEPS: %d\n", step);
      return idx;
    }
    if (cmp < 0) {
      // insert in h1 the children
      int left_i = (idx)*2 + 1;
      int right_i = (idx)*2 + 2;
      if (left_i < h->size && h->compare(h->data[left_i], data) <= 0) {
#ifdef PRINT_FULLNESS
        printf("inserting %d\n", left_i);
#endif
        augmented *a = augm_create(h->data[left_i], left_i);
        heap_insert(h1, a);
      }
      if (right_i < h->size && h->compare(h->data[right_i], data) <= 0) {
#ifdef PRINT_FULLNESS
        printf("inserting %d\n", right_i);
#endif
        augmented *a = augm_create(h->data[right_i], right_i);
        heap_insert(h1, a);
      }
    }
  }
  heap_free(&h1);
  return -1;
}

int heap_find_elem_pos2(Heap *h, void *data) {
  // The supporting data structure should have as key the same key (to extract
  // the min) and the index position as satellite information, to return the pos
  MinHeapInt *h1 = minheapint_create(h->size);
  minheapint_insert(h1, 0);
  int step = 0;
  while (h1->size > 0) {
    step++;
    int idx = minheapint_extract(h1);
    int cmp = h->compare(h->data[idx], data);
#ifdef PRINT_FULLNESS
    printf("at pos %d .. el: %d cmp: %d Fullness: %d\n", idx,
           *(int *)h->data[idx], cmp, h1->size);
#endif
    if (cmp == 0) {
      minheapint_free(&h1);
      printf("STEPS: %d\n", step);
      return idx;
    }
    if (cmp < 0) {
      // insert in h1 the children
      int left_i = (idx)*2 + 1;
      int right_i = (idx)*2 + 2;
      if (left_i < h->size && h->compare(h->data[left_i], data) <= 0) {
#ifdef PRINT_FULLNESS
        printf("inserting %d\n", left_i);
#endif
        minheapint_insert(h1, left_i);
      }
      if (right_i < h->size && h->compare(h->data[right_i], data) <= 0) {
#ifdef PRINT_FULLNESS
        printf("inserting %d\n", right_i);
#endif
        minheapint_insert(h1, right_i);
      }
    }
  }
  minheapint_free(&h1);
  return -1;
}

inline void *heap_at_pos(Heap *h, int pos) {
  // return element at pos
  return h->data[pos];
}

char *formatint(const void *data) {
  // data
  int a1 = *(int *)data;
  char *out = (char *)malloc(12 * sizeof(char));
  sprintf(out, "%d", a1);
  return out;
}

// benchmarking
// How long time is needed to create an heap of 100k elements? (random)
// How long is needed to create an arra of 100k, the order it?
// Hint: stable input, so generate a file and read it

int main_no() {
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