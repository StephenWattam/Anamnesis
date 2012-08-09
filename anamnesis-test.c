#include <stdlib.h>
#include "anamnesis.h"

int main(int argc, char ** argv){
	SvarList vars;
	anam_init(&vars);


	char * buffer = (char *)anam_alloc_add(&vars, 255);
	char * buffer2 = (char *)anam_alloc_add(&vars, 2055);
	char * buffer3 = (char *)anam_alloc_add(&vars, 255);
	char * buffer4 = (char *)anam_alloc_add(&vars, 255);

	buffer2 = (char *)anam_alloc_add(&vars, 2055);
	buffer2 = (char *)anam_alloc_add(&vars, 2055);
	buffer2 = (char *)anam_alloc_add(&vars, 2055);
	buffer2 = (char *)anam_alloc_add(&vars, 2055);
	buffer2 = (char *)anam_alloc_add(&vars, 2055);
	anam_remove_and_destroy(&vars, anam_get_by_index(&vars, 2));

	anam_remove_and_destroy(&vars, anam_get(&vars, buffer2));

	anam_destroy(&vars);
	return 0;
}
