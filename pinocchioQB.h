//
// Created by meraj on 2/12/21.
//

#ifndef PINOCCHIOSQL_QUERYBUILDER_H
#define PINOCCHIOSQL_QUERYBUILDER_H

#include <pqxx/pqxx>

class pinocchioQB {
protected:
    std::string table_name;
private:
    std::vector<std::string> queryValues;
    std::string select_columns = "*";
    std::string selectRaw_sql;
    std::string limit_query;
    std::string where_statements;
    std::string order_by_query;
    std::string groupByQuery;
    std::vector<std::string> customColumns;
    std::vector<std::string> customValues;
    /**
     * execute sql Queries
     * @param query string
     * @author Meraj
     * @since 0.1
     */
    pqxx::result execute(std::string query);
public:
    pqxx::connection *db;
    pinocchioQB();

    /**
     * connect to the database for running sqlQueries
     * @param connect std::string     * @author Meraj
     * @since 0.1
     */
    pinocchioQB(std::string connect);

    /**
    * connect to the database for running sqlQueries
    * @param dbName std::string - database Name
    * @param dbUser std::string - database User
    * @param dbPass std::string - database Password
    * @param dbHost std::string - database HostName
    * @param dbPort std::string - database Port
    * @author Meraj
    * @since 0.1
    */
    pinocchioQB(std::string dbName, std::string dbUser, std::string dbPass, std::string dbHost, std::string dbPort);

    /**
     * connect to the database for running sqlQueries
     * @param connection pqxx::connection - pass pqxx::connection
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB(pqxx::connection connection);

    /**
     * Query Buidler Main Function - build sql queries
     * @param Type int - SELECT -> 0
     * @param Type int - INSERT -> 1
     * @param Type int - UPDATE -> 2
     * @param Type int - DELETE -> 3
     * @author Meraj
     * @since 0.1
     */
    std::string QueryBuilder(int Type = 0);

    /**
     * set table name
     * @param TableName
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB setTableName(std::string TableName);

    /**
     * select a single column
     * @param column_name std::string - column name
     * @sample select("column_one")
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB select(std::string column_name);

    /**
     * select multiple columns
     * @param column_names vector<std::string - column names as Array
     * @sample select({"column_one","column_two"})
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB select(std::vector<std::string> column_names);

    /**
     * select raw for custom select
     * @param sql std::string     * @sample selectRaw("SUM(column_one)")
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB selectRaw(std::string sql);

    /**
     * select raw for custom select with bind params
     * @param sql string
     * @param bindParams vector<string>
     * @author Meraj
     * @since 0.2
     */
    pinocchioQB selectRaw(std::string sql,std::vector<std::string> bindParams);

    /**
     * add select raw for custom select
     * @param sql
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB addSelectRaw(std::string sql);

    /**
     * add select raw for custom select with bind params
     * @param sql
     * @param bindParams
     * @return
     */
    pinocchioQB addSelectRaw(std::string sql,std::vector<std::string> bindParams);

    /**
     * add a new column after run select function
     * @param column_name std::string     * @author Meraj
     * @since 0.1
     */
    pinocchioQB addSelect(std::string column_name);

    /**
     * where statement sql
     * @param column_name std::string     * @param column_value std::string     * @sample where("column_one","this is a test")
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB where(std::string column_name, std::string column_value);

    /**
     * where statement sql with custom operation
     * @param column_name std::string     * @param operation std::string     * @param column_value std::string     * @sample where("column_name","LIKE","this is a test")
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB where(std::string column_name, std::string operation, std::string column_value);

    /**
     * OR WHERE
     * @param column_name std::string     * @param column_value std::string     * @author Meraj
     * @since 0.1
     */
    pinocchioQB orWhere(std::string column_name, std::string column_value);

     /**
     * OR WHERE with operation
     * @param column_name std::string     * @param operation std::string     * @param column_value std::string     * @author Meraj
     * @since 0.1
     */
    pinocchioQB orWhere(std::string column_name, std::string operation, std::string column_value);

    /**
       * where statement sql with custom raw
       * @param whereRaw std::string       * @sample whereRaw("column_one = 'this is a test'")
       * @sample whereRaw("column_one LIKE 'this is a test'")
       * @author Meraj
       * @since 0.1
       */
    pinocchioQB whereRaw(std::string whereRaw);

    /**
     * column_name the name of the column that you want to sort by
     * @param column_name std::string     * @param order_type std::string -> ASC or DESC
     * @sample orderBy("column_one","ASC");
     * @sample orderBy("column_one","DESC");
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB orderBy(std::string column_name, std::string order_type);

    /**
     * limit rows
     * @param limit int
     * @param offset int
     * @sample limit(10);
     * @sample limit(10,2);
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB limit(int limit, int offset = 0);

    /**
     * Group By
     * @param column_name string
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB groupBy(std::string column_name);

    /**
     * Group By
     * @param column_name vector<string>
     * @author Meraj
     * @since 0.1
     */
    pinocchioQB groupBy(std::vector<std::string> column_names);

    pqxx::result insert(std::string column_name, std::string column_value);

    pqxx::result insert(std::vector<std::string> column_names, std::vector<std::string> column_values);
    /**
     * get single row
     * @return pqxx::row
     * @author Meraj
     * @since 0.1
     */
    pqxx::row first();
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

    /**
     * count Rows
     * @author Meraj
     * @since 0.1
     */
    int count();

    /**
     * execute custom query
     * @param sql
     * @author Meraj
     * @since 0.1
     */
    pqxx::result query(std::string sql);
    /**
     * update row/rows
     * @param column_name string
     * @param column_value string
     * @author Meraj
     * @since 0.1
     */
    pqxx::result update(std::string column_name,std::string column_value);

    /**
     * update row/rows
     * @param column_names vector<string>
     * @param column_values vector<string>
     * @author Meraj
     * @since 0.1
     */

    pqxx::result update(std::vector<std::string> column_names,std::vector<std::string> column_values);

    /**
     * delete row/rows
     * @author Meraj
     * @since 0.1
     */
    pqxx::result Delete();


};

#endif //PINOCCHIOSQL_QUERYBUILDER_H
