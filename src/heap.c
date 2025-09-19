#define HEAP_EXPOSE_INTERNAL

#include "heap.h"

#define HEAP_AT(heap, i) ((heap)->data + (i) * (heap)->stride)

Heap* heap_new(const size_t stride, Comparison compare, const enum Ordering order) {
	assert(order != EQ && "dale loco");
	Heap* heap = malloc(sizeof(Heap));
	assert(heap);
	heap->capacity = HEAP_MINIMUM_CAPACITY;
	heap->size = 0;
	heap->stride = stride;
	heap->compare = compare;
	heap->data = malloc(stride * heap->capacity);
	heap->order = order;
	return heap;
};

bool heap_is_empty(const Heap* heap) {
	if (!heap) return true;
	return heap->size == 0;
};

size_t heap_size(const Heap* heap) {
	if (!heap) return 0;
	return heap->size;
};

void heap_destroy(Heap* heap) {
	if (heap == NULL) return;
	if (heap->data) free(heap->data);
	free(heap);
};

void heap_realloc(Heap* heap, const size_t new_capacity) {
	assert(heap);
	assert(heap->data);

	uint8_t* new_data = realloc(heap->data, new_capacity * heap->stride);
	if (new_data == NULL) {
		heap_destroy(heap);
		exit(EXIT_FAILURE);
	};

	heap->data = new_data;
	heap->capacity = new_capacity;
};

uint8_t* heap_top(const Heap* heap) {
	assert(heap && "no heap");
	assert(heap->data && "heap has no data");
	assert(heap->size && "cannot access empty heap");
	return (uint8_t*)HEAP_AT(heap, 1);
};

void heap_insert(Heap* heap, uint8_t* data) {
	assert(heap && "no heap");
	if (heap->capacity < HEAP_MINIMUM_CAPACITY) {
		heap_realloc(heap, HEAP_MINIMUM_CAPACITY);
	};

	if (heap->size == 0) {
		memcpy(HEAP_AT(heap, 1), data, heap->stride);
		heap->size = 1;
		return;
	};

	while (heap->size + 1 > heap->capacity) {
		heap_realloc(heap, heap->capacity * 2);
	};

	heap->size++;
	memcpy(HEAP_AT(heap, heap->size), data, heap->stride);

	uint8_t* aux = heap->data;
	size_t index = heap->size;
	assert(aux);

	const enum Ordering disordered = (heap->order == GT)? LT : GT;
	while (index > 1) {
		size_t parent = index / 2;
		
		if (heap->compare(HEAP_AT(heap, parent), HEAP_AT(heap, index)) == disordered) {
			memcpy(aux, HEAP_AT(heap, parent), heap->stride);
			memcpy(HEAP_AT(heap, parent), HEAP_AT(heap, index), heap->stride);
			memcpy(HEAP_AT(heap, index), aux, heap->stride);
			index = parent;
		} else {
			break;
		}
	};
};

void heap_delete(Heap* heap, const size_t target_index) {
	assert(heap && "no heap");
	assert(target_index && target_index <= heap->size && "out of range delete");

	if (heap->size * 4 < heap->capacity && heap->size * 2 >= HEAP_MINIMUM_CAPACITY) {
		heap_realloc(heap, heap->capacity / 2);
	};

	if (target_index == heap->size) {
		heap->size--;
		return;
	};
	
	uint8_t* aux = heap->data;
	assert(aux);
	
	size_t index = target_index;
	memcpy(HEAP_AT(heap, index), HEAP_AT(heap, heap->size), heap->stride);
	heap->size--;

	const enum Ordering disordered = (heap->order == GT)? LT : GT;
	while (index <= heap->size) {
		const size_t left = 2 * index;
		const size_t right = 2 * index + 1;
		size_t next = left;

		if (left > heap->size) break;
		if (right <= heap->size) {
			const enum Ordering rel = heap->compare(HEAP_AT(heap, left), HEAP_AT(heap, right));
			next = (rel == heap->order)? left : right;
		};

		if (heap->compare(HEAP_AT(heap, index), HEAP_AT(heap, next)) == disordered) {
			memcpy(aux, HEAP_AT(heap, index), heap->stride);
			memcpy(HEAP_AT(heap, index), HEAP_AT(heap, next), heap->stride);
			memcpy(HEAP_AT(heap, next), aux, heap->stride);
		} else {
			break;
		};

		index = next;
	};
};

#ifndef HEAP_NO_MAIN_IMPL

enum Ordering compare_int(const uint8_t* lhs_, const uint8_t* rhs_) {
	const int lhs = *(int*)lhs_;
	const int rhs = *(int*)rhs_;
	if (lhs == rhs) return EQ;
	return (lhs < rhs)? LT : GT;
};

void heapsort(const size_t n, int* arr) {
	Heap* heap = heap_new(sizeof(int), compare_int, LT);
	for (size_t i = 0; i < n; i++) {
		heap_insert(heap, (void*)(arr + i));
	};

	for (size_t i = 0; i < n; i++) {
		arr[i] = *(int*)heap_top(heap);
		heap_delete(heap, 1);
	};

	heap_destroy(heap);
};

int main() {
	// aweseomeeee
	int the_data[128] = {
		979,241,747,1139,1346,418,1083,1629,910,272,826,503,1120,1346,1529,1367,1095,30,514,1931,1299,206,1853,357,513,482,571,182,1662,2019,1066,20,1706,322,742,590,1137,495,367,1176,1099,1033,1050,1376,872,199,1659,661,1594,616,813,782,806,175,1952,1609,918,717,230,1245,418,544,533,772,1599,1797,1617,837,423,1797,1001,1974,445,757,1373,1878,867,394,1033,927,216,1241,584,252,1039,1529,1671,293,1451,1388,1267,332,284,1137,471,654,878,519,1642,1739,949,90,619,837,1838,2032,1032,578,1436,1043,1789,251,1900,1684,1260,861,1992,1592,1621,823,435,116,330,1906,798,1015,1181,1936
	};

	printf("data: [");
	for (size_t i = 0; i < 128; i++) {
		if (i) printf(", ");
		printf("%d", the_data[i]);
	};
	printf("]\n");

	heapsort(128, the_data);

	printf("heapsort: [");
	for (size_t i = 0; i < 128; i++) {
		if (i) printf(", ");
		printf("%d", the_data[i]);
	};
	printf("]\n");

	return 0;
};

#endif //HEAP_NO_MAIN_IMPL
