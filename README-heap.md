# The heap structure

Given an `order`, the heap structure is a tree with the property:

> The parent node preceeds its children

An `order` is a function from the cartesian product of the domain (D x D)
to the order values (B,S,A: Before, Same, After):

> order: D x D -> O

In the code Before == -1, Same == 0, After == 1

## Heap Operations and their cost

**Insert**: O(log n)

**FindPos**: O(log n)

**RemoveFirst**: O(log n)

**RemovePos**: O(log n)

## Find next-of

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

1. For (l=L+1; l>0; l--) do
  1.1. for each n in NodeofLevel(l) do
       2.1.1. if (n>V && DSX(top)>n)? { DSX_replaceTop(n); replaced=1}
       2.1.2. if (n<V)? keep_asc=1;

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


https://wiki.libsdl.org/SDL2/Tutorials
https://glusoft.com/sdl2-tutorials/


