//
// Created by meraj on 2/12/21.
//

#include "querybuilder.h"

using namespace std;

#include <utility>
#include <vector>
#include <string>
#include <pqxx/pqxx>
#include <iostream>

/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(string connect) {
    try
    {
        this->db = new pqxx::connection(connect);
        if (!this->db->is_open())
        {
            std::cout << "CAN NOT open database" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }

}
/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(string dbName, string dbUser, string dbPass, string dbHost, string dbPort) {
    try
    {
        this->db = new pqxx::connection("dbname = " + dbName + " user = " + dbUser + " password = " + dbPass+ " hostaddr = " + dbHost +" port = " + dbPort+ "");
        if (!this->db->is_open())
        {
            std::cout << "CAN NOT open database" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }
}
/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(pqxx::connection connection) {
    try
    {
        this->db = &connection;
        if (!this->db->is_open())
        {
            std::cout << "CAN NOT open database" << endl;
        }
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }
}
/**
 *  querybuilder constructor
 */
querybuilder::querybuilder() {
}

/**
 *  Query Builder
 */
string querybuilder::QueryBuilder(int Type) {
    string sqlQuery;
    switch (Type) {
        case 0:
            sqlQuery += "SELECT "+this->select_columns+" FROM "+this->table_name;
            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        default:
            break;
    }
    //
    if(!this->where_statements.empty()){
        sqlQuery += " "+ this->where_statements;
    }
    if(!this->order_by_query.empty()){
        sqlQuery += " "+ this->order_by_query;
    }
    if(!this->limit_query.empty()){
        sqlQuery += " "+ this->limit_query;
    }
    return sqlQuery;
}
/**
 *  set table name
 */
querybuilder querybuilder::setTableName(string TableName) {
    this->table_name = std::move(TableName);
    return *this;
}
/**
 *  select
 */
querybuilder querybuilder::select(string column_name){
    this->select_columns = std::move(column_name);
    return *this;
}
/**
 *  select
 */
querybuilder querybuilder::select(vector<string> column_names) {
    this->select_columns = "";
    for(string column_name : column_names){
        this->select_columns += column_name + ",";
    }
    this->select_columns.pop_back();
    return *this;
}
/**
 * where statement
 */
querybuilder querybuilder::where(string column_name, string column_value) {
    string before = " AND ";
    if(this->where_statements == "" || this->where_statements.empty()){
        before = " WHERE ";
    }
    this->where_statements = this->where_statements + before + std::move(column_name) + "=" + std::move(column_value);
    return *this;
}
/**
 * where statement with operation
 */
querybuilder querybuilder::where(string column_name, string operation, string column_value) {
    string before = " AND ";
    if(this->where_statements == "" || this->where_statements.empty()){
        before = " WHERE ";
    }
    this->where_statements = this->where_statements + before + std::move(column_name) + " " + operation + " " + std::move(column_value);
    return *this;
}



querybuilder querybuilder::whereRaw(string whereRaw) {
    this->where_statements = " WHERE "+whereRaw;
    return *this;
}

/**
 * order by
 */
querybuilder querybuilder::orderBy(string column_name, string order_type) {
    this->order_by_query = "ORDER BY " + std::move(column_name) + " "+std::move(order_type);
    return *this;
}
/**
 * limit
 */
querybuilder querybuilder::limit(int limit, int offset) {
    this->limit_query = " LIMIT "+to_string(limit)+" OFFSET "+to_string(offset);
    return *this;
}

pqxx::result querybuilder::insert(string column_name, string column_value) {
    std::cout << "asdadadadad";
    //  string SqlQuery = this->insertQuery({std::move(column_name)},{std::move(column_value)});
    //  return this->doInsert(SqlQuery);
}

pqxx::result querybuilder::insert(vector<string> column_names, vector<string> column_values) {
    string SqlQuery = this->insertQuery(column_names, column_values);
    cout << SqlQuery << endl;
    return this->doInsert(SqlQuery);
}

string querybuilder::insertQuery(vector<string> column_names, vector<string> column_values) {
    string columnNamesString;
    string columnValuesString;
    for (int i = 0; i < column_names.size(); i++) {
        columnNamesString += column_names[i] + ",";
        if (this->isPrepared) {
            columnValuesString += "$" + to_string(this->preparedIndex) + ",";
            this->preparedValues.push_back(column_values[i]);
            this->preparedIndex++;
        } else {
            columnValuesString += "'" + column_values[i] + "',";
        }
    }
    columnNamesString.pop_back();
    columnValuesString.pop_back();
    string sqlQuery =
            "INSERT INTO " + this->table_name + "(" + columnNamesString + ") VALUES (" + columnValuesString + ")";
    return sqlQuery;
}

pqxx::result querybuilder::doInsert(const string &sqlQuery) {
    pqxx::work W{*this->db};
    try {
        pqxx::result R{W.exec(sqlQuery)};
        W.commit();
        return R;
    } catch (exception exception) {
        cout << "error";
    }

}

pqxx::result querybuilder::first() {
    this->limit(1);
    string query = this->QueryBuilder();
    pqxx::work W{ *this->db};
    try{
        pqxx::result R{W.exec(query)};
        W.commit();
        return R;
    }catch (exception exception){
        cout << "error";
    }
}



