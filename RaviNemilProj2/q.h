// Need to modify the changes to incorporate the functionality of Semaphore Queue
#include "TCB.h"
TCB_t * RunQ; // pointer to the head of the RunQ
//Note that if Q is a head pointer to the queue, then InitQ will have to be passed &Q.
void InitQ (TCB_t ** Q)
{
	// Initialize the Semaphore Queue to Null.
	// Called when Semaphore is created	
    	*Q = NULL;
}


// Adding an item to the Semaphore Queue
void AddQ(TCB_t ** Q, TCB_t *item)
{
	// Null Check, if queue is empty,
	// add the item and set its previous and next
	// pointer to itself
	if (*Q == NULL)
	{
		*Q = item;
	        item->previous = item;
        	item->next = item;
     	}
	else
	{
		item->previous = (*Q)->previous;
        	item->next = *Q;
        	(*Q)->previous->next = item;
        	(*Q)->previous = item;
    	}
    
}

// will return a pointer to the item deleted.
TCB_t * DelQ(TCB_t ** Q) // will return a pointer to the item deleted
{
	// Null Check: Danger
	if (*Q ==NULL)
	{
		// nothing to be deleted, return the head(NULL)
		return NULL;
    	}
	TCB_t *delItem = *Q;
	// Danger again: Segmentation Fault. 
    	// if (delItem->previous == delItem->next)
    	if (delItem->previous == delItem && delItem->next == delItem)
    	{
        	*Q = NULL;
    	}
    	else
    	{
        	delItem->previous->next = delItem->next;
        	delItem->next->previous = delItem->previous;
        	*Q = delItem->next;
    	}
    	return delItem;// will return a pointer to the item deleted
}

// deletes the head and adds it to the tail, by just moving the header pointer to the next item.
void RotateQ(TCB_t ** Q)
{
	// Null Check: important
	if (*Q==NULL)
    	{
        	return;
    	}
    	TCB_t *delItem = DelQ(Q);// delete the head
    	AddQ(Q, delItem);// add the deleted node to the end, hence rotate
}


