#include "mm.h"

//Memory map array
static unsigned short mem_map [PAGING_PAGES] = {0,};

//Allocate memory for a new task
unsigned long get_free_page()
{
	for (int i = 0; i < PAGING_PAGES; i++)
	{
		if (mem_map[i] == 0)
		{
			mem_map[i] = 1;
			return LOW_MEMORY1 + i*PAGE_SIZE;
		}
	}
	return 0;
}

//Assumming it works as advertised
void free_page(unsigned long p)
{
	mem_map[(p - LOW_MEMORY1) / PAGE_SIZE] = 0;
}
