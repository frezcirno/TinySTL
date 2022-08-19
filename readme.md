# Tiny STL

This repo is a personal, cutted implementation of the Standard Template Library(STL), which was once part of my data structure course project and is now distributed separately.

## CONTENT

### Infrastructures

- allocator

    - The memory allocator used by the STL data structure.

- type_traits

    - A number of c++ templates to get features of the type as well as manipulate the type.

- construct

    - The object constructor and deconstructors.

- uninitialized

    - Universal object memory manipulation utils.

- rbtree

    - The efficient but tricky red-black-tree.

    - It's the underlying data structure of `map` and `set`.

- heap

    - A simple wrapper on the container, making it a heap.

    - It's the underlying data structure of `priority_queue`.

- pair

    - Combine any two objects together and create a new combination type.

    - The iterator of `map` and `set` use this.

### Data Structures

- array

    - Just an array, nothing special.

- vector

    - Dynamic array, auto-resizing, contiguous storage.

    - **Random access -> O(1)**

- list

    - Double link list, auto-resizing.

    - **Insert/Delete at any position -> O(1)**

- deque

    - Double-end queue, with the two ends supporting push/pop operations.

    - **Push/pop at queue front -> O(1)** (Compared to `vector`s `O(n)`)

    - The coefficient of time complexity is larger than vector, because it does much more operations to maintain the queue.

- queue

    - FIFO queue, push to the end, pop on the front.

    - Use `deque` as container by default

- stack

    - LIFO container, push/pop to the top.

    - Use `deque` as container by default

- priority_queue

    - Just like a heap, push into the heap, pop the element with the highest priority.

- set

    - Just like the `set` in Mathematics. The key of any two elements is not allowed to be equal.

    - Access, insert, delete by key -> `O(log n)`

- map

    - Just like the `set`, but each element comes with a `value`.

- algorithm

    - Some container-independent algorithms

- functional

    - function object

### My extensions

These don't exists in the real STL, but I'd like to implement them.

- `BigIngeter`

    - Support very large number calculations

    - Use FFT algorithm to do large integer multiplications

- `disj_set`

    - disjoint set

### Unimplemented

- const iterator

- reverse iterator

- Any features exist in the real STL, but no implementation here.

## REFERENCES

- 《STL源码解析》——侯捷
- GNU ISO C++ Library 9.2.0