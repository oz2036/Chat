#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

#define BUFF_SIZE 1024

pthread_t threads[5];
int done[5];

void *thread_main(void *);


int main(int argc,char *argv[]){
  int server_socket;
  int client_socket;
  int client_addr_size;
  
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  char buff_rcv[BUFF_SIZE+5];
  char buff_snd[BUFF_SIZE+5];

  server_socket = socket(PF_INET,SOCK_STREAM,0);
  if(server_socket==-1){
    printf("server socket 생성 실패\n");
    exit(1);
  }

  memset(&server_addr,0,sizeof(server_addr));
  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(4000);
  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);

  if(-1 == bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))){
    printf("bind() error\n");
    exit(1);
  }
  
    if(-1 == listen(server_socket,5)){
      printf("listen() error\n");
      exit(1);
    }
    //  printf("thread [%d]\n",threads[0]);
    client_addr_size=sizeof(client_addr);
    client_socket=accept(server_socket,(struct sockaddr*)&client_addr,&client_addr_size);
    if(-1 == client_socket){
      printf("client connecting error\n");
      exit(1);
    }
  while(1){
    memset(buff_rcv,0,BUFF_SIZE);
    read(client_socket,buff_rcv,BUFF_SIZE);
    char ip[15];
    memset(ip,0,15);
    sprintf(ip,"%d.%d.%d.%d",
	    (int)(client_addr.sin_addr.s_addr&0xFF),
	    (int)((client_addr.sin_addr.s_addr&0xFF00)>>8),
	    (int)((client_addr.sin_addr.s_addr&0xFF0000)>>16),
	    (int)((client_addr.sin_addr.s_addr&0xFF000000)>>24));

    printf("receive[%s]\n",ip);

    sprintf(buff_snd,"[%s] %s",ip,buff_rcv);
    write(client_socket,buff_snd,strlen(buff_snd)+1);
    sleep(1);
    //    close(client_socket);
  }
  return 0;
}

void *thread_main(void *arg){  
  //  while(1){
  //  }
  pthread_exit((void *)0);
}
