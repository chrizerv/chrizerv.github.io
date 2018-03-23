#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"









int main (int argc, char **argv)
{
	struct words *wl;
	FILE *s, *l;
	int size;
	int i;
//------------------------------------------------------------//
	if (argc != 3)
	{
		usage();	
		return -1;
	}
//------------------------------------------------------------//


	

	s=fopen(argv[1],"r");
	if (s==NULL)
	{
		printf("\nSource file not found!\n\n");
		return -1;
	}


	l=fopen(argv[2],"r");
	if (l==NULL)
	{
		printf("\nWordlist file not found!\n\n");
		return -1;
	}

	if (is_empty(s,l))
		return -1;


	if(is_not_correct(l))
		return -1;

	size = words_in_list(l);
	wl = (struct words*)malloc(size*sizeof(struct words));

	if(wl==NULL)
	{
		printf("\n Out of Memory \n\n");
		return -1;
	}

	if (word_len_alloc(l,wl,size))
		return -1;

	read_wl_file(l,wl,size);
		
	times(s,wl,size);

	sel_sort(wl,size);

	display(wl,size);

	





//-------------------------------------------//
	
	for(i=0;i<size;i++)
	{
		free(wl[i].name);
	}
	free(wl);

	fclose(l);
	fclose(s);
//-------------------------------------------//
	return 0;
}






