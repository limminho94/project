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
#define ID 10
#define PW 10


typedef struct USEID
{
	char u_id[10];
	char u_pw[10];
	char u_name[13];
	char phone[10];
	int not;   /// 아이디 등록여부
	int u_num;
}USE_ID;

USE_ID UID[10]={{"aaa","11","하이하이","11111111",1},{"bbb","111","키키키","22222222",1}};


void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);
int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;
void log_in(void* arg);


int main(int argc, char *argv[])
{
	
	int str_len=0, i;
	char msg[BUF_SIZE];
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	



	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	pthread_mutex_init(&mutx, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++]=clnt_sock;
		UID[clnt_cnt++].u_num=clnt_sock;
		pthread_mutex_unlock(&mutx);
		
		
		
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}
	
void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, a=1;
	char msg[BUF_SIZE];
	char idwant[]="아이디를 입력하세요\n";
	char pwwant[]="비밀번호를 입력하세요\n";
	char fa[]="아이디또는 비밀번호가 틀립니다.\n";
	char Id[10]; char Pw[10];
	
		
			
	while(a)
	{

		write(clnt_sock,"로그인(1  비밀번호찾기(2  회원가입(3\n",strlen("로그인(1  비밀번호찾기(2  회원가입(3\n"));
		str_len=read(clnt_sock, msg, sizeof(msg));
		msg[str_len-1]= '\0'; 
			if(strcmp(msg,"1")==0)
			{while(a)
			{
			write(clnt_sock, idwant, sizeof(idwant));
			str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
			msg[str_len-1]= '\0';
			strcpy(Id,msg);
			printf("%s",Id);

			write(clnt_sock, pwwant, sizeof(pwwant));
			str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
			msg[str_len-1]= '\0';
			strcpy(Pw,msg);
			printf("%s",Pw);

			for(int j=0; j<10;j++)
			{
			if(strcmp(Id,UID[j].u_id)==0 && strcmp(Pw,UID[j].u_pw)==0)
			{
			write(clnt_sock,"로그인성공\n",strlen("로그인성공\n"));
			a=0;
			break; 
			}
			else if(j == 10)
			exit(0);
			} // 아이디 확인용 반복문
			} // 아이디 재입력 반복문
			} // 1번 if 로그인 끝나는 괄호
			else if(strcmp(msg,"2")==0) /// 2번 비밀 번호 찾기
			{
				write(clnt_sock, "아이디를 입력하세요\n", sizeof("아이디를 입력하세요\n"));
				str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
				msg[str_len-1]= '\0';
				strcpy(Id,msg);
				write(clnt_sock, "핸드폰 뒷 8자리를 -없이 입력하세요\n", sizeof("핸드폰 뒷 8자리를 -없이 입력하세요\n"));
				str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
				msg[str_len-1]= '\0';
				for(int j=0; j<10;j++)  /// 아이디 확인용 반복문
				{	if(strcmp(Id,UID[j].u_id)==0 && strcmp(msg,UID[j].phone)==0){
					sprintf(msg,"비밀번호는 %s 입니다.\n",UID[j].u_pw);
					write(clnt_sock, msg, sizeof(msg));} ///  비밀번호 보내기

				}
			} /// 2번 if 끝나는 괄호
			else if(strcmp(msg,"3")==0)
			{
				while(a)
				{
				write(clnt_sock,"아이디를 입력하세요\n",sizeof("아이디를 입력하세요\n"));
				str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
				msg[str_len-1]= '\0';
				for(int j=0; j<10; j++)
				{
					if(strcmp(msg,UID[j].u_id)!=0 && UID[j].not == 0)
					{
						strcpy(UID[j].u_id,msg);

						write(clnt_sock,"비밀번호를 입력하세요\n",sizeof("비밀번호를 입력하세요\n"));
						str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
						msg[str_len-1]= '\0';
						strcpy(Pw,msg); // 첫번째 비밀번호 저장
						write(clnt_sock,"비밀번호를 다시 입력하세요\n",sizeof("비밀번호를 다시 입력하세요\n"));
						str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
						msg[str_len-1]= '\0';
						if(strcmp(msg,Pw)==0)  // 비밀번호 2개가 맞으면 저장
						{strcpy(UID[j].u_pw,msg);
						write(clnt_sock,"가입이 완료되었습니다.\n",sizeof("가입이 완료되었습니다.\n"));
						a=0;
						break;
						}
						else {
						write(clnt_sock,"잘못된 입력입니다.\n",sizeof("잘못된 입력입니다.\n"));
						break;
						}
					}
					
				}
				
				
				}

			}


	}

	
		

	
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		send_msg(msg, str_len);
	
	pthread_mutex_lock(&mutx);
	for(int i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(clnt_sock==clnt_socks[clnt_cnt])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[clnt_cnt]=clnt_socks[clnt_cnt+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_msg(char * msg, int len)   // send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}


// void log_in(void* arg)
// {
// 	int clnt_sock=*((int*)arg);
// 	int str_len=0, i,a=1;
// 	char msg[BUF_SIZE];
// 	char idwant[]="아이디를 입력하세요";
// 	char pwwant[]="비밀번호를 입력하세요";
// 	char fa[]="아이디또는 비밀번호가 틀립니다.";
// 	char Id[10]; char Pw[10];

// 		write(clnt_socks[i],"로그인(1  아이디찾기(2  회원가입(3\n",strlen("로그인(1  아이디찾기(2  회원가입(3\n"));
// 		msg[str_len-1]= '\0';
// 			if(strcmp(msg,"1")==0)
// 			{while(1)
// 			{
// 			write(clnt_socks[i], idwant, sizeof(idwant));
// 			str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
// 			msg[str_len-1]= '\0';
// 			strcpy(Id,msg);

// 			write(clnt_socks[i], idwant, sizeof(idwant));
// 			str_len=read(clnt_sock, msg, sizeof(msg));  /// 입력메세지
// 			msg[str_len-1]= '\0';
// 			strcpy(Pw,msg);

// 			for(int j=0; j<10;j++)
// 			{
// 			if(strcmp(Id,UID[j].u_id)==0 && strcmp(Pw,UID[j].u_pw)==0)
// 			{
// 			write(clnt_socks[i],"로그인성공\n",strlen("로그인성공\n"));
// 			break; 
// 			}
// 			else if(j == 10)
// 			exit(0);
// 			}
// 			}
// 			}
// }