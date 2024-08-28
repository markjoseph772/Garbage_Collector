 /* This is the header file containing data structures that are used in the program. They inlude:
 An object Structure where the user data is stored
 A heap Structure  to manage the objects allocated by the user */
 #include<stdlib.h>
 #define YOUNG_GEN_SIZE 1024
#define OLD_GEN_SIZE 2048
#define OBJECT_SIZE 64
 
 typedef struct Object {
 	
 	/* User data is placed here */
 	void *data;
 	size_t size;
 	
 	int age; /* This tracks the age of objects */
 	int reference_count;
 	int marked;  /*This is the mark flag for the GC */
 	struct Object* next;  /* Pointer to the next node on the linked list */
	struct Object* reference[10]; /*Example reference */   
 }Object;
 
 typedef struct Heap {
 	
 	/* For the linked list of all objects in the heap */
 	Object* objects;
 	size_t total_size;  /*Current size of the allocated memory */
 	size_t max_size;   /*This is the trigger threshhold for thr GC */
 } Heap;
 
 typedef struct Generational {
 	Object* young_gen[YOUNG_GEN_SIZE];
 	Object* old_gen[OLD_GEN_SIZE];
 	int young_gen_count;
 	int old_gen_count;
 }Generational;
 
 
 typedef enum {
	GC_MARK_SWEEP, GC_GENERATIONAL,
	
} GCStrategy;

typedef struct GC {
	GCStrategy strategy;
	Heap heap;
	/* Add other necessary fields for each strategy */
}GC;
