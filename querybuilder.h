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
    pqxx::connection *db;
private:
    bool isPrepared = false;
    int preparedIndex = 1;
    std::vector<std::string> preparedValues;
    string select_columns;
    string selectRaw_sql;
    string limit_query;
    string where_statements;
    string order_by_query;

    vector<string> insertColumns;
    vector<string> insertValues;

    pqxx::result doInsert();
public:
    querybuilder();

    /**
     * connect to the database for running sqlQueries
     * @param connect string
     * @author Meraj
     * @since 0.1
     */
    querybuilder(string connect);

    /**
    * connect to the database for running sqlQueries
    * @param dbName string - database Name
    * @param dbUser string - database User
    * @param dbPass string - database Password
    * @param dbHost string - database HostName
    * @param dbPort string - database Port
    * @author Meraj
    * @since 0.1
    */
    querybuilder(string dbName, string dbUser, string dbPass, string dbHost, string dbPort);

    /**
     * connect to the database for running sqlQueries
     * @param connection pqxx::connection - pass pqxx::connection
     * @author Meraj
     * @since 0.1
     */
    querybuilder(pqxx::connection connection);

    /**
     * Query Buidler Main Function - build sql queries
     * @param Type int - SELECT -> 0
     * @param Type int - INSERT -> 1
     * @param Type int - UPDATE -> 2
     * @param Type int - DELETE -> 3
     * @author Meraj
     * @since 0.1
     */
    string QueryBuilder(int Type = 0, bool is_prepared = false);

    /**
     * set table name
     * @param TableName
     * @author Meraj
     * @since 0.1
     */
    querybuilder setTableName(string TableName);

    /**
     * select a single column
     * @param column_name string - column name
     * @sample select("column_one")
     * @author Meraj
     * @since 0.1
     */
    querybuilder select(string column_name);

    /**
     * select multiple columns
     * @param column_names vector<string> - column names as Array
     * @sample select({"column_one","column_two"})
     * @author Meraj
     * @since 0.1
     */
    querybuilder select(vector<string> column_names);

    /**
     * select raw for custom select
     * @param sql string
     * @sample selectRaw("SUM(column_one)")
     * @author Meraj
     * @since 0.1
     */
    querybuilder selectRaw(string sql);

    /**
     * add select raw for custom select
     * @param sql
     * @author Meraj
     * @since 0.1
     */
    querybuilder addSelectRaw(string sql);

    /**
     * add a new column after run select function
     * @param column_name string
     * @author Meraj
     * @since 0.1
     */
    querybuilder addSelect(string column_name);

    /**
     * column_name the name of the column that you want to sort by
     * @param column_name string
     * @param order_type string -> ASC or DESC
     * @sample orderBy("column_one","ASC");
     * @sample orderBy("column_one","DESC");
     * @author Meraj
     * @since 0.1
     */
    querybuilder orderBy(string column_name, string order_type);

    /**
     * limit rows
     * @param limit int
     * @param offset int
     * @sample limit(10);
     * @sample limit(10,2);
     * @author Meraj
     * @since 0.1
     */
    querybuilder limit(int limit, int offset = 0);

    /**
     * where statement sql
     * @param column_name string
     * @param column_value string
     * @sample where("column_one","this is a test")
     * @author Meraj
     * @since 0.1
     */
    querybuilder where(string column_name, string column_value);

    /**
     * where statement sql with custom operation
     * @param column_name string
     * @param operation string
     * @param column_value string
     * @sample where("column_name","LIKE","this is a test")
     * @author Meraj
     * @since 0.1
     */
    querybuilder where(string column_name, string operation, string column_value);

    /**
     * OR WHERE
     * @param column_name string
     * @param column_value string
     * @author Meraj
     * @since 0.1
     */
    querybuilder orWhere(string column_name, string column_value);

     /**
     * OR WHERE with operation
     * @param column_name string
     * @param operation string
     * @param column_value string
     * @author Meraj
     * @since 0.1
     */
    querybuilder orWhere(string column_name, string operation, string column_value);

    /**
       * where statement sql with custom raw
       * @param whereRaw string
       * @sample whereRaw("column_one = 'this is a test'")
       * @sample whereRaw("column_one LIKE 'this is a test'")
       * @author Meraj
       * @since 0.1
       */
    querybuilder whereRaw(string whereRaw);




    pqxx::result insert(string column_name, string column_value);

    pqxx::result insert(vector<string> column_names, vector<string> column_values);
    /**
     * get single row
     * @return pqxx::result
     * @author Meraj
     * @since 0.1
     */
    pqxx::result first();
    /**
     * get row/rows
     * @return pqxx::result
     * @author Meraj
     * @since 0.1
     */
    pqxx::result get();
    /**
     * get row/rows with limit
     * @param limit int
     * @sample get(2) -> get 2 rows
     * @return pqxx::result
     * @author Meraj
     * @since 0.1
     */
    pqxx::result get(int limit);
};

#endif //PQXXQB_QUERYBUILDER_H
