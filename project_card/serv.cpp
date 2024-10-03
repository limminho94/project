#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <sstream>              // ostringstream 사용을 위해 추가
#include "maria_db_connector.h" // MariaDB 연결 헤더

constexpr int BUF_SIZE = 1024;

MariaDBConnector db_connector; // MariaDB 연결 객체

void handle_client(int client_socket)
{
    char buffer[BUF_SIZE];

    // 클라이언트로부터 받은 쿼리 처리 및 일반 메시지 처리
    while (true)
    {
        ssize_t bytes_received = recv(client_socket, buffer, BUF_SIZE, 0);
        if (bytes_received <= 0)
        {
            std::cout << "클라이언트가 연결을 끊었거나 오류가 발생했습니다.\n";
            close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0'; // 받은 데이터를 null로 종료
        std::string buffer_str(buffer);

        // 받은 메시지 디버그 출력
        std::cout << "받은 메시지: " << buffer_str << std::endl;

        if (buffer_str.compare(0, 6, "query:") == 0)
        {
            // "query:" 이후의 문자만 추출
            std::string query_str = buffer_str.substr(6); // "query:" 이후의 모든 문자열
            std::cout << "처리할 쿼리: " << query_str << std::endl;

            // 받은 메시지를 쿼리로 처리하여 결과 전송
            if (db_connector.query(query_str.c_str()))
            {
                // 쿼리 성공시 결과 출력 후 클라이언트로 전송
                std::ostringstream oss;
                oss << "쿼리 성공" << std::endl
                    << db_connector.getQueryResult();
                std::string resultQuery = oss.str();
                send(client_socket, resultQuery.c_str(), resultQuery.size(), 0);
            }
            else
            {
                // 쿼리 실패 시 실패 메시지 전송
                std::string result = "쿼리 실패\n";
                send(client_socket, result.c_str(), result.size(), 0);
            }
        }
        else
        {
            // 일반 메시지 처리
            std::string response = "서버가 받은 메시지: " + buffer_str + "\n";
            std::cout << "일반 메시지: " << buffer_str << std::endl; // 로그로 출력
            send(client_socket, response.c_str(), response.size(), 0); // 클라이언트에 다시 메시지 전달
        }
    }
}

int main()
{
    // 포트 입력 받기
    int port;
    std::cout << "사용할 포트를 입력하세요: ";
    std::cin >> port;

    // 포트 유효성 검사
    if (std::cin.fail() || port <= 0 || port > 65535)
    {
        std::cerr << "유효하지 않은 포트 번호입니다. 1과 65535 사이의 값을 입력하세요.\n";
        return 1;
    }

    // MariaDB 연결 초기화
    const char *host = "10.10.20.116";
    const char *user = "dbd";
    const char *password = "1234";
    int db_port = 3306;
    const char *database = "card_project";

    db_connector.initialize(); // MariaDB 드라이버 초기화

    if (!db_connector.connect(host, user, password, db_port))
    {
        std::cerr << "MariaDB 연결 실패\n";
        return 1;
    }

    // 서버 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "소켓 생성에 실패했습니다.\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // 바인드
    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "바인딩에 실패했습니다.\n";
        close(server_socket);
        return 1;
    }

    // 리스닝 설정
    if (listen(server_socket, 10) < 0)
    {
        std::cerr << "리스닝에 실패했습니다.\n";
        close(server_socket);
        return 1;
    }

    std::cout << "서버가 포트 " << port << "에서 리스닝 중입니다...\n";

    // 클라이언트 연결 수락 및 처리
    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            std::cerr << "클라이언트 연결 수락에 실패했습니다.\n";
            continue;
        }

        std::cout << "클라이언트가 연결되었습니다.\n";
        std::thread(handle_client, client_socket).detach();
    }

    // 서버 종료 시 자원 해제
    db_connector.finalize();
    return 0;
}
