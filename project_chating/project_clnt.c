#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
	
#define BUF_SIZE 100
// #define NAME_SIZE 20
// 함수 헤더 선언
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
// 지역변수 선언
// char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];
// 메인함수 실행
int main(int argc, char *argv[])    // argc argv[]의 인자 개수 | argv[] 문자열의 주소를 저장하는 포인터배열
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }
	// 문자열 출력
	// sprintf(name, "[%s]", argv[3]);
    // 클라이언트 소켓 생성
	sock=socket(PF_INET, SOCK_STREAM, 0);
	// 주소정보 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	// 서버로 연결요청
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	// 쓰레드 생성
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);  
	return 0;
}
// 메세지 보내는 함수
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	
	while(1) 
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) 
		{
			close(sock);
			exit(0);
		}
		// sprintf(name_msg,"%s %s", name, msg);
		write(sock, msg, strlen(msg));
	}
	return NULL;
}
// 메세지 받는 함수
void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	
	int str_len;
	while(1)
	{
		str_len=read(sock, msg, BUF_SIZE-1);
		if(str_len==-1) 
			return (void*)-1;
		msg[str_len]=0;
		fputs(msg, stdout);
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}