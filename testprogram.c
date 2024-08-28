#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

extern Object* allocate(Heap* heap);
extern void garbage_collect(GC* gc);
extern Heap* create_heap(size_t max_size);
int main() {
	
	GC gc;
	gc.strategy = GC_GENERATIONAL;
	Heap* heap = create_heap(1000);
	
	int *arr1 = (int*)allocate(heap);
	int *arr2 = (int*)allocate(heap);
	int i;
	for(i=0; i<10; i++) {
		
		arr1[i] = i;
	}
	for(i=0; i < 20; i++){
		
		arr2[i] = i * 2;
	}
	/* simulate a memory leak by forgetting arr2 */
	arr2 = NULL;
	
	garbage_collect(&gc);
	
	int *arr3 = (int*)allocate(heap);
	for(i = 0; i < 15; i++){
		
		arr3[i] = i * 3;
	}
	
	printf("arr1[0]: %d, arr[9]: %d\n", arr1[0], arr1[9]);
	printf("arr3[0]: %d, arr[14]: %d\n", arr3[0], arr3[14]);
	
	garbage_collect(&gc);
	return 0;
	
}
