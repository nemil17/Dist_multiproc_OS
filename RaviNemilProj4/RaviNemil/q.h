#ifndef Q_H_INCLUDED
#define Q_H_INCLUDED

#include "TCB.h"

TCB_t * RunQ;

//Note that if Q is a head pointer to the queue, then InitQ will have to be passed &Q.
void InitQ (TCB_t ** Q)
{
    *Q = NULL;
}

void AddQ(TCB_t ** Q, TCB_t *item)
{
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
        //
        (*Q)->previous->next = item;
        (*Q)->previous = item;
    }
    //
//    TCB_t * t = *Q;
//    printf("AddQ %p -> ", Q);
//    do
//    {
//        printf("%p <-> ", t);
//        t = t->next;
//    }
//    while(t!=*Q);
//    printf("\n");
}

// will return a pointer to the item deleted.
TCB_t * DelQ(TCB_t ** Q)
{
    TCB_t * del = *Q;
    //
    if (*Q ==NULL)
    {
//        printf("DelQ %p -> NULL\n", Q);
        return NULL;
    }
    //
//    TCB_t * t = *Q;
//    printf("DelQ %p -> ", Q);
//    do
//    {
//        printf("%p <-> ", t);
//        t = t->next;
//    }
//    while(t!=*Q);
//    printf("\n");
    //
    if (del->previous == del && del == del->next)
    {
        *Q = NULL;
    }
    else
    {
        del->previous->next = del->next;
        del->next->previous = del->previous;
        *Q = del->next;
    }
    return del;
}

// deletes the head and adds it to the tail, by just moving the header pointer to the next item.
void RotateQ(TCB_t ** Q)
{
    if (*Q==NULL)
    {
        return;
    }
    TCB_t *item = DelQ(Q);
//    printf("RotateQ ing ..\n");
    AddQ(Q, item);
}

#endif // Q_H_INCLUDED

