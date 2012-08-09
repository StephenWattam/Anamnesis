#include "anamnesis.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <stdio.h>

/*
//FOR REFERENCE ONLY, THESE ARE IN THE .H

typedef struct safe_var{
	struct safe_var * next;
	void * location;
	unsigned long int size;
} Svar;

typedef struct safe_var_list{
	Svar *front, *back;
} SvarList;

*/


//thanks to http://osdir.com/ml/lib.common-c++.bugs/2004-01/msg00002.html 
long getTicks() {
        long ticks;
        struct timeval now;
        gettimeofday(&now, NULL);
        ticks = now.tv_sec * 1000l;
        ticks += now.tv_usec / 1000l;
        return ticks;
};

/*
   Initialise anamnesis.
   This creates an empty linked list of safe vars 'Svars'.
*/
int anam_init(SvarList * q){
	//Svar test;
	//printf("%i", sizeof(test));
	srand(getTicks());

	if(!(q->front = (Svar *)malloc(sizeof(struct safe_var))))
		return -1;	//malloc failed, whine.
	q->back = q->front;
	q->front->next = q->back;
	return 0;
}

void anam_destroy_var(Svar * var){
	free(var->location);
	free(var);
}

/*
   This frees memory used by the list only: none of the member variables
*/   
void anam_destroy(SvarList * q){
	Svar * item = q->front->next;
	Svar * oldItem;
	unsigned int i = 0;
	//printf("removing...\n");
	while(item != q->back){
		//printf("%p:%p\n", item_to_remove, item->next);
		//item->next = item->next->next;	//cure dangling pointer
		oldItem = item;
		item = item->next;
		anam_destroy_var(oldItem);

		i++;
	}
	free(q->front);
}

/* 
   Verifies the integrity of a variable.
   Returns true if no overflow has occurred, false if it's been fucked.
*/   
int anam_verify(Svar * var){
	byte * testByte = var->location + var->size;
	if(*testByte == var->checkbyte)
		return 1;	//valid
	return 0;	//invalid
}

Svar * anam_alloc(unsigned int length){

	byte * checkByte;

	Svar * var = malloc(sizeof(Svar));
	var->size = length;
	var->next = (Svar *)0;
	var->location = malloc(length +1);
	var->checkbyte = (byte)(128*(rand()/((double)RAND_MAX + 1)));	//255 fits in a byte, I'm playing safe
	
	checkByte = var->location + length;
	*checkByte = var->checkbyte;
	return var;
}

void anam_clear(Svar * var){
	memset(var->location, '\0', var->size);	//does not clear verification block
}

Svar * anam_alloc_clear(unsigned int length){
	Svar * var = anam_alloc(length);
	anam_clear(var);
	return var;
}

/*
   Create a new variable at the end of the list
*/   
int anam_put(SvarList * q, Svar * new_item){
	//printf("front: %p, back: %p\n", q->front, q->back);
	new_item->next = q->back;//set to back of queue
	//printf("New item: %p, next: %p\n", new_item, new_item->next);

	//now find where to put it
	if(q->front->next == q->back){
		//printf("if\n");
		q->front->next = new_item;
		//printf("==%p==", new_item);
	}else{
		//printf("else\n");
		//printf("::%p::", new_item);
		Svar * item = q->front->next;
		while(item->next != q->back)
			item = item->next;
		item->next = new_item;
	}
	return 0;
}

/*
   Remove a variable from the list and clear its memory.
   THIS FREES MEMORY.
*/
int anam_remove_and_destroy(SvarList * q, Svar * item_to_remove){
	Svar * item = q->front->next;
	Svar * oldItem;
	unsigned int i = 0;
	//printf("removing...\n");
	while(item != q->back){
		//printf("%p:%p\n", item_to_remove, item->next);
		if(item->next == item_to_remove){
			
			//REORDER!
			oldItem = item->next;
			item->next = item->next->next;
			anam_destroy_var(oldItem);
			return i;
		}
		item = item->next;
		
		i++;
	}
	return -1;
}

/*
   Count all variables held
*/  
int anam_count(SvarList * q){
	Svar * item = q->front->next;
	unsigned int i = 0;
	//printf("counting [front: %p, back: %p]...\n", q->front, q->back);
	//printf("[");
	while(item != q->back){
		//printf("item: %p; filedesc: %i,", item, item->data.user_data.socket_set.file_descriptor);
		//printf("item: %p, i: %i, item->next: %p\n", item, i, item->next);
		//sleep(1);
		item = item->next;
		i++;
	}
	//printf("]\n");
	return i;
}

/*
   Is the memory manager empty?
*/   
int anam_empty(SvarList * q){
	if(q->front->next == q->back)
		return 1;
	return 0;
}


/*
   Returns a pointer to a variable
*/
Svar * anam_get_by_index(SvarList * q, unsigned int index){
	Svar * item = q->front->next;
	unsigned int i = 0;
	while(item != q->back){
		if(i == index)
			return item;
		item = item->next;
		i++;
	}
	return (Svar *) 0;
}

/*
   Returns a pointer to a variable when give the memory is occupies, or 0
*/
Svar * anam_get(SvarList * q, void * location){
	Svar * item = q->front->next;
	while(item != q->back){
		if(item->location == location)
			return item;
		item = item->next;
	}
	return (Svar *) 0;
}

/*
   Returns a pointer to the front variable
*/   
Svar * anam_peek(SvarList * q){
	return q->front->next;
}

/*Svar * anam_get(SvarList * q){
	if(anam_empty(q))
		return (Svar *)0;

	Svar * itemToReturn = q->front->next;
	//printf("--%p--", itemToReturn);
	q->front->next = q->front->next->next;
	return itemToReturn;
}*/

void * anam_alloc_add(SvarList * list, unsigned int length){
	Svar * var = anam_alloc(length);
	anam_put(list, var);
	return var->location;
}


Svar * anam_alloc_add_sval(SvarList * list, unsigned int length){
	Svar * var = anam_alloc(length);
	anam_put(list, var);
	return var;
}	

void * anam_alloc_add_clear(SvarList * list, unsigned int length){
	Svar * var = anam_alloc_add_sval(list, length);
	anam_clear(var);
	return var->location;
}
void * anam_resize_and_reallocate(Svar * var, unsigned int desiredLength){
	void * tempBuffer = malloc(desiredLength + 1);
	byte * checkByte;
	memset(tempBuffer, '\0', (desiredLength));	//does not clear verification block

	if(desiredLength < var->size)
		memcpy(tempBuffer, var->location, desiredLength);
	else
		memcpy(tempBuffer, var->location, var->size);	

	//clear the current pointer, it dangles at this point
	free(var->location);
	//move the location
	var->location = tempBuffer;
	//update length
	var->size = desiredLength;

	//write verification block	
	checkByte = var->location + var->size;
	*checkByte = var->checkbyte;
}

void anam_print_list(SvarList * q){
	Svar * item = q->front->next;
	unsigned int i = 0;
	printf("----------------- SvarList at %p \n", q);
	printf("[front: %p, back: %p]...\n", q->front, q->back);

	while(item != q->back){
		printf("[%p --> %p]\n", item, item->next);
		item = item->next;
		i++;
	}
	printf("-----------------------------------\n");
}


void anam_print(Svar * var){
	int i,j;
	byte * checkByte;
	unsigned short int * val;
	char * charVal;


	checkByte = var->location + var->size;
	printf("---------------- Svar at %p \n", var);
	printf("Checkbyte: %i\t found: %i\n", var->checkbyte, *checkByte);
	if(anam_verify(var))
		printf("Variable is still safe\n");
	else
		printf("***Buffer overflow has occurred!\n");
	printf("-----------------------------------\n");
	
	//printf("Bit\t|\tAddress   \t|\tvalue   \t|\tchar\n");
	for(i=0;i<var->size;i++){
		val = (int *)(var->location + i);
		charVal = (char *)(var->location + i);
		//printf("%i\t|\t%p\t|\t%i\t|\t%c\t|", i, (var->location + i), *val, *charVal);
		printf(" |");
		for(j=0;j<8;j++){
			if( (1 << j) & *val)
				printf("1");
			else
				printf("0");
		}
		printf("[%c]| \t", *val);
		if(i%2)
			printf("\n");
	}
	printf("-----------------------------------\n");
}
