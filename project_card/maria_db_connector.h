#include <stdio.h>
#include <iostream>
#include <string>
#include <fmt/core.h>
//#include <fmt/format.h>
//#include <format>
#include <mariadb/conncpp.hpp>

class MariaDBConnector
{
public:
    void initialize();
    void finalize();
    bool connect(const char* host, const char* user, const char* passwd, int port = -1);
    bool query(const char* query_content);
    std::string getQueryResult() const; // 결과를 반환하는 함수 추가
    bool isConnected();

protected:
private:
    bool checkInitialized();

    sql::Driver* driver_ = nullptr;
    std::unique_ptr<sql::Connection> conn_ = nullptr;
    std::string result_; // 쿼리 결과를 저장할 멤버 변수 추가

    bool initialized_ = false;
    bool connected_ = false;
};

#ifndef MARIA_DB_TUTORIAL_MARIA_DB_CONNECTOR_H
#define MARIA_DB_TUTORIAL_MARIA_DB_CONNECTOR_H

#endif //MARIA_DB_TUTORIAL_MARIA_DB_CONNECTOR_H