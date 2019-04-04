#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<math.h>

int t, *result, *number;
pthread_t thread_id[100];

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int result[], int low, int high) 
{ 
    int pivot = result[high];    
    int i = (low - 1), j;  
  
    for (j = low; j <= high- 1; j++) 
    { 
        if (result[j] <= pivot) 
        { 
            i++;
            swap(&result[i], &result[j]); 
        } 
    } 
    swap(&result[i + 1], &result[high]); 
    return (i + 1); 
} 

void quickSort(int result[], int low, int high) 
{ 
    if (low < high) 
    { 
        int pi = partition(result, low, high); 
  
        quickSort(result, low, pi - 1); 
        quickSort(result, pi + 1, high); 
    } 
} 

void* factorial(void *arg)
{
    int a, iter, check;
    pthread_t id=pthread_self();
    for(a=0;a<t;a++){
        if(pthread_equal(id, thread_id[a])){
			result[a]=1;
			for(iter=number[a]; iter>1; iter--) result[a]*=iter;
		}
    }
	return NULL;
}

int main(int count_param, char *my_param[])
{
	int i, err;
	t=count_param;
	result=(int *) malloc(t*sizeof(int));
	number=(int *) malloc(t*sizeof(int));
	for(i=1; i<t; i++){
		number[i-1]=atoi(my_param[i]);
        err=pthread_create(&(thread_id[i-1]),NULL,&factorial,NULL);
	}
	for(i=0; i<t; i++){
       pthread_join(thread_id[i],NULL);
	}
	quickSort(result, 0, t-2);
	for(i=0; i<t-1; i++){
		printf("%d\n", result[i]);
	}

    free(result);
    free(number);
	exit(0);
	return 0;
}
