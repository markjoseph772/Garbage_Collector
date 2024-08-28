#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h" 
#include <string.h>


 
Generational gen_collect ={0};



void Mark_and_sweep_mark(Object* object) {
	if(object == NULL || object->marked) {
		return;
	}
	object->marked = 1;  
	/*Marking logic if the object has reference to other objects */
}

void Mark_and_sweep_sweep(Heap* heap) {
	Object** object = &heap->objects;
	while (*object) {
		if(!(*object)->marked) {
			/*Object is unmarked so it can be freed */
			Object* unreached = *object;
			*object = unreached->next;
			free(unreached);
			heap->total_size -= sizeof(Object);
			
		} else {
			/* Unmark objects for  the next GC cycle */
			(*object)->marked = 0;
			(*object) = (*object)->next;
		}
	}
}
/* Mark phase for young generation */
void generational_mark(Object* object) {
	if(object && object->age == 0) {
		object->age = 1; /* Mark the object as live */
		int i;
		for( i = 0; i < object->reference_count; i++) {
			generational_mark(object->reference[i]);
		}
	}
}

/* sweep phase for young generation */
void sweep_young_gen() {
	int i;
	for(i = 0; i < gen_collect.young_gen_count; i++) {
		Object* recovery = gen_collect.young_gen[i];
		if(recovery->age == 0) {
			free(recovery); /*Reclaim memory */
			gen_collect.young_gen[i] = NULL ;
		} else {
			recovery->age = 0; /* Reset for the next cycle */
		}
	}
}

/* Collect young generation */
void collect_young_gen() {
	/* Mark phase */
	int i;
	for( i = 0; i < gen_collect.young_gen_count; i++) {
		generational_mark(gen_collect.young_gen[i]);
	}
	
	/* Sweep phase */
	sweep_young_gen();
	
	/* Move survived objects to old generation */
	
	for( i = 0; i < gen_collect.young_gen_count; i++) {
		Object* obj = gen_collect.young_gen[i];
		if(obj && obj->age == 1) {
			obj->age = 0; /* Reset for next GC cycle */
			if(gen_collect.old_gen_count < OLD_GEN_SIZE) {
				gen_collect.old_gen[gen_collect.old_gen_count++] = obj;
			} else {
				/* The old gen is full: implementation required to handle this scenario */
			}
		}
	}
	gen_collect.young_gen_count = 0 ;/* Clear young generation */
}
void add_to_young_gen(Object* object) {
	if(gen_collect.young_gen_count < YOUNG_GEN_SIZE) {
		gen_collect.young_gen[gen_collect.young_gen_count++] = object;
	} else {
		/*Young generation is full: perform collection*/
		collect_young_gen();
		add_to_young_gen(object);
	}
}

void Mark_and_sweep_collect(Heap* heap) {
	/* This is the mark phase. mark all reachable objects starting from the root set */
	Object* root = (Object*)malloc(sizeof(Object));
	for(root = heap->objects; root !=NULL; root = root->next){
		
		Mark_and_sweep_mark(root); /* simulated root set traversal */
	}
	
	/* Sweep phase : Sweep and free unmarked objects */
	Mark_and_sweep_sweep(heap);
}

Object* allocate(Heap* heap) {
	if(heap->total_size > heap->max_size) {
		/* trigger garbage collcetor when memory limit is exceeded */
		printf("Garbage collection triggered.\n");
		Mark_and_sweep_collect(heap);
	}
	
	
	/* allocate Memory for a new object */
	Object* object = (Object*)malloc(sizeof(Object));
	memset(object, 0, sizeof(Object));
	object->marked = 0;
	object->age = 0;   /* New objects are young */
	object->next = heap->objects;
	heap->objects = object;
	heap->total_size += sizeof(Object);
	return object;
}

Heap* create_heap(size_t max_size) {
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	heap->objects = NULL;
	heap->total_size = 0;
	heap->max_size = max_size;
	return heap;
}

void garbage_collect(GC* gc) {
	switch (gc->strategy) {
		case GC_MARK_SWEEP:
			Mark_and_sweep_collect (&gc->heap);
			break;
		
		case GC_GENERATIONAL:
		collect_young_gen();
		break;
	
	default:
	fprintf(stderr,"Unknown GC strategy\n");
	exit(EXIT_FAILURE);		
	}
}

 /*int main() {
	
	GC gc;
	gc.strategy =GC_GENERATIONAL; /*OR GC_MARK_SWEEP */
/*	garbage_collect (&gc);
	/*
	 Create a heap with a maxsize limit for trigerring GC 
	Heap* heap = create_heap(1024);
	
	Allocate objects and manage memory 
	Object* obj1 = allocate(heap);
	Object* obj2 = allocate(heap);
	Object* obj3 = allocate(heap);
	
	simulate root objects to prevent them from being collected 
	mark(obj1);
	mark(obj2);
	
	/*Trigger garbage collection manually 
	collect_garbage (heap);
	
	/*Allocate more objects to demonstrateautomatic GC 
	Object* obj4 = allocate(heap);
	
	/* clean up 
	collect_garbage(heap);
	free(heap);*/
/*	return 0; 
	
} */
