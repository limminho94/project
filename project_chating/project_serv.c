#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 150
#define MAX_CLNT 256
#define USER_NUM 100

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
	char question[100];
	char answer[100];
	char nick_name[30];
	int already;	// 회원가입하면 1
	int login;	// 로그인하면 1

} User;
	// 구조체 변수 선언
	User user[USER_NUM] = {{"id001","pw001","01012345678","email1","당신의 고향은","광주","멸치", 1 ,},{"id002","pw002","01012345677","email2","당신의 이름은","개똥이","고등어", 1, } };
	
	// 회원가입 저장 변수 초기화
	int user_cnt = 0;

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
	int str_len=0, i, j, chk;
	
	char msg[BUF_SIZE];
	char login_msg[BUF_SIZE];
	char id_msg[BUF_SIZE];
	char pw_msg[BUF_SIZE];
	char pw_chk[BUF_SIZE];
	char phone_msg[BUF_SIZE];
	char email_msg[BUF_SIZE];
	char question_msg[BUF_SIZE];
	char answer_msg[BUF_SIZE];
	char nick_msg[BUF_SIZE];
	char choice[] = {"번호를 선택해주세요\n1.로그인 2.회원가입 3.아이디찾기 4.비밀번호찾기\n"};

	
	while(1)
	{
		write(clnt_sock, choice, strlen(choice));
		str_len=read(clnt_sock, msg, sizeof(msg));
		msg[str_len-1]= '\0';
		// 1.로그인
		if(strcmp(msg,"1") == 0)
		{
			while(1)
			{
				write(clnt_sock, "아이디를 입력하세요\n", strlen("아이디를 입력하세요\n"));
				(str_len=read(clnt_sock, id_msg, sizeof(id_msg)));
				id_msg[str_len-1] = '\0';
				// printf("입력아이디:%s\n", id_msg);
				write(clnt_sock, "비밀번호를 입력하세요\n", strlen("비밀번호를 입력하세요\n"));
				(str_len=read(clnt_sock, pw_msg, sizeof(pw_msg)));
				pw_msg[str_len-1] = '\0';
				// printf("입력비밀번호:%s\n", pw_msg);
				chk = 0;
				for(j=0; j<USER_NUM; j++)
				{
					if(strcmp(id_msg,user[j].id) == 0 && strcmp(pw_msg, user[j].pw) == 0 && user[j].login == 1)
					{
						write(clnt_sock, "이미 접속중인 아이디입니다\n", strlen("이미 접속중인 아이디입니다\n"));
						chk = 1;
						break;
					}
					else if(strcmp(id_msg,user[j].id) == 0 && strcmp(pw_msg, user[j].pw) == 0 && user[j].login != 1)
					{
						write(clnt_sock, "로그인성공!\n", strlen("로그인성공!\n"));
						user[j].login = 1;
						printf("로그인상태:%d\n", user[j].login);
						printf("아이디:%s\n", user[j].id);
						break;
					}
					else if(j == 99)
					{
						write(clnt_sock, "존재하지않는 아이디나 비밀번호입니다.다시 입력해주세요\n", strlen("존재하지않는 아이디나 비밀번호입니다.다시 입력해주세요\n"));
						chk = 1;
						break;
					}
				}
				if(chk == 1)
				{
					continue;
				}
				break;
			}
			break;
		}
		// 2.회원가입
		else if(strcmp(msg,"2") == 0)
		{
			// 아이디
			write(clnt_sock, "아이디를 입력하세요(4~15글자)\n", strlen("아이디를 입력하세요(4~15글자)\n"));
			while((str_len=read(clnt_sock, id_msg, sizeof(id_msg)))!=0)
			{
				id_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", id_msg);
				chk = 0;
				for(j=0; j<USER_NUM; j++)
				{
					if(strlen(id_msg) < 4 || strlen(id_msg) > 15)
					{
						write(clnt_sock, "글자수가 맞지 않습니다.다시 입력해주세요\n", strlen("글자수가 맞지 않습니다.다시 입력해주세요\n"));
						chk = 1;
						break;
					}
					else if(strcmp(user[j].id, id_msg) == 0)
					{
						write(clnt_sock, "이미 사용하고 있는 아이디입니다.다시 입력해주세요\n", strlen("이미 사용하고 있는 아이디입니다.다시 입력해주세요\n"));
						chk = 1;
						break;
					}
					
				}
				if(chk == 1)
				{
					continue;
				}
				break;
	
			}
			
			// 비밀번호
			write(clnt_sock, "비밀번호를 입력하세요(4~15글자)\n", strlen("비밀번호를 입력하세요(4~15글자)\n"));
			while((str_len=read(clnt_sock, pw_msg, sizeof(pw_msg)))!=0)
			{
				pw_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", pw_msg);
				strcpy(pw_chk, pw_msg);
				chk = 0;
				if(strlen(pw_msg) < 4 || strlen(pw_msg) > 15)
				{
					write(clnt_sock, "글자수가 맞지 않습니다.다시 입력해주세요\n", strlen("글자수가 맞지 않습니다.다시 입력해주세요\n"));
					chk = 1;
					break;
				}
				else
				{
					chk = 0;
					while(chk == 0)
					{
						write(clnt_sock, "한번 더 입력하세요\n", strlen("한번 더 입력하세요\n"));
						str_len = read(clnt_sock, pw_msg, sizeof(pw_msg));
						pw_msg[str_len-1] = '\0';
						
						// printf("비밀번호중복확인 입력메세지:%s\n", pw_chk);
						if(strcmp(pw_chk, pw_msg) == 0)
						{
							break;
						}
						else
						{
							write(clnt_sock, "입력된 비밀번호가 같지않습니다. 다시입력해주세요\n", strlen("입력된 비밀번호가 같지않습니다. 다시입력해주세요\n"));
							continue;
						}
					}
				}
				if(chk == 1)
				{
					continue;
				}
				break;
			}

			// 휴대폰번호
			write(clnt_sock, "휴대폰번호를 입력하세요(-제외)\n", strlen("휴대폰번호를 입력하세요(-제외)\n"));
			while ((str_len=read(clnt_sock, phone_msg, sizeof(phone_msg)))!=0)
			{
				phone_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", phone_msg);
				chk = 0;
				for(j=0; j<USER_NUM; j++)
				{
					if(strlen(phone_msg) != 11)
					{
						write(clnt_sock, "사용할 수 없는 번호입니다.다시 입력해주세요\n", strlen("사용할 수 없는 번호입니다.다시 입력해주세요\n"));
						chk = 1;
						break;
					}
					else if(strcmp(user[j].phone, phone_msg) == 0)
					{
						write(clnt_sock, "이미 사용하고 있는 번호입니다.다시 입력해주세요\n", strlen("이미 사용하고 있는 번호입니다.다시 입력해주세요\n"));
						chk = 1;
						break;
					}
					
				}
				if(chk == 1)
				{
					continue;
				}
				break;
			}

			// 이메일
			write(clnt_sock, "이메일을 입력하세요\n", strlen("이메일을 입력하세요\n"));
			while ((str_len=read(clnt_sock, email_msg, sizeof(email_msg)))!=0)
			{
				email_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", email_msg);
				break;
			}

			// 질문
			write(clnt_sock, "비밀번호 찾기 시 질문을 작성하세요(15글자이내로)\n", strlen("비밀번호 찾기 시 질문을 작성하세요(15글자이내로)\n"));
			while((str_len=read(clnt_sock, question_msg, sizeof(question_msg)))!=0)
			{
				question_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", question_msg);
				if(strlen(question_msg) > 45 )
				{
					write(clnt_sock, "글자수를 초과하셨습니다. 다시 입력해주세요\n", strlen("글자수를 초과하셨습니다. 다시 입력해주세요\n"));
					continue;
				}
				break;
			}

			// 답변
			write(clnt_sock, "비밀번호 찾기 시 답변을 작성하세요(15글자이내로)\n", strlen("비밀번호 찾기 시 답변을 작성하세요(15글자이내로)\n"));
			while((str_len=read(clnt_sock, answer_msg, sizeof(answer_msg)))!=0)
			{
				answer_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", answer_msg);
				if(strlen(answer_msg) > 45 )
				{
					write(clnt_sock, "글자수를 초과하셨습니다. 다시 입력해주세요\n", strlen("글자수를 초과하셨습니다. 다시 입력해주세요\n"));
					continue;
				}
				break;
			}

			// 닉네임
			write(clnt_sock, "사용하실 닉네임을 입력하세요(7글자이내로)\n", strlen("사용하실 닉네임을 입력하세요(7글자이내로)\n"));
			while((str_len=read(clnt_sock, nick_msg, sizeof(nick_msg)))!=0)
			{
				nick_msg[str_len-1] = '\0';
				// printf("입력메세지:%s\n", nick_msg);
				if(strlen(nick_msg) > 23 )
				{
					write(clnt_sock, "글자수를 초과하셨습니다. 다시 입력해주세요\n", strlen("글자수를 초과하셨습니다. 다시 입력해주세요\n"));
					continue;
				}
				break;
			}
			
			// 회원가입정보 저장
			pthread_mutex_lock(&mutx);
			strcpy(user[user_cnt].id, id_msg);
			strcpy(user[user_cnt].pw, pw_msg);
			strcpy(user[user_cnt].phone, phone_msg);
			strcpy(user[user_cnt].email, email_msg);
			strcpy(user[user_cnt].question, question_msg);
			strcpy(user[user_cnt].answer, answer_msg);
			strcpy(user[user_cnt].nick_name, nick_msg);
			// printf("저장된 아이디,비밀번호,휴대폰번호,이메일,질문,답변,닉네임: %s %s %s %s %s %s %s\n", user[0].id, user[0].pw, user[0].phone, user[0].email, user[0].question, user[0].answer, user[0].nick_name);
			user_cnt++;
			pthread_mutex_unlock(&mutx);
			
			continue;
		}
		
		// 3.아이디찾기
		else if(strcmp(msg,"3") == 0)
		{
			write(clnt_sock, "휴대폰번호를 입력하세요\n", strlen("휴대폰번호를 입력하세요\n"));
			while((str_len=read(clnt_sock, phone_msg, sizeof(phone_msg)))!=0)
			{
				phone_msg[str_len-1] = '\0';
				chk = 0;
				for(j=0; 0<USER_NUM; j++)
				{
					if(strcmp(phone_msg, user[j].phone) == 0)
					{
						memset(msg, 0, sizeof(msg));
						sprintf(msg, "아이디는 %s입니다.\n", user[j].id);
						write(clnt_sock, msg, strlen(msg));
						break;
					}
					else if(j == 99)
					{
						write(clnt_sock, "다시 입력해주세요\n", strlen("다시 입력해주세요\n"));
						chk = 1;
						break;
					}
				}
				if(chk == 1)
				{
					continue;
				}
				break;
			}

		}

		// 비밀번호찾기
		else if(strcmp(msg,"4") == 0)
		{
			while(1)
			{	
				write(clnt_sock, "아이디를 입력하세요\n", strlen("아이디를 입력하세요\n"));
				str_len=read(clnt_sock, id_msg, sizeof(id_msg));
				id_msg[str_len-1] = '\0';
				chk = 0;
				for(j=0; 0 <USER_NUM; j++)
				{	
					if(strcmp(id_msg, user[j].id) == 0)
					{
						memset(msg, 0, sizeof(msg));
						sprintf(msg, "질문: %s?\n", user[j].question);
						write(clnt_sock, msg, strlen(msg));
						break;
					}
					else if(j == 99)
					{
						write(clnt_sock, "아이디가 맞지않습니다\n", strlen("아이디가 맞지않습니다\n"));
						chk = 1;
						break;
						
					}
				}
				if(chk == 1)
				{
					continue;
				}
				while(1)
				{
					write(clnt_sock, "답을 입력하세요\n", strlen("답을 입력하세요\n"));
					str_len=read(clnt_sock, answer_msg, sizeof(answer_msg));
					answer_msg[str_len-1] = '\0';
					chk = 0;
					for(j=0; 0 <USER_NUM; j++)
					{	
						if(strcmp(answer_msg, user[j].answer) == 0)
						{
							memset(msg, 0, sizeof(msg));
							sprintf(msg, "정답입니다! 비밀번호는 %s입니다.\n", user[j].pw);
							write(clnt_sock, msg, strlen(msg));
							break;
						}
						else if(j == 99)
						{
							write(clnt_sock, "틀렸습니다\n", strlen("틀렸습니다\n"));
							chk = 1;
							break;
							
						}

					}
					if(chk == 1)
					{
						continue;
					}
					break;
				}
				break;
			}
			
		}
	}
    //-----------------로그인,회원가입,아이디,비밀번호찾기 끝-------------------------


	//-------------------------------------------------------------------------
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
	if(user[i].login == 1)
	{
		write(clnt_socks[i], msg, len);
	}
		
	pthread_mutex_unlock(&mutx);
}
// 에러 표시?함수
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}