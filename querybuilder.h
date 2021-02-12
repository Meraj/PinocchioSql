//
// Created by meraj on 2/12/21.
//

#ifndef PQXXQB_QUERYBUILDER_H
#define PQXXQB_QUERYBUILDER_H

#include <iostream>

using namespace std;

#include <vector>
#include <string>
#include <pqxx/pqxx>
class querybuilder {
protected:
    string table_name;
    pqxx::connection* db;
private:
    bool isPrepared = false;
    int preparedIndex = 1;
    std::vector<std::string> preparedValues;
    string select_columns;
    string limit_query;
    string where_statements;
    string order_by_query;
public:
    string QueryBuilder(int Type = 0);
    void setTableName(string TableName);
    querybuilder select(string column_name);
    querybuilder select(vector<string> column_names);
    querybuilder orderBy(string column_name,string order_type);
    querybuilder limit(int limit,int offset = 0);
    querybuilder where(string column_name,string column_value);
    querybuilder whereRaw(string whereRaw);
    string insertQuery(vector<string> column_names,vector<string> column_values);
    pqxx::result doInsert(const string& sqlQuery);
    pqxx::result insert(string column_name,string column_value);
    pqxx::result insert(vector<string> column_names,vector<string> column_values);
    pqxx::result first();
    void strConnect(string con);
};

#endif //PQXXQB_QUERYBUILDER_H
