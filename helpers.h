#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"
#include "symbols.h"

struct detalii_pacient{
	int delay_venire,durata_consultatie,id;
};

struct queue *q;

pthread_mutex_t mtx1,mtx2;
sem_t sem1,sem2;


int initiate_mtx_sem(){
	if(sem_init(&sem1,0,MAX_NR_DOCTORI)){
        perror(NULL);
        return errno;
    }
    
    if(sem_init(&sem2,0,0)){
        perror(NULL);
        return errno;
    }
	
    if(pthread_mutex_init(&mtx1,NULL)){
        perror(NULL);
        return errno;
    }
    
    if(pthread_mutex_init(&mtx2,NULL)){
        perror(NULL);
        return errno;
    }
    return 0;
}

int destroy_mtx_sem(){
	if(pthread_mutex_destroy(&mtx1)){
        perror(NULL);
        return errno;
    }
    if(pthread_mutex_destroy(&mtx2)){
        perror(NULL);
        return errno;
    }
    if(sem_destroy(&sem1)){
    	perror(NULL);
        return errno;
    }
    if(sem_destroy(&sem2)){
    	perror(NULL);
        return errno;
    }
    return 0;
}






