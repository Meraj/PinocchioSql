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
string querybuilder::QueryBuilder(int Type,bool is_prepared) {
    this->isPrepared = is_prepared;
    string sqlQuery;
    switch (Type) {
        case 0:
            sqlQuery = "SELECT "+this->select_columns+" FROM "+this->table_name;
            break;
        case 1:
            sqlQuery = "INSERT INTO " + this->table_name + " (";
            for(auto column_name :this->insertColumns){
                sqlQuery += column_name + ",";
            }
            sqlQuery.pop_back();
            sqlQuery = sqlQuery + ") VALUES (";
            if(is_prepared){
                for(int i = 1 ; i <= this->preparedValues.size();i++){
                    sqlQuery += "$" + to_string(i) + ",";
                }
                sqlQuery.pop_back();
                sqlQuery = sqlQuery +")";
            }else{
                for(string column_value :this->preparedValues){
                    sqlQuery += column_value + ",";
                }
                sqlQuery.pop_back();
                sqlQuery = sqlQuery +")";
            }
            break;                                                                                                                                                                                  
        case 2:

            break;
        case 3:

            break;
        default:
            break;
    }

    //
    if(Type != 1){
        if(!this->where_statements.empty()){
            sqlQuery += " "+ this->where_statements;
        }
        if(!this->order_by_query.empty()){
            sqlQuery += " "+ this->order_by_query;
        }
        if(!this->limit_query.empty()){
            sqlQuery += " "+ this->limit_query;
        }
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
 * add select
 */
querybuilder querybuilder::addSelect(string column_name) {
    this->select_columns = this->select_columns + "," + column_name;
    return *this;
}
/**
 * select Raw
 */
querybuilder querybuilder::selectRaw(string sql) {
    if(this->selectRaw_sql.empty()){
        this->selectRaw_sql = std::move(sql);
    }else{
        this->selectRaw_sql += ","+sql;
    }
    return *this;
}

/**
 * add select raw
 */
querybuilder querybuilder::addSelectRaw(string sql) {
    if(this->selectRaw_sql.empty()){
        this->selectRaw_sql = std::move(sql);
    }else{
        this->selectRaw_sql += ","+sql;
    }
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
/**
 * or Where
 */
querybuilder querybuilder::orWhere(string column_name, string column_value) {
    this->where_statements = this->where_statements + " OR " + std::move(column_name) + " = " + std::move(column_value);
    return *this;
}
/**
 * or Where with operation
 */
querybuilder querybuilder::orWhere(string column_name, string operation, string column_value) {
    this->where_statements = this->where_statements + " OR " + std::move(column_name) + " " + std::move(operation) + " " + std::move(column_value);
    return *this;
}


/**
 * where statement with custom raw
 */
querybuilder querybuilder::whereRaw(string whereRaw) {
    string before = " AND ";
    if(this->where_statements == "" || this->where_statements.empty()){
        before = " WHERE ";
    }
    this->where_statements = before + whereRaw;
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
    this->insertColumns = {std::move(column_name)};
    this->preparedValues.push_back(column_value);
    return this->doInsert();
}

pqxx::result querybuilder::insert(vector<string> column_names, vector<string> column_values) {
    this->insertColumns = column_names;
    for (string value :column_values){
        this->preparedValues.push_back(value);
    }
    return this->doInsert();
}


pqxx::result querybuilder::doInsert() {
    string sqlQuery = this->QueryBuilder(1);
    pqxx::work W{*this->db};
    try {
        if(this->isPrepared){
            this->db->prepare("example",sqlQuery);
            return W.exec_prepared("example",this->preparedValues);
        }else{
            pqxx::result R{W.exec(sqlQuery)};
            W.commit();
            return R;
        }
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }
}
/**
 * first
 */
pqxx::result querybuilder::first() {
    return this->get(1);
}
/**
 * get
 */
pqxx::result querybuilder::get() {
    string query = this->QueryBuilder();
    pqxx::work W{ *this->db};
    try{
        pqxx::result R{W.exec(query)};
        W.commit();
        return R;
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }
}
/**
 * get with limitation
 */
pqxx::result querybuilder::get(int limit) {
    this->limit(limit);
    string query = this->QueryBuilder();
    pqxx::work W{ *this->db};
    try{
        pqxx::result R{W.exec(query)};
        W.commit();
        return R;
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
    }
}




