#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

pthread_t my_thread_id[10];

typedef struct pet{
    char name[13];
    int health;
    int hunger;
    int hygiene;
    int food;
    char status_bath[50];
}my_pet;

my_pet hachi;

int status_bathroom, status_restock, *stock_toko, cooldown, flag, status_game, HP_enemy;
int baru, win;

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

void *cooldown_bath(void *arg){
    while(flag){
        while(cooldown!=0 && flag){
            if(status_game==1){
                sprintf(hachi.status_bath, "Bath will be ready in %d seconds", cooldown);
                cooldown--;
                sleep(1);
            }
        }
        sprintf(hachi.status_bath, "Bath is ready");
    }
}

void *health_increase(void *arg){
    int sleep_count=0;
    while(flag){
        if(status_game==1){
            sleep(1);
            sleep_count++;
            if(sleep_count==10) hachi.health+=5, sleep_count=0;
            if(hachi.health>=300) hachi.health=300;
        }
    }
}

void *hunger_decrease(void *arg){
    int sleep_count=0;
    while(flag){
        if(status_game==1){
            sleep(1);
            sleep_count++;
            if(sleep_count==10) hachi.hunger-=5, sleep_count=0;
        }
    }
}

void *hygiene_decrease(void *arg){
    int sleep_count=0;
    while(flag){
        if(status_game==1){
            sleep(1);
            sleep_count++;
            if(sleep_count==30) hachi.hygiene-=10, sleep_count=0;
        }
    }
}

void *game_ui(void *arg){
    int a=0;
    while(flag){
        system("clear");
        if(hachi.health<=0 || hachi.hunger<=0 || hachi.hygiene<=0){
            printf("YOU LOSE!!\n"); 
            flag=0;
            exit(0);
        }
        else if(win==1 && a==0){
            a=3;
            printf("You win\n");
        }
        else if(win==1 && a!=0){
            printf("You win\n");
            a--;
            if(a==0){
                status_game=1;
                win=0;
            } 
        }
        else if(status_game==1){
            printf("Standby Mode\n");
            printf("Health\t: %d\n", hachi.health);
            printf("Hunger\t: %d\n", hachi.hunger);
            printf("Hygiene\t: %d\n", hachi.hygiene);
            printf("Food left\t: %d\n", hachi.food);
            printf("%s\n", hachi.status_bath);
            printf("Choices:\n");
            printf("\t1. Eat\n");
            printf("\t2. Bath\n");
            printf("\t3. Battle\n");
            printf("\t4. Shop\n");
            printf("\t5. Exit\n");
            printf("Your choice: ");
        }
        else if(status_game==2){
            if(baru){
                HP_enemy=100;
                baru = 0;
            }
            printf("Battle Mode\n");
            printf("Monster's Health\t: %d\n", hachi.health);
            printf("Enemy's Health\t\t: %d\n", HP_enemy);
            printf("Choices:\n");
            printf("\t1. Attack\n");
            printf("\t2. Run\n");
        }
        else if(status_game==3){
            printf("Shop Mode\n");
            printf("shop food stock\t: %d\n", *stock_toko);
            printf("Your food stock\t: %d\n", hachi.food);
            printf("Choices:\n");
            printf("\t1. Buy\n");
            printf("\t2. Back\n");
        }
        fflush(stdout);
        sleep(1);
    }
}

void *game_input(void *arg){
    while(flag){
        char a=mygetch();
        if(status_game==1){
            if(a=='1'){
                if(hachi.food!=0){
                    hachi.hunger+=15;
                    if(hachi.hunger>200) hachi.hunger=200;
                    hachi.food--;
                }
            }
            else if(a=='2'){
                if(cooldown==0){
                    hachi.hygiene+=30; 
                    if(hachi.hygiene>100) hachi.hygiene=100;
                    cooldown=20;
                }
            }
            else if(a=='3'){
                baru=1;
                status_game=2;
            }
            else if(a=='4'){
                status_game=3;
            }
            else if(a=='5'){
                flag=0;
            }
        }
        else if(status_game==2){
            if(a=='1'){
                HP_enemy-=20;
                hachi.health-=20;
                if(HP_enemy<=0) win=1;
            }
            else if(a=='2'){
                status_game=1;
            }
        }
        else if(status_game==3){
            if(a=='1'){
                if((*stock_toko)!=0){
                    hachi.food++;
                    (*stock_toko)--;
                }
                else{
                    printf("\nCannot buy a food\n");
                }
            }
            else if(a=='2'){
                status_game=1;
            }
        }
    }
}

int main(){
    status_game=1;
    flag=1;
    cooldown=0;
    win=0;
    hachi.health=300;
    hachi.hunger=200;
    hachi.hygiene=100;
    hachi.food=10;
    printf("Insert your pet's name: ");
    scanf("%s", hachi.name);

    key_t key=1234;
    int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock_toko=shmat(shmid, NULL, 0);

    pthread_create(&(my_thread_id[0]), NULL, &cooldown_bath, NULL);
    pthread_create(&(my_thread_id[1]), NULL, &game_ui, NULL);
    pthread_create(&(my_thread_id[2]), NULL, &game_input, NULL);
    pthread_create(&(my_thread_id[3]), NULL, &health_increase, NULL);
    pthread_create(&(my_thread_id[4]), NULL, &hunger_decrease, NULL);
    pthread_create(&(my_thread_id[5]), NULL, &hygiene_decrease, NULL);

    pthread_join(my_thread_id[0], NULL);
    pthread_join(my_thread_id[1], NULL);
    pthread_join(my_thread_id[2], NULL);
    pthread_join(my_thread_id[3], NULL);
    pthread_join(my_thread_id[4], NULL);
    pthread_join(my_thread_id[5], NULL);

    shmdt(stock_toko);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}