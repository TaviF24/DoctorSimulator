#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "helpers.h"

int nr_pacienti=MAX_NR_PACIENTI;

int random_number(int lower, int upper) { 
	//sleep(0.01);
	struct timespec begin;
	timespec_get(&begin,TIME_UTC);
    srand(begin.tv_nsec); //take nanoseconds as seed
    
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
} 

int consultare(int timp_consultatie, int doctor, struct timespec begin, int pacient_id){
    struct timespec end;
    timespec_get(&end,TIME_UTC);
    double timp_asteptare= (end.tv_sec-begin.tv_sec)+(end.tv_nsec-begin.tv_nsec)/1000000000.0;
    sleep(timp_consultatie);
    char *s;
    if(timp_consultatie==1)
    	s="secunda";
    else
    	s="secunde";
    printf("Gata consultatia: Pacient %d | Doctor %d | Asteptare %lf secunde | Consultatie %d %s\n", pacient_id,doctor+1,timp_asteptare,timp_consultatie, s);
    pthread_mutex_lock(&mtx1);
    push(doctor, q);
    pthread_mutex_unlock(&mtx1);
    return 0;
}

void asteptare_consultare(struct detalii_pacient *arg){
	struct timespec begin;
    timespec_get(&begin,TIME_UTC);
    	
	sem_wait(&sem1);
	pthread_mutex_lock(&mtx1);
	// mereu o sa am ceva in coada in pasul asta, altfel mutex-ul nu lasa thread-ul sa treaca mai departe 
	int doctor = front(q); 
	pop(q);
	pthread_mutex_unlock(&mtx1);
	consultare(arg->durata_consultatie,doctor,begin,arg->id);
	
	sem_post(&sem1);
}

void *bariera(void *arg){
	struct detalii_pacient *det = (struct detalii_pacient*)arg;

	pthread_mutex_lock(&mtx2);
	nr_pacienti--;
	if(nr_pacienti==0){
		pthread_mutex_unlock(&mtx2);
		sem_post(&sem2);
	}
	pthread_mutex_unlock(&mtx2);

	sem_wait(&sem2);
	sem_post(&sem2);
	
	sleep(det->delay_venire);
	char *s;
	if(det->delay_venire==1)
		s="secunda";
	else
		s="secunde";
	printf("Pacientul cu id-ul %d a venit dupa %d %s\n",det->id,det->delay_venire, s);
	asteptare_consultare(det);
	
	free(arg);
	return NULL;
}

int main(int argc, char **argv){

    pthread_t pacienti[MAX_NR_PACIENTI];
    struct timespec begin,end;
    timespec_get(&begin,TIME_UTC);
    init(&q);
    initiate_mtx_sem();
    
    for(int i=0;i<MAX_NR_DOCTORI;i++)
        push(i,q);
	if(MAX_NR_DOCTORI){
		for(int i=0;i<MAX_NR_PACIENTI;i++){
			struct detalii_pacient *arg = (struct detalii_pacient*)malloc(sizeof(struct detalii_pacient));
		    int *dur_consultatie=(int*)malloc(sizeof(int));
		    arg->delay_venire=random_number(0,DURATA_MAX_ASTEPTARE);
		    arg->durata_consultatie=random_number(1,DURATA_MAX_CONSULTATIE);
		    arg->id = i+1;

		    if(pthread_create(&pacienti[i],NULL,bariera,arg)){
		        perror(NULL);
		        return errno;
		    }
		}

		for(int i=0;i<MAX_NR_PACIENTI;i++){
		    if(pthread_join(pacienti[i],NULL)){
		        perror(NULL);
		        return errno;
		    }
		}
    }else
    	printf("Cabinet inchis. Nu sunt doctori!\n");
    

    destroy_mtx_sem();
    destroy(q);
    free(q);
   
    timespec_get(&end,TIME_UTC);
    double time_spent= (end.tv_sec-begin.tv_sec)+(end.tv_nsec-begin.tv_nsec)/1000000000.0;
    
    printf("Total execution time: %lf\n",time_spent);
    return 0;
}

