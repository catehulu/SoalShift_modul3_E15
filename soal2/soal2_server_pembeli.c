#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8888

int *data, flag=1;

int main(int argc, char const *argv[]) {
    int server_fd, valread, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
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

    char buffer_for_read[1024], buffer_for_send[1024];
    int value_read;
    
    while(flag){
        while(!(read(new_socket, buffer_for_read, 1024)));
        if(!strcmp("beli", buffer_for_read)){
            if((*data)==0) sprintf(buffer_for_send, "transaksi gagal\n");
            else sprintf(buffer_for_send, "transaksi berhasil\n"), (*data)--;
        }
        else if(!strcmp("exit", buffer_for_read)){
            strcpy(buffer_for_send, buffer_for_read);
            flag=0;
        }
        send(new_socket, buffer_for_send, strlen(buffer_for_send), 0);
    }

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}