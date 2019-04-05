#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t a,s; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
int status = 1;
int mode = 3;
int counterA = 1;
int counterB = 1;

void* counter1(void *arg)
{
    sleep(10);
    counterB = 1;

}

void* counter2(void *arg)
{
    sleep(10);
    counterA = 1;

}

void* agmal(void *arg)
{
    int WakeUp_Status = 0;
    int counter = 0;
    pthread_t local_thread;
    //printf("Thread a created\n");
    while(1){
        while(mode != 0 && mode != 1 && status);
        if (mode == 0) {
            printf("Agmal WakeUp_Status = %d\n",WakeUp_Status);
            sleep(1);
        }
        else if (!counterA)
        {
            printf("Agmal Ayo Bangun disabled 10 s\n");
            mode = 3;
        }
        else if (mode == 1){
            WakeUp_Status += 15;
            counter++;
            if (WakeUp_Status >= 100) {
                printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                pthread_cancel(s);
                status = 0;
                return NULL;

            }
            
            if (counter == 3) {
                pthread_create(&(local_thread), NULL, counter1, NULL);
                counterB = 0;
            }

            mode = 3;
        }
        
    }
    
}

void* iraj(void *arg)
{
    int Spirit_Status = 100;
    int counter = 0;
    pthread_t local_thread;
    //printf("Thread s created\n");
    while(1){
        while(mode != 0 && mode != 2 && status);
        if (mode == 0) {
            printf("Iraj Spirit_Status = %d\n",Spirit_Status);
            sleep(1);
        }
        else if (!counterB)
        {
            printf("Iraj Ayo Tidur disabled 10 s\n");
            mode = 3;
        }
        else if (mode == 2){
            Spirit_Status -= 20;
            counter++;
            if (Spirit_Status <= 0) {
                printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                status = 0;
		        pthread_cancel(a);
                return NULL;
            }
            
            if (counter == 3) {
                pthread_create(&(local_thread), NULL, counter2, NULL);
                counterB = 0;
            }

            mode = 3;
        }
    }
}
        

int main(void)
{
    char fitur1[] = "All Status";
    char fitur2[] = "Agmal Ayo Bangun";
    char fitur3[] = "Iraj Ayo Tidur";
    char *input = malloc(50*sizeof(char));
    int err;
    pthread_create(&(a), NULL, &agmal, NULL);
    pthread_create(&(s), NULL, &iraj, NULL);

    while(status){
        fgets(input, 50, stdin);
        if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n'))
            input[strlen (input) - 1] = '\0';
        if (strcmp(fitur1,input) == 0) {
            printf("input fitur 1\n");
            mode = 0;
            sleep(1);
            mode = 3;
        }
        else if (strcmp(fitur2,input) == 0)
        {
            printf("input fitur 2\n");
            mode = 1;
            sleep(1);
        }
        else if (strcmp(fitur3,input) == 0)
        {
            printf("input fitur 3\n");
            mode = 2;
            sleep(1);
        }
    }
    exit(EXIT_SUCCESS);
}
