# The heap structure

Given an `order`, the heap structure is a tree with the property:

> The parent node preceeds its children

An `order` is a function from the cartesian product of the domain (D x D)
to the order values (B,S,A: Before, Same, After):

> order: D x D -> O

In the code Before == -1, Same == 0, After == 1

## Heap Operations and their cost

**Insert**: O(log n)

**FindPos**: O(n)

**RemoveFirst**: O(log n)

**RemovePos**: O(log n)

### Find next-of

Find the next. Use DSX (Data Structure X, DSX(top) is the biggest in DSX, DSX(top) costs O(1))

1. FindPos(V) in O(log n), and its level L.
2. For l=L; l>0; l-- do
   2.0. replaced = 0; keep_asc=0;
   2.1. for each n in NodeofLevel(l) do
       2.1.1. if (n>V && DSX(top)>n)? { DSX_replaceTop(n); replaced=1}
       2.1.2. if (n<V)? keep_asc=1;
   (in DSX is the smallest node bigger than V found so far, at level l, so,
    at level l-1 all nodes are bigger than their respective children,
    but not necessary bigger than some of the node visited in the level l (which is not a direct child).
    But if no node X is smaller than V, there is no parent P that is V < P < X, because P > X, and I am
    searching for P that is the first successor of V, so it is smaller than any X visited so far)
   2.2. (replaced ==0 && keep_asc==0)? break;
3. in DSX(top) is the smallest node bigger than V found so far, and there is nothing smaller in H that is bigger than V

And when the Next-of is at level>L? It must be looped for each level l>L also

1. For (l=L+1; l< log n; l++) do
  1.0. keep_desc = 0;
  1.1. for each n in NodeofLevel(l) do
       2.1.1. if (n>V && DSX(top)>n)? { DSX_replaceTop(n); replaced=1}
       2.1.2. if (n>V)? keep_desc=1;

...Find next is too much complex to be supported by the heap data structure in a easy (and efficient) way.

### Remove first

Everything follow the first, which is at level 0.

At level 1 there are 2 elements, say A and B, A << B or B << A?

In case A << B then each child of A can either << B or >> B, but every choise of them as
sibling of B maintain the heap property `The parent node preceeds its children`

Which of the l*2 elements children is the best candidate to be promoted as parent? the first.

This procedure is `heapify`.

The fact that `extract` take the last element and put it to the first position, does not influence
the complexity. In fact `the last` can be seen as a placeholder for which it is true that every
node can before it: a place that must be occupied by the best candidate.

## Tests and Benchmarks

https://www.programiz.com/dsa/heap-data-structure

https://www.geeksforgeeks.org/cache-oblivious-algorithm/


## Visualizer

The objective of visual reppresentation is to show the operations made during heap insertion, search, deletion, and so on.

So, every element should be reppresentable by a `Box`, every `Box` should have a `Text caption` inside, and maybe including
some graphical sign or no-a-letter glyph to separate infos.

Each Box should be linked with other (say parents and/or children) by an `Arc`. An `Arc` is a line has a sourceBox and
a targetBox. It should be possible to draw the `Arc` satisfying the requirement of "not passing through the Box.

To warrant this property, the drawing system must know where is the `Box` and where is the `Arc`, relative to each other.

For example:

* is the `Arc` _above_ the `Box`? then the line of the `Arc` is attached to the _top_ line of the `Box`
* is the `Arc` _below_ the `Box`? then the line of the `Arc` is attached to the _bottom_ line of the `Box`

Is there any other option? Nope. To estabilish the _above_ property of 2 graphical elements it can be used Geometrical computing
technics, or this could be defined by the domain itself. Let's say that an `Arc` is identified by its link from the parent Box,
to the children Box, and let's say that the visualization of the tree goes from top to the bottom, parent to children.

So, easily the `Arc` is attached to the bottom of the parent Box (source), and to the top of the child Box (target).

Drawing an `Arc` translate to:

* find the middle of the bottom of the Box of the origin
* find the middle of the top of the Box of the target
* draw a linking line

Who is responsible to provide "the middle of..."? Answer: The Box.

```C
typedef struct {
        int x;
        int y;
} Point;

typedef struct {
   Point top_left;
   Point bot_right;
} NodeBox;

typedef struct {
  NodeBox *source;
  NodeBox *target;
} Arc;

Point box_get_source_lnk(Box *b) {}

Point box_get_target_lnk(Box *b) {}

```

And that's all about the tree.

### The associated array

The heap is in fact stored into a regular array. This array should be reppresented near the tree.

The array is arranged vertically and is made of boxes containing the same infos as the box in the tree.

### Animation

It should be possible to visualize animation during operation.

A swap animation is made of:

* two element `blinking` to focus the attention to the action.
* an element "moving around" while it is swapped (start swap)
* an element "leaving the space" for the swapped element
* an element "occuping the new made space"
* an element "filling up the space left by another" (end swap)

An insertion animation is made of:

* the new element appearing fading in and out 2 times and blinking (insertion is done at the bottom of the tree)
* the associated Arc behave the same, in concert

A lookup is made of a traverse of the tree, so the traverse is made of:

* blinking elements in a level
* at a given time select the bounding elements (clear up the algos)

A deletion is made of:

* lookup animation
* blinking box of the element found
* remove of the element
* blinking of the two candidates for the position
* selected candidate moving to fill the vacant position

### Data structure <-> animation callback

This is done by adding a pointer to a function that update the animation.

Inside the heap data structure "method" (it's C, but ok) that function can be called with parameters:

* char *cmd, index index, pos, ...

TBD

### Arranging a tree structure

To arrange the tree structure that reppresente the heap there are info that can be used:

1. number of nodes
2. levels of the tree
3. nr of elements for each level of the tree.

Nr of nodes (1.) is required, while 2. and 3. can be derived.

The procedure `arrange_tree(data, n)` has a reference to data, it does not require methods from the heap:

1. calc the height (nr of levels) by counting required number of n shift to right to reach 0 (it is log n + 1)
2. calc the weight by multiply 2 * levels (calculated in 1.)
3. repeat for each level: arrange_level(i);

arrange_level(i, center):

0. far_from_center = (weight - 2^l) / 2 ;
1. for (x=i*2+1; x<(i+1)*2; x++) do
2. _  draw_box(i)
3. _  if (i>0) arc_to_upper(x, x/2)



https://wiki.libsdl.org/SDL2/Tutorials
https://glusoft.com/sdl2-tutorials/


https://www.jamesfmackenzie.com/2019/12/01/webassembly-graphics-with-sdl/