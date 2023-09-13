# Visualizer (of something)

This is mostly an experimenting project around SDL first, and webassembly then.

The data structure used is the heap, the object is to visualize logically [1].

Main objectives are: animate, interact.

[1] and maybe phisically, if there is enough space in the canvas

## Quick run

```bash
> make
...
> cd serve
> node index.js
```

**Requires**:

* emcc
* sdl2 and sdl_ttf2 dependencies

(why not meson or autotool? In the future, and maybe)

## Heap

see [here](README-heap.md)