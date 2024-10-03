#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
#include <unistd.h>
#include <limits> // numeric_limits 사용을 위해 추가
#include <set>
#include <string>
#include <sstream>
#include <vector>
#define RED "\033[31m"
#define RED_BOLD "\033[1;31m"
#define ORANGE "\033[38;5;214m"
#define ORANGE_BOLD "\033[1;38;5;214m"
#define YELLOW "\033[33m"
#define YELLOW_BOLD "\033[1;33m"
#define GREEN "\033[32m"
#define GREEN_BOLD "\033[1;32m"
#define BLUE "\033[34m"
#define BLUE_BOLD "\033[1;34m"
#define PURPLE "\033[35m"
#define PURPLE_BOLD "\033[1;35m"
#define PINK "\033[38;5;213m"
#define PINK_BOLD "\033[1;38;5;213m"
#define BROWN "\033[38;5;130m"
#define BROWN_BOLD "\033[1;38;5;130m"
#define LBLUE "\033[38;5;153m" // 연한 파란색
#define LBLUE_BOLD "\033[1;38;5;153m"
#define LGREEN "\033[38;5;154m" // 연한 초록색
#define LGREEN_BOLD "\033[1;38;5;154m" // 연한 초록색
#define END "\033[0m"


constexpr int BUF_SIZE = 1024;
const char *SERVER_IP = "127.0.0.1";

class Card
{
private:
    std::string name;           // 카드명
    std::string company;        // 카드사
    std::string annualFee;      // 연회비
    std::string preMonSpending; // 전월 실적
    std::string benetype1;      // 혜택1 타입
    std::string bene1;          // 혜택1 상세
    std::string benetype2;      // 혜택2 타입
    std::string bene2;          // 혜택2 상세

public:
    // 생성자
    Card(const std::string &name, const std::string &company, const std::string &annualFee,
         const std::string &previousMonthSpending, const std::string &benetype1, const std::string &bene1, const std::string &benetype2, const std::string &bene2)
        : name(name), company(company), annualFee(annualFee),
          preMonSpending(previousMonthSpending), benetype1(benetype1), bene1(bene1), benetype2(benetype2), bene2(bene2) {}

    // 접근 메서드
    std::string getName() const { return name; }
    std::string getCompany() const { return company; }
    std::string getAnnualFee() const { return annualFee; }
    std::string getPreviousMonthSpending() const { return preMonSpending; }

    // 필터링 메서드: 사용자가 선택한 혜택과 일치하는지 확인
    bool isBenefitMatch(const std::vector<std::string> &userBenefits) const
    {
        return std::find(userBenefits.begin(), userBenefits.end(), benetype1) != userBenefits.end() ||
               std::find(userBenefits.begin(), userBenefits.end(), benetype2) != userBenefits.end();
    }

    // 카드 정보 출력 함수
    void printCardInfo() const
    {
        if(company.compare("KB국민카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << YELLOW_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        else if(company.compare("신한카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << BLUE_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        else if(company.compare("롯데카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << RED_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        else if(company.compare("삼성카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << PINK_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        else if(company.compare("우리카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << ORANGE_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        else if(company.compare("하나카드") == 0)
        {
            std::cout << "\n-----------------------------------------------------------------------------------------------\n" << PURPLE_BOLD << name <<END << '\t'
                << GREEN_BOLD << company << END << '\n'
                << BROWN_BOLD << "["  << benetype1 
                << "] " << bene1  << '\t'
                << " [" << benetype2 
                << "] " << bene2 << END << '\n'
                << LGREEN_BOLD << "연회비 " << '\t'
                <<  annualFee << END <<'\t'
                << LBLUE_BOLD << " 전월실적 " << '\t'
                << preMonSpending << END << "\n-----------------------------------------------------------------------------------------------\n" 
                << std::endl;
        }
        
        
    }
};

// 문자열을 구분자로 분리하는 함수
std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

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

    bool inputMode = false;
    bool sqlMode = true;

    std::vector<Card> cards; // Card 객체를 저장할 벡터
    std::string user_input;

    // 메시지 송수신 루프
    while (true)
    {
        /* 0. 카드 클래스 생성 및 데이터 가져오기 */
        while (!inputMode && sqlMode)
        {
            char sqlBuffer[10000]; // recv를 위한 버퍼

            // 카드사 목록
            std::vector<std::string> card_companies = {"KB국민카드", "신한카드", "우리카드", "하나카드", "롯데카드", "삼성카드"};

            for (const auto &company : card_companies)
            {
                // 쿼리 문자열 생성
                std::string modified_message = "query:SELECT name AS '카드명', corp_name AS '카드사', annual AS '연회비', month AS '전월실적', "
                                               "bene1_type AS '혜택1', bene1 AS '혜택1 상세', bene2_type AS '혜택2', bene2 AS '혜택2 상세' "
                                               "FROM card_project.Card WHERE corp_name = '" +
                                               company + "'";

                // 서버에 쿼리 전송
                send(client_socket, modified_message.c_str(), modified_message.length(), 0);

                // 서버에서 응답 수신
                ssize_t bytes_received = recv(client_socket, sqlBuffer, sizeof(sqlBuffer) - 1, 0);
                if (bytes_received <= 0)
                {
                    std::cerr << "서버가 연결을 끊었거나 오류가 발생했습니다.\n";
                    break;
                }

                sqlBuffer[bytes_received] = '\0'; // 문자열 종료 추가

                // 문자열 스트림을 사용해 한 줄씩 데이터를 읽고 처리
                std::stringstream ss(sqlBuffer);
                std::string line;

                while (std::getline(ss, line))
                {
                    if (!line.empty())
                    {
                        std::vector<std::string> parts = split(line, '_');
                        if (parts.size() == 8)
                        {
                            Card card(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7]);
                            cards.push_back(card);
                        }
                    }
                }
            }

            // 모든 Card 객체의 정보 출력
            for (const auto &card : cards)
            {
                card.printCardInfo();
            }

            sqlMode = false; // sqlMode 종료
        }

        /* 1. 메인 화면 */
        while (!inputMode && !sqlMode)
        {
            // 1-1. 인트로 화면
            system("clear");

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

            // 1-2.메뉴 선택 화면
            char buffer[BUF_SIZE];
            bool menu = true;
            int menuIdx = 0; // 0 : 메뉴 선택, 1 : 카드 검색, 2: 카드 추천
            std::set<std::string> valid_inputs;
            while (menu)
            {
                while (menuIdx == 0) // 메뉴 선택
                {
                    system("clear");
                    
                    valid_inputs = {"검색", "카드 검색", "카드검색", "1", "추천", "카드 추천", "카드추천", "2"}; // 유효한 입력값을 모아놓은 set

                    std::cout << "--------------------------------------------------------\t";
                    std::cout << "--------------------------------------------------------\n";
                    std::cout << "|                 "<<YELLOW<<"맞춤 혜택으로 직접 찾기"<<END"               |\t";
                    std::cout << "|                    "<<YELLOW<<"테스트로 추천받기"<<END"                  |\n";
                    std::cout << "|                                                       |\t";
                    std::cout << "|                                                       |\n";
                    std::cout << "|                                                       |\t";
                    std::cout << "|                                                       |\n";
                    std::cout << "|                  "<<PURPLE<<"6가지 상세혜택으로"<<END"                   |\t";
                    std::cout << "|                  "<<PURPLE<<"소비성향으로 알아보는"<<END"                |\n";
                    std::cout << "|                      " << GREEN <<"1. 카드 검색"<< END"                     |\t";
                    std::cout << "|                   " << GREEN <<"2. 카드 추천 테스트"<< END"                 |\n";
                    std::cout << "--------------------------------------------------------\t";
                    std::cout << "--------------------------------------------------------\n";

                    while (true)
                    {
                        std::cout << "메뉴를 선택하세요 : ";
                        std::getline(std::cin, user_input); // 사용자 입력 받기

                        if (valid_inputs.find(user_input) != valid_inputs.end())
                        {
                            if (user_input == "1" || user_input == "검색" || user_input == "카드 검색" || user_input == "카드검색")
                            {
                                menuIdx = 1;
                                break;
                            }
                            else if (user_input == "2" || user_input == "추천" || user_input == "카드 추천" || user_input == "카드추천")
                            {
                                menuIdx = 2;
                                break;
                            }
                        }
                        else // 유효하지 않은 입력이면 다시 요청
                        {
                            std::cout << "잘못된 입력입니다. 다시 시도해주세요." << std::endl;
                            continue;
                        }
                    }
                }
                while (menuIdx == 1)
                {
                    system("clear");

                    std::vector<std::string> userSelectedBenefits; // 사용자가 선택한 혜택 리스트
                    std::string result;                            // 사용자가 선택한 혜택

                    /*  유효한 입력값을 모아놓은 set */
                    valid_inputs = {"대중교통", "교통", "쇼핑", "외식", "의료", "주유", "통신",
                                    "1", "2", "3", "4", "5", "6", "back"};

                    std::cout << "----------------------------------\t";
                    std::cout << "----------------------------------\t";
                    std::cout << "----------------------------------\n";
                    std::cout << "|           1.대중교통           |\t";
                    std::cout << "|           2. 쇼핑              |\t";
                    std::cout << "|           3. 외식              |\n";
                    std::cout << "|               🚌               |\t";
                    std::cout << "|               🛍️                |\t";
                    std::cout << "|               🍽️                |\n";
                    std::cout << "----------------------------------\t";
                    std::cout << "----------------------------------\t";
                    std::cout << "----------------------------------\n\n";
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
                    std::cout << "이전 메뉴로 돌아가기 : 'back' 입력\n";

                    // 선호 혜택 선택
                    while (menuIdx == 1)
                    {
                        std::cout << "선호하시는 혜택을 선택하세요 : ";
                        std::getline(std::cin, user_input); // 사용자 입력 받기

                        if (valid_inputs.find(user_input) != valid_inputs.end())
                        {
                            // 유효한 입력이면 대응하는 결과를 설정하고 반복 종료
                            if (user_input == "대중교통" || user_input == "교통" || user_input == "1")
                            {
                                result = "대중교통";
                                break;
                            }
                            else if (user_input == "2" || user_input == "쇼핑")
                            {
                                result = "쇼핑";
                                break;
                            }
                            else if (user_input == "3" || user_input == "외식")
                            {
                                result = "외식";
                                break;
                            }
                            else if (user_input == "4" || user_input == "의료")
                            {
                                result = "의료";
                                break;
                            }
                            else if (user_input == "5" || user_input == "주유")
                            {
                                result = "주유";
                                break;
                            }
                            else if (user_input == "6" || user_input == "통신")
                            {
                                result = "통신";
                                break;
                            }
                            else if (user_input == "back")
                            {
                                menuIdx = 0;
                                break;
                            }
                        }
                        else // 유효하지 않은 입력이면 다시 요청
                        {
                            std::cout << "잘못된 입력입니다. 다시 시도해주세요." << std::endl;
                            continue;
                        }
                    }

                    // 선호 혜택 결과 출력
                    while (menuIdx == 1)
                    {
                        system("clear");
                        userSelectedBenefits.push_back(result); // 사용자가 입력한 혜택을 리스트에 추가

                        for (const auto &card : cards)
                        {
                            if (card.isBenefitMatch(userSelectedBenefits))
                            {
                                card.printCardInfo();
                            }
                        }

                        std::cout << "\n다른 혜택을 선택하시려면'back', 메인 메뉴로 돌아가시려면 'main'을 입력하세요.\n>>";
                        std::cin >> user_input;

                        if (user_input == "back")
                        {
                            break; // 입력을 끝내기 위해 '끝'을 입력
                        }
                        else if (user_input == "main")
                        {
                            menuIdx = 0;
                            break; // 입력을 끝내기 위해 '끝'을 입력
                        }
                    }
                }
                while (menuIdx == 2)
                {
                    std::cout << "핵살론 카드 추천 서비스에 오신걸 환영합니다.\n";
                    std::cout << "계속하려면 아무 키나 누르세요.";
                    std::cin.get(); // 사용자로부터 아무 키나 입력받음
                    break;
                }
                continue;
            }
            
            inputMode = true;
            menu = false;
        }

        /* 일반 메시지 전송 모드로 전환 */
        while (inputMode)
        {
            char buffer[BUF_SIZE];
            // system("clear");
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

        // 에러 시 while 종료
        break;
    }

    // 클라이언트 소켓 종료
    close(client_socket);
    return 0;
}