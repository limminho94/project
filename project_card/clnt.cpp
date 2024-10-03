#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits> // numeric_limits 사용을 위해 추가
#include <set>
#include <string>

constexpr int BUF_SIZE = 1024;
const char *SERVER_IP = "127.0.0.1";

int main()
{
    // 포트 입력 받기
    int port;
    std::cout << "연결할 서버 포트를 입력하세요: ";
    std::cin >> port;

    // 유효성 검사
    if (std::cin.fail() || port <= 0 || port > 65535)
    {
        std::cerr << "유효하지 않은 포트 번호입니다. 1과 65535 사이의 값을 입력하세요.\n";
        return 1;
    }

    // 입력 버퍼 클리어 (포트 입력 후 남은 개행 제거)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "소켓 생성에 실패했습니다.\n";
        return 1;
    }

    // 서버 주소 설정
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        std::cerr << "유효하지 않은 주소입니다.\n";
        return 1;
    }

    // 서버에 연결
    if (connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "서버 연결에 실패했습니다.\n";
        return 1;
    }

    std::cout << "서버에 연결되었습니다.\n";

    // 유효한 입력값을 모아놓은 set
    std::set<std::string> valid_inputs = {"대중교통", "교통", "쇼핑", "외식", "의료", "주유", "통신", "1", "2", "3", "4", "5", "6"};

    std::string user_input;
    char buffer[BUF_SIZE];

    // 메시지 송수신 루프
    while (true)
    {

        system("clear");
        // std::cout << "서버에 연결되었습니다.\n";

        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⠠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠂⢕⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⡟⠋⠛⠙⠋⠛⠙⠋⠛⠙⠋⠛⠙⠋⠛⠙⠋⠛⠙⠋⠛⢻⣿⣿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⡀⠀⠀⠀⠀⠠⡁⠀⢀⠰⡀⠀⠀⠀⠀⠀⠰⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠂⠀⡀⢄⠄⠀⠀⠂⠁⠀⠀⠀⠀⠀⠀⠸⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⡀⠀⢠⢈⠂⠀⠠⢂⠆⢄⠀⠀⢀⠠⡠⢠⠀⢘⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⡢⡁⠀⠀⠁⠀⠑⢌⠂⠐⠁⠐⠀⠀⠁⢨⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⢀⠆⠀⠢⠢⡀⡀⢀⡀⠪⡐⠅⠀⠀⠀⠀⠀⠀⢰⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣾⠆⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠀⠁⠀⠀⠀⠁⠌⠨⠂⠌⠂⠈⠀⠢⡀⠀⠀⠀⠀⠰⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠲⣾⣻⡵⣿⠅⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢄⠅⠁⠀⠀⢀⠆⠀⠀⠀⠀⠈⠀⠀⠀⠀⢘⣿⣿⡇⠀⠀⠀⠀⠀⢀⣤⣶⡻⣿⠀⠀⢙⡾⣯⡃⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⣿⠿⠃⠀⠀⣠⣴⡗⠙⠳⣽⣝⣿⠀⠀⠐⣿⣻⠆⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⢀⡤⢞⣟⢿⣮⣏⠀⠀⣸⡷⣿⠀⠀⠨⣿⣽⡃⠀⠀⠀⠀⢸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢐⣾⢿⡂⠀⠉⣿⢮⡗⠀⠀⣸⣯⣿⠀⠀⢐⡿⣾⠅⠀⣀⣀⣀⣸⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣻⠅⠀⠀⣿⢽⣏⠀⠀⣸⡷⣻⠀⠀⢐⣿⣽⡃⠀⢼⣿⣿⣿⣿⣿⠅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠉⠀⠀⠀⠉⠈⠁⠀⠀⠈⠉⠉⠀⠀⠀⠁⠈⠀⠀⠈⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣤⣾⣾⣶⣤⢴⣷⡶⣺⣶⡆⠀⠀⠀⠀⠀⢰⣷⣶⠀⠀⢰⣷⣾⠀⠀⠀⠀⠀⠐⣷⣾⣶⣷⣾⣶⣷⣾⣶⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢛⣛⣻⣛⣛⢹⣿⣿⣾⣿⡇⠀⠀⠀⠀⢀⣼⣿⣿⣇⠀⢸⣿⣿⣿⡇⠀⠀⠀⢐⣷⣶⣶⣶⣶⣶⣾⣿⣿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣯⣿⣿⢸⣿⣏⢿⣿⡇⠀⠀⠀⢸⣿⠿⠋⠻⢿⣿⢎⣿⣿⠀⠀⠀⠀⠀⠀⢿⢿⢷⣷⣷⣷⣶⣶⢶⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢩⣽⣭⣭⣭⣬⣯⣣⣽⣝⡄⠀⠀⠀⠨⣶⣶⣶⣶⣶⣶⣶⣷⣷⠀⠀⠀⠀⠀⢿⠿⡷⠿⠿⢿⠿⡶⡷⢿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠛⠛⠛⠛⣿⣿⡇⠀⠀⠀⢐⣷⣷⡷⠷⠷⠷⠾⠿⢿⠁⠀⠀⠀⠀⠠⣿⣾⣦⣄⣤⣤⣠⣤⣄⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
        std::cout << "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠟⠻⠁⠀⠀⠀⠀⠛⠛⠟⠟⠿⠻⠟⠿⠻⠁⠀⠀⠀⠀⠀⠻⠛⠟⠛⠛⠛⠛⠛⠛⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\n";

        // std::cin.get()을 사용하여 아무 키나 입력 받기
        std::cout << "핵살론 카드 추천 서비스에 오신걸 환영합니다.\n";
        std::cout << "계속하려면 아무 키나 누르세요.";

        std::cin.get(); // 사용자로부터 아무 키나 입력받음
        system("clear");
        
        std::cout << "--------------------------------------------------------\t";
        std::cout << "--------------------------------------------------------\n";
        std::cout << "|                 맞춤 혜택으로 직접 찾기               |\t";
        std::cout << "|                    테스트로 추천받기                  |\n";

        std::cout << "|                                                       |\t";
        std::cout << "|                                                       |\n";
        std::cout << "|                                                       |\t";
        std::cout << "|                                                       |\n";
        std::cout << "|                  6가지 상세혜택으로                   |\t";
        std::cout << "|                  소비성향으로 알아보는                |\n";
        std::cout << "|                      1. 카드검색                      |\t";
        std::cout << "|                   2. 카드추천테스트                   |\n";
        std::cout << "--------------------------------------------------------\t";
        std::cout << "--------------------------------------------------------\n";



        // 유효한 입력값을 모아놓은 set
        std::set<std::string> valid_inputs = {"대중교통", "교통", "쇼핑", "외식", "의료", "주유", "통신",
                                              "1", "2", "3", "4", "5", "6"};
        bool inputMode = true;
        
        
        while(inputMode)
        {
            std::cout << "원하는 항목을 입력하세요 (1.카드검색 2.카드추천테스트): ";
            std::getline(std::cin, user_input); // 사용자 입력 받기
            
            if(user_input != "1" && user_input != "2")
            {
                std::cout << "다시 입력해주세요\n";
                continue;
            }
            else if(user_input == "1")
            {
                system("clear");
                std::cout << "1번 출력\n";
                break;
            }
            else if(user_input == "2")
            {
                system("clear");
                std::cout << "2번 출력\n";
                break;
            }

        }

        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\n";

        std::cout << "|           1.대중교통            |\t";
        std::cout << "|           2. 쇼핑              |\t";
        std::cout << "|           3. 외식              |\n";

        std::cout << "|               🚌                |\t";
        std::cout << "|               🛍️                |\t";
        std::cout << "|               🍽️                |\n";

        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\n\n";

        // 두 번째 줄 (4, 5, 6)
        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\n";

        std::cout << "|           4. 의료              |\t";
        std::cout << "|           5. 주유              |\t";
        std::cout << "|           6. 통신              |\n";

        std::cout << "|               🏥               |\t";
        std::cout << "|               ⛽               |\t";
        std::cout << "|               📞               |\n";

        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\t";
        std::cout << "----------------------------------\n";

        // 사용자 입력 반복 루프
        while (inputMode)
        {
            std::string user_input;
            std::string result;
            char buffer[BUF_SIZE];

            std::cout << "원하는 항목을 입력하세요 (대중교통, 교통, 쇼핑, 외식, 의료, 주유, 통신, 또는 1에서 6까지의 숫자): ";
            std::getline(std::cin, user_input); // 사용자 입력 받기

            // 입력값이 유효한지 확인
            if (valid_inputs.find(user_input) != valid_inputs.end())
            {
                // 유효한 입력이면 대응하는 결과를 설정하고 반복 종료
                if (user_input == "대중교통" || user_input == "교통" || user_input == "1")
                {
                    result = "대중교통";
                }
                else if (user_input == "2" || user_input == "쇼핑")
                {
                    result = "쇼핑";
                }
                else if (user_input == "3" || user_input == "외식")
                {
                    result = "외식";
                }
                else if (user_input == "4" || user_input == "의료")
                {
                    result = "의료";
                }
                else if (user_input == "5" || user_input == "주유")
                {
                    result = "주유";
                }
                else if (user_input == "6" || user_input == "통신")
                {
                    result = "통신";
                }   

                system("clear");
                // TOP3 출력
                // "query:" 접두사 추가
                std::string modified_message = "query:SELECT rb.card_name AS 카드명, rb.type AS 유형, rb.corp_name AS 카드사, c.annual AS 연회비, c.month AS 전월실적, CASE WHEN c.bene1_type = '" + result + "' THEN c.bene1 WHEN c.bene2_type = '" + result + "' THEN c.bene2 ELSE '해당 혜택 없음' END AS 혜택 FROM card_project.Rank_Benefit rb JOIN card_project.Card c ON rb.card_name = c.name WHERE rb.type = '" + result + "'";

                send(client_socket, modified_message.c_str(), modified_message.length(), 0);
                std::cout << std::endl;

                ssize_t bytes_received = recv(client_socket, buffer, 10000, 0);
                if (bytes_received <= 0)
                {
                    std::cerr << "서버가 연결을 끊었거나 오류가 발생했습니다.\n";
                    break;
                }

                buffer[bytes_received] = '\0';
                std::cout << buffer << std::endl;

                std::cout << "===============================================================================";
                // "query:" 접두사 추가
                modified_message = "query:SELECT name AS '카드명', corp_name AS '카드사', annual AS '연회비', month AS '전월실적', bene1 AS '혜택1', bene2 AS '혜택2', bene1_type AS '혜택타입1', bene2_type AS '혜택타입2' FROM card_project.Card WHERE name NOT IN(SELECT card_name FROM card_project.Rank_Benefit WHERE type = '" + result + "') AND (bene1_type ='" + result + "' OR bene2_type ='" + result + "')";

                send(client_socket, modified_message.c_str(), modified_message.length(), 0);
                std::cout << std::endl;

                bytes_received = recv(client_socket, buffer, 10000, 0);
                if (bytes_received <= 0)
                {
                    std::cerr << "서버가 연결을 끊었거나 오류가 발생했습니다.\n";
                    break;
                }

                buffer[bytes_received] = '\0';
                system("clear");
                std::cout << buffer << std::endl;

                // 다시 시작 또는 종료 여부 확인
                std::cout << "다시 시작하려면 Enter를, 종료하려면 'exit'를 입력하세요: ";
                std::getline(std::cin, user_input);

                if (user_input == "exit")
                {
                    break; // 프로그램 종료
                }

                inputMode = false;
                break; // 반복 종료
            }
            else
            {
                // 유효하지 않은 입력이면 다시 요청
                std::cout << "잘못된 입력입니다. 다시 시도해주세요." << std::endl;
            }
        }
        // 일반 메시지 전송 모드로 전환
        // 일반 메시지 전송 모드로 전환
        while (!inputMode)
        {
            system("clear");
            std::cout << "서버로 보낼 메시지를 입력하세요 ('exit'을 입력하면 종료합니다): ";
            std::getline(std::cin, user_input);

            // 빈 입력에 대한 처리: 사용자가 엔터만 입력하면 다시 입력 요구
            if (user_input.empty())
            {
                std::cout << "입력 값이 비어 있습니다. 다시 입력해주세요.\n";
                continue; // 루프의 처음으로 돌아가 다시 입력 받음
            }

            // 'exit' 입력 시 일반 메시지 모드 종료
            if (user_input == "exit")
            {
                break; // 일반 메시지 모드 종료
            }

            // 일반 메시지 전송
            if (send(client_socket, user_input.c_str(), user_input.length(), 0) == -1)
            {
                std::cerr << "메시지 전송에 실패했습니다.\n";
                return 1;
            }
            std::cout << "메시지를 전송했습니다: " << user_input << std::endl;

            // 서버로부터의 응답 수신
            ssize_t bytes_received = recv(client_socket, buffer, BUF_SIZE - 1, 0);
            if (bytes_received <= 0)
            {
                std::cerr << "서버가 연결을 끊었거나 오류가 발생했습니다.\n";
                return 1;
            }

            buffer[bytes_received] = '\0'; // 받은 데이터를 문자열로 변환
            std::cout << "서버로부터: " << buffer << std::endl;
        }
        break;
    }

    // 클라이언트 소켓 종료
    close(client_socket);
    return 0;
}
