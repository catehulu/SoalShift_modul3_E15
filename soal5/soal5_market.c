#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

int status_restock, *stock_toko, flag;
pthread_t my_thread_id[2];

int mygetch ( void ) 
{
  int my_char_input;
  struct termios old_termios, new_termios;
  
  tcgetattr ( STDIN_FILENO, &old_termios );
  new_termios = old_termios;
  new_termios.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &new_termios );
  my_char_input = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &old_termios );
  
  return my_char_input;
}

void *shop_ui(void *arg){
    while(flag){
        printf("Shop\n");
        printf("Food stock: %d\n", *stock_toko);
        printf("Choices:\n");
        printf("\t1. Restock\n");
        printf("\t2. Exit\n");
        printf("Your choice: ");
        fflush(stdout);
        sleep(1);
        system("clear");
    }
}

void *shop_input(void *arg){
    while(flag){
        char a=mygetch();
        if(a=='1'){
            (*stock_toko)++;
        }
        else if(a=='2'){
            flag=0;
        }
    }
}

int main(){
    key_t key=1234;
    int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock_toko=shmat(shmid, NULL, 0);
    
    *stock_toko=10;
    flag=1;

    pthread_create(&(my_thread_id[0]), NULL, &shop_ui,NULL);
    pthread_create(&(my_thread_id[1]), NULL, &shop_input,NULL);

    pthread_join(my_thread_id[0], NULL);
    pthread_join(my_thread_id[1], NULL);

    shmdt(stock_toko);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}