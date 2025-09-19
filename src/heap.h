#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef HEAP_MINIMUM_CAPACITY
#define HEAP_MINIMUM_CAPACITY 8
#endif //HEAP_MINIMUM_CAPACITY

typedef enum Ordering: uint8_t {
	LT = -1,
	EQ = 0,
	GT = 1
} Ordering;

typedef enum Ordering(*Comparison)(const uint8_t* lhs, const uint8_t* rhs);

#ifndef HEAP_EXPOSE_INTERNAL
typedef struct Heap Heap;
#else
typedef struct Heap {
	size_t capacity;
	size_t size;
	size_t stride;
	Comparison compare;
	enum Ordering order;
	uint8_t* data;
} Heap;
#endif //HEAP_EXPOSE_INTERNAL

Heap* heap_new(const size_t stride, Comparison compare, const enum Ordering order);
bool heap_is_empty(const Heap* heap);
size_t heap_size(const Heap* heap);
void heap_destroy(Heap* heap);
void heap_realloc(Heap* heap, const size_t new_capacity);
uint8_t* heap_top(const Heap* heap);
void heap_insert(Heap* heap, uint8_t* data);
void heap_delete(Heap* heap, const size_t index);

#ifdef HEAP_IMPLEMENTATION
#define HEAP_NO_MAIN_IMPL
#include "heap.c"
#endif //HEAP_IMPLEMENTATION
