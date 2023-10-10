#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sched.h>

#include <unistd.h>

#include <math.h>


#define N	(3)
#define ATTEMPS		(100000000)

typedef struct{

	int id;
	int* input;
	int* output;
} thread_data_type;


void* thread_routine(void* arg)
{
	int i = 0;
	thread_data_type* ptr_data = (thread_data_type*) arg;
	int id = ptr_data->id;
	int* in = ptr_data->input;
	int* out = ptr_data->output;
	int local_id = -1;

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	i =	pthread_getaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);

	if(CPU_ISSET(0, &cpuset)) local_id = 0;
	if(CPU_ISSET(1, &cpuset)) local_id = 1;
	if(CPU_ISSET(2, &cpuset)) local_id = 2;
	if(CPU_ISSET(3, &cpuset)) local_id = 3;

	printf("I'm thread #%d, and I am working in CPU_%d\n",id, local_id);


	for(i = 0; i < ATTEMPS; ++i){
		
		out[0] = sin((float)in[0]) + sqrt(in[0]/2.);
	}

	printf("working pthread #%d\n", id);

	return NULL;
}


int main(int argc, char** argv)
{
	int i = 0;
	int ret_val = 0;
	int in[N];
	int out[N];

	cpu_set_t cpuset[N];
	pthread_t pmas[N];
	pthread_attr_t pattr;

	thread_data_type thdp[N];

	for(i=0; i < N; ++i){
	
		thdp[i].id = i;
		thdp[i].input = &in[i];
		thdp[i].output = &out[i];
	}


	for(i=0; i < N; ++i){

		CPU_ZERO(&cpuset[i]);
		CPU_SET((i+1)%4, &cpuset[i]);
	}

	printf("Hello world!\n");

	for(i=0; i < N; ++i){

		ret_val = pthread_create(&pmas[i], NULL, &thread_routine, (void*) (&thdp[i]));
		ret_val = pthread_setaffinity_np(pmas[i], sizeof(cpu_set_t), &cpuset[i]);
	}

	printf("start join threads!\n");

	for(i = 0; i < N; ++i)
	{
		pthread_join(pmas[i],NULL);
	}

	printf("return from main programm!\n");


	return 0;
}

