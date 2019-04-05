#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8888

int sock, flag=1;
pthread_t thread_id[2];     // Thread 1 menangani penerimaan pesan
                            // Thread 2 menangani pengiriman pesan

void *send_a_message(void *arg){
    char buffer[1024];
    
    while(flag){
        scanf("%s", buffer);
        send(sock, buffer, strlen(buffer), 0);
        if(!strcmp(buffer, "exit")) flag=0;
    }
}

void *recieve_a_message(void *arg){
    char buffer[1024];
    while(flag){
        memset(buffer, '\0', sizeof(buffer));
        while(!(read(sock, buffer, 1024)));
        if(!(strcmp(buffer, "exit"))) flag=0;
        else printf("%s\n", buffer);
    }
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    pthread_create(&(thread_id[0]),NULL,&recieve_a_message,NULL);
    pthread_create(&(thread_id[1]),NULL,&send_a_message,NULL);
    
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    return 0;
}