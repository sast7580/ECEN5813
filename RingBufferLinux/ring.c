/*========================================================================
** Author: Sam Solondz
** Date: 3/18/19
** Circular buffer
** ECEN5813
**========================================================================*/
#include "ring.h"
#include "stdlib.h"

/*Initalize a new ring buffer, return a pointer to where it is located in memory*/
ring_t *init( int length )
{
	ring_t newRing;
	ring_t * newRingPtr = malloc(sizeof(newRing));

	newRingPtr->Length = length;
	char arr[length];
	newRingPtr->Buffer = arr;
	for(int i = 0; i<(newRingPtr->Length); i++)				//initialize each character to 0 in the fuller
	{
		newRingPtr->Buffer[i] = '0';
	}
	newRingPtr->Ini = 0;
	newRingPtr->Outi = 0;

	return newRingPtr;
}

/*Insert an entry into the ring buffer. Note: Inefficient due to modulo operation, can be improved */
int insert( ring_t *ring, char data )
{
	//Check to make sure ring is not full (space of 1 between)
	if(((ring->Ini + 1) % ring->Length) == ring->Outi)
	{
		//printf("\n\rRing buffer is full, cannot add data");
		return -1;
	}
	ring->Buffer[ring->Ini] = data;
	ring->Ini = (ring->Ini + 1) % ring->Length;
	return 0;
};

/*Remove an entry from the ring buffer. Note: Inefficient due to modulo operation, can be improved */
int remove_entry( ring_t *ring, char *data )
{
	//Check to make sure ring is not empty
	if(ring->Outi == ring->Ini)
	{
		//printf("\n\rRing buffer is empty, cannot remove data");
		return -1;
	}
	*data = ring->Buffer[ring->Outi];
	ring->Outi = (ring->Outi + 1) % ring->Length;
	//printf("\n\rRemoved %c", *data);
	return 0;
};

/*Count the number of entries in chronological order. Note: Inefficient due to modulo operation, can be improved*/
int entries( ring_t *ring )
{
	int count = 0;
	int traverse = (ring->Outi);			//Set traverse to the read pointer
//printf("\n\rBuffer contents: ");
	while(traverse != ring->Ini)			//Increment the traverse pointer until we get to the write pointer (most recent entry)
	{
		//printf("%c", ring->Buffer[traverse]);
		traverse = (traverse + 1) % ring->Length;
		count++;
	}
	//printf("\n\rNumber of elements = %d\n\t", count);
	return count;
};
