#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

// 함수 헤더 선언
void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

// 전역 변수 선언
int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

// 메인 함수 실행
int main(int argc, char *argv[])    // argc : argv[]의 개수 | argv[] 문자열의 주소를 저장하는 포인터배열
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
    // 쓰레드 동기화
	pthread_mutex_init(&mutx, NULL);
    // 서버소켓생성
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    // 서버소켓 주소할당
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
    // 연결대기상태 
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
        // 연결요청 수락
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        printf("연결요청수락 clnt_sock:%d\n",clnt_sock);
		// 임계영역 락
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&mutx);
        // 클라이언트 쓰레드 생성
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        // 클라이언트 쓰레드 소멸
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}
	
// 클라이언트 생성 함수
void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
    
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		send_msg(msg, str_len);
	
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
// 메세지 전송 함수
void send_msg(char * msg, int len)   // send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
// 에러 표시?함수
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}