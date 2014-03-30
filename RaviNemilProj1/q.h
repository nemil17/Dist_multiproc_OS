#include "TCB.h"
TCB_t *RunQ;// pointer to the head of  the queue
// Note that if Q is a head pointer to the queue
// then InitQ will have to be passed &Q. 
void InitQ()
{
	RunQ = NULL;
}

void AddQ(TCB_t *item)
{
	// Null Check, if queue is empty, 
	// add the item and set its previous and next
	// pointer to itself
	if(RunQ == NULL)
	{
		RunQ = item;
		item->previous = item;
		item->next = item;
	}
	else
	{
		item->previous = RunQ->previous;
		item->next = RunQ;
		RunQ->previous->next = item;
		RunQ->previous = item;
	}
}

TCB_t *DelQ() // will return a pointer to the item deleted.
{
	TCB_t *delItem = RunQ; // head item is deleted
	if(delItem == NULL)
	{
		// nothing to be deleted, return the  head(NULL)
		return delItem;
	}
	// if only one item is present in the queue
	if(delItem->previous == delItem->next)
	{
		RunQ = NULL;
	}
	else
	{
		delItem->previous->next = delItem->next;
		delItem->next->previous = delItem->previous;
		RunQ = delItem->next;
	}
	return delItem;// will return a pointer to the item deleted.

}

// deletes the head and adds it to the tail, by just moving the header pointer to the next item.
TCB_t RotateQ()
{
	TCB_t *delItem = DelQ();// delete the head
	AddQ(delItem);// add the deleted node to the end, hence rotate
}


	
