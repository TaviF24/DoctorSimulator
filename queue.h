#include<stdio.h>
#include<stdlib.h>

struct queue {
    int value; int size;
    struct queue *next;
    struct queue *back;
};

int init(struct queue **q){
    *q=(struct queue*)malloc(sizeof(struct queue));
    if(*q==NULL)
        return 1;
    (*q)->next=NULL;
    (*q)->back=NULL;
    (*q)->size=0;
    (*q)->value=0;
    return 0;
}

int front(struct queue *q){
    if(q->size)
        return q->next->value;
}

int push(int value, struct queue *q){
    struct queue *q2 = (struct queue*)malloc(sizeof(struct queue));
    if(q2==NULL)
        return 1;

    struct queue *aux=q;
    while(aux->next!=NULL)
        aux=aux->next;
    aux->next=q2;

    q->back=q2;
    q2->value=value;
    q2->size=aux->size+1;
    q->size=q2->size;
    return 0;
}

int pop(struct queue *q){
    if(q->next==NULL)
        return 1;
    struct queue *aux= q->next->next;
    free(q->next);
    q->next=aux;
    q->size-=1;
    if(q->size)
    	q->back->size-=1;
    else
    	q->back=NULL;
    return 0;
}

void destroy(struct queue *q){
    while(q->next!=NULL)
        pop(q);
}








