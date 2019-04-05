#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <dirent.h>
#include <errno.h>

pthread_t tid_list[3]; 
pthread_t tid_zip[3]; 
pthread_t tid_unzip[3]; 
pthread_t tid_timer; 

void* createlist(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_list[0])) 
	{
		DIR* dir = opendir("/home/catehulu/Documents/FolderProses1");
		if(dir == NULL) system("mkdir ~/Documents/FolderProses1");
		closedir(dir);
		system("ps -aux | head -n11 >> ~/Documents/FolderProses1/SimpanProses1.txt");
	}
	else if(pthread_equal(id,tid_list[1])) 
	{
		DIR* dir = opendir("/home/catehulu/Documents/FolderProses2");
		if(dir == NULL) system("mkdir ~/Documents/FolderProses2");
		closedir(dir);
		system("ps -aux | head -n11 >> ~/Documents/FolderProses2/SimpanProses2.txt");
	}

	pthread_exit(NULL);
}

void* zipfile(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_zip[0])) 
	{
		system("zip -j ~/Documents/FolderProses1/KompresProses1.zip ~/Documents/FolderProses1/SimpanProses1.txt >> /dev/null");
		system("rm ~/Documents/FolderProses1/SimpanProses1.txt  >> /dev/null");
	}
	else if(pthread_equal(id,tid_zip[1])) 
	{
		system("zip -j ~/Documents/FolderProses2/KompresProses2.zip ~/Documents/FolderProses2/SimpanProses2.txt  >> /dev/null");
		system("rm ~/Documents/FolderProses2/SimpanProses2.txt  >> /dev/null");
	}

	pthread_exit(NULL);

}

void* unzip(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_unzip[0])) 
	{
		system("unzip ~/Documents/FolderProses1/KompresProses1.zip -d ~/Documents/FolderProses1/  >> /dev/null");
	}
	else if(pthread_equal(id,tid_unzip[1])) 
	{
		system("unzip ~/Documents/FolderProses2/KompresProses2.zip -d ~/Documents/FolderProses2/  >> /dev/null");
	}

	pthread_exit(NULL);
}

void* wait_timer(void *arg)
{
	printf("Menunggu 15 detik untuk ekstrak kembali\n");
	sleep(15);
	pthread_exit(NULL);
}

int main(void)
{
	int err;
	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_list[i]),NULL,&createlist,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}
	
	pthread_join(tid_list[0],NULL);
	pthread_join(tid_list[1],NULL);

	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_zip[i]),NULL,&zipfile,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}
	
	pthread_join(tid_zip[0],NULL);
	pthread_join(tid_zip[1],NULL);
	
	err=pthread_create(&(tid_timer),NULL,&wait_timer,NULL);
	pthread_join(tid_timer,NULL);

	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_unzip[i]),NULL,&unzip,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}

	pthread_join(tid_unzip[0],NULL);
	pthread_join(tid_unzip[1],NULL);
	
	exit(0);
	return 0;
}
