#include <sstream>  // ostringstream 사용을 위해 추가
#include "maria_db_connector.h"

void MariaDBConnector::initialize()
{
    if (not initialized_)
    {
        try
        {
            driver_ = sql::mariadb::get_driver_instance();
            initialized_ = true;
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error during initialization: " << e.what() << std::endl;
            return;
        }
    }
}

void MariaDBConnector::finalize()
{
    if (isConnected())
    {
        conn_->close();
    }

    driver_ = nullptr;
    connected_ = false;
}

bool MariaDBConnector::connect(const char *host, const char *user, const char *passwd, int port)
{
    if (checkInitialized())
    {
        try
        {
            sql::SQLString url;
            if (port < 0)
            {
                // url = fmt::print("{}{}", "jdbc:mariadb://", std::string(host));
                url = "jdbc:mariadb://10.10.20.116:3306/card_project";
            }
            else
            {
                url = "jdbc:mariadb://10.10.20.116:3306/card_project";
                // url = fmt::print("{}{}/{}", "jdbc:mariadb://", std::string(host), std::to_string(port));
            }

            sql::Properties properties({{"user", user}, {"password", passwd}});

            conn_.reset(driver_->connect(url, properties));
            connected_ = true;

            std::cout << "Connected to " << url << std::endl;

            return true;
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error during connection: " << e.what() << std::endl;

            return false;
        }
    }

    return false;
}

bool MariaDBConnector::query(const char *query_content)
{
    if (checkInitialized())
    {
        try
        {
            std::unique_ptr<sql::Statement> stmt(conn_->createStatement());
            sql::SQLString query(fmt::format("{}", query_content));

            sql::ResultSet *res = stmt->executeQuery(query);
            if (res != nullptr)
            {
                unsigned int col_nums = res->getMetaData()->getColumnCount();
                std::ostringstream output_stream;

                // 행을 반복해서 값을 저장           
                while (res->next())  // 첫 번째 행부터 출력되도록 수정
                {
                    for (int i = 1; i <= col_nums; ++i)
                    {
                        output_stream << res->getString(i);
                        if (i != col_nums)  // 마지막 컬럼이 아닐 때만 언더바 추가
                        {
                            output_stream << "_";
                        }
                    }
                    output_stream << std::endl;  // 각 행 끝에 줄바꿈 추가
                }

                // 결과를 멤버 변수에 저장
                result_ = output_stream.str();
            }

            return true;
        }
        catch (sql::SQLException &e)
        {
            std::cerr << "Error during query: " << e.what() << std::endl;
            result_.clear(); // 예외 발생 시 결과를 초기화
            return false;
        }
    }
    return false;
}

std::string MariaDBConnector::getQueryResult() const
{
    return result_; // 저장된 쿼리 결과 반환
}

bool MariaDBConnector::isConnected()
{
    return connected_;
}

bool MariaDBConnector::checkInitialized()
{
    if (initialized_)
    {
        return true;
    }
    else
    {
        std::cerr << "Maria DB is not initialized." << std::endl;
        return false;
    }
}