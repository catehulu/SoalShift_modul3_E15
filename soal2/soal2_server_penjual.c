#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 7777

pthread_t thread_id[2];     // Thread 1 menangani penerimaan pesan
                            // Thread 2 menangani pengiriman pesan
int new_socket, *data, status_write, status_read;
int flag=1;

void *recieve_a_message(void *arg){
    char buffer[1024];
    int value_read;
    
    while(flag){
        while(!(read(new_socket, buffer, 1024)));
        if(!strcmp("tambah", buffer)){
            printf("Masuk\n");
            while(status_write || status_read);
            status_write=1;
            (*data)++;
            status_write=0;
        }
        if(!(strcmp("exit", buffer))){
            flag=0;
        }
    }
}

void *send_a_message(void *arg){
    char buffer[1024];
    
    while(flag){
        while(status_read || status_write);
        status_read=1;
        memset(buffer, '\0', sizeof(buffer));
        sprintf(buffer, "Stok barang saat ini:%d", *data);
        send(new_socket, buffer, strlen(buffer), 0);
        status_read=0;
        sleep(5);
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024];
    strcpy(buffer, "exit");
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    key_t key=1234;
    int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
    data=shmat(shmid, NULL, 0);

    *data=3;

    pthread_create(&(thread_id[0]),NULL,&recieve_a_message,NULL);
    pthread_create(&(thread_id[1]),NULL,&send_a_message,NULL);
    
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    send(new_socket, buffer, strlen(buffer), 0);

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}