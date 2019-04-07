#include "ring.h"

int entries( ring_t *ring )
{
	int i = 0;
	
	// check if empty
	if(ring->Ini == ring->Outi)
	{
		printf("Number of buffer entries: 0\n");
		
		return 0;
	}
	
	// loop through until input and output are same
	while(ring->Ini != ring->Outi)
	{
		ring->Ini - 1;
		i++;
	}
	
	// print number of entries
	printf("Number of Buffer Entries: %d\n", i);
	
	return i;
}