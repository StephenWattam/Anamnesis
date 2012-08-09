/* 
   Anamnesis is a simple form of malloc-agnostic memory pooler.

   It provides a linked list of all heap variables, giving it the 
   ability to reallocate, free and move variables arbitrarily 
   without danger of memory leaks.

   Thank Valgrind for testing :-)

*/


typedef char byte;

typedef struct safe_var{
	struct safe_var * next;
	byte checkbyte;
	void * location;
	unsigned long int size;
} Svar;

typedef struct safe_var_list{
	Svar *front, *back;
} SvarList;

int anam_init(SvarList * q);
void anam_destroy_var(Svar * var);
void anam_destroy(SvarList * q);
int anam_verify(Svar * var);
Svar * anam_alloc(unsigned int length);
void anam_clear(Svar * var);
Svar * anam_alloc_clear(unsigned int length);
int anam_put(SvarList * q, Svar * new_item);
int anam_remove_and_destroy(SvarList * q, Svar * item_to_remove);
int anam_count(SvarList * q);
int anam_empty(SvarList * q);
Svar * anam_get_by_index(SvarList * q, unsigned int index);
Svar * anam_get(SvarList * q, void * location);
Svar * anam_peek(SvarList * q);
void * anam_alloc_add(SvarList * list, unsigned int length);



