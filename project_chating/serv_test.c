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

// User 구조체 선언
typedef struct User
{
	char id[30];
	char pw[30];
	char phone[30];
	char email[30];
	char question[200];
	char answer[100];
	char nick_name[30];
	int h;

} User;
// 구조체 변수 선언
	User user[50] = {{"id001","pw1","010-0000-0001","email1","질문","답변","닉네임", 1}};

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
	char choice[] = {"번호를 선택해주세요\n1.로그인 2.회원가입 3.아이디찾기 4.비밀번호찾기\n"};

	while(1)
	{
		write(clnt_sock, choice, strlen(choice));
		str_len = read(clnt_sock, msg, sizeof(msg));
		// strcmp 함수를 쓰기위해 msg 마지막값에 널문자를 넣어준다.
		msg[str_len-1]= '\0';
		// strcmp 첫번째인자와 두번째인자가 같다면 0
		// 1.로그인
		if(strcmp(msg,"1") == 0)
		{
		// 	write(clnt_sock, id, strlen(id));
		// 	str_len = read(clnt_sock, msg, sizeof(msg));
		// 	msg[str_len-1] = '\0';
		// 	printf("%d\n", str_len);
		// 	printf("%s\n", msg);
		// 	for(i=0; i<10; i++)
		// 	{
		// 		// 아이디가 일치하는 경우
		// 		if(strcmp(msg,user[i].id) == 0)
		// 		{
					
		// 		}
				
		// 	}
		// 	// 아이디가 일치하지 않는 경우
		// 	if(strcmp(msg,user[i].id) != 0)
		// 	{
		// 		write(clnt_sock, f_id, strlen(f_id));
		// 		continue;
		// 	}
			
		}
		// 회원가입
		else if(strcmp(msg,"2") == 0)
		{
			while(1)
			{
				// 회원가입 아이디입력
				write(clnt_sock, "아이디를 입력하세요(4~15글자)\n", strlen("아이디를 입력하세요(4~15글자)\n"));
				str_len = read(clnt_sock, msg, sizeof(msg));
				msg[str_len-1] = '\0';
				printf("입력문자:%s\n", msg);
				for(int j=0; j<100; j++)
				{
					printf("j값:%d\n", j);
					if(strlen(msg) < 4 || strlen(msg) > 15)
					{
						write(clnt_sock, "글자수가 맞지 않습니다.다시 입력해주세요\n", strlen("글자수가 맞지 않습니다.다시 입력해주세요\n"));
						break;
					}
					else if(strcmp(user[j].id, msg) == 0)
					{
						write(clnt_sock, "이미 사용하고 있는 아이디입니다\n", strlen("이미 사용하고 있는 아이디입니다\n"));
				
					}
					else
					{
						// 아이디 배열에 저장
						strcpy(user[j].id , msg);
						// h 값을 1로 바꿈.
						user[j].h = 1;
						printf("%d\n", j);
						printf("%s\n", user[j].id);
						break;
					}
				}

				// 회원가입 비밀번호 입력
				// write(clnt_sock, "비밀번호를 입력하세요(4~15글자)\n", strlen("비밀번호를 입력하세요(4~15글자)\n"));
				// str_len = read(clnt_sock, msg, sizeof(msg));
				// msg[str_len-1] = '\0';
				// for(i=0; 0<5; i++)
				// {
				// 	if(user[i].h == 0)
				// 	{
						
				// 		strcpy(user[i].pw, msg);
				// 		user[i].h = 1;
				// 		break;
				// 	}
				// }
			}
			
		}
		
	}
    
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