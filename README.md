# ding-dsa

A generic data structures & algorithms library written from scratch in C.

`ding-dsa` implements common data structures using a lightweight runtime type system instead of macros or `void*` hacks scattered through user code. You describe a type once with a `type_info` struct, and every container — vector, linked list, queue, pair, and (in progress) hash map — knows how to store, copy, and print it.

## Why this exists

This started as a personal tool for LeetCode practice and future projects, but it turned into a deeper exercise in:

- **Memory management** — manual allocation, resizing, and freeing with raw `malloc`/`realloc`/`memcpy`, no garbage collector to fall back on.
- **Generic programming in C** — building type-safe-ish containers without C++ templates or `_Generic`, using a `type_info` descriptor (name, size, print function) passed alongside the data.
- **Systems-level tradeoffs** — thinking through amortized growth (doubling capacity on push), pointer arithmetic for indexing, and the cost of type mismatches caught only at runtime.

## Features

- **`type_info`** — a small runtime type descriptor (`type_name`, `type_size`, `print`) with built-in support for all standard C primitive types (`bool`, `char`, `int`, `float`, `size_t`, etc.)
- **`vector`** — dynamic array with push/pop, indexed access, search (first/last/all occurrences), concatenation, reserve, and shrink-to-fit
- **`linked_list`** — doubly linked list with sentinel head/tail nodes for O(1) push/pop at both ends
- **`queue`** — FIFO built on top of the linked list implementation
- **`pair`** — a generic two-value container for heterogeneous key/value-style data
- **`unordered_map`** *(in progress)* — bucket-based hash map built on the linked list backend

## Example

```c
#include "dsupera.h"

int main(void) {
    vector *v = init_vector_ret(&TI_INT);

    int a = 10, b = 20, c = 30;
    vec_push_back(v, &a, &TI_INT);
    vec_push_back(v, &b, &TI_INT);
    vec_push_back(v, &c, &TI_INT);

    vec_info_log(v); // prints size, capacity, and each element

    vec_free(v);
    return 0;
}
```

Type mismatches between a container and the data you try to insert are caught at runtime via the `type_info` pointer comparison, rather than failing silently.

## Usage

Sample usage lives in `main.c` — feel free to overwrite it for your own testing.

## Roadmap

- [ ] Finish the `unordered_map` implementation (hashing + bucket collision handling)
- [ ] Convert to a single-header library for easier drop-in use
- [ ] Add more linked list operations (insert-at, reverse, etc.)
- [ ] Expand test coverage across container types

## Status

Actively developed as a personal systems-programming project. APIs may change as the library evolves toward a single-header release.
