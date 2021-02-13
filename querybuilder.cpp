//
// Created by meraj on 2/12/21.
//

#include "querybuilder.h"

#include <iostream>
#include <pqxx/pqxx>

/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(std::string connect) {
    try {
        this->db = new pqxx::connection(connect);
        if (!this->db->is_open()) {
            std::cout << "CAN NOT open database" << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}

/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(std::string dbName, std::string dbUser, std::string dbPass, std::string dbHost,
                           std::string dbPort) {
    try {
        this->db = new pqxx::connection(
                "dbname = " + dbName + " user = " + dbUser + " password = " + dbPass + " hostaddr = " + dbHost +
                " port = " + dbPort + "");
        if (!this->db->is_open()) {
            std::cout << "CAN NOT open database" << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

/**
 *  querybuilder constructor
 */
querybuilder::querybuilder(pqxx::connection connection) {
    try {
        this->db = &connection;
        if (!this->db->is_open()) {
            std::cout << "CAN NOT open database" << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
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
std::string querybuilder::QueryBuilder(int Type, bool is_prepared) {
    this->isPrepared = is_prepared;
    std::string sqlQuery;
    switch (Type) {
        case 0:
            sqlQuery = "SELECT " + this->select_columns + " FROM " + this->table_name;
            break;
        case 1:
            sqlQuery = "INSERT INTO " + this->table_name + " (";
            for (auto column_name :this->customColumns) {
                sqlQuery += column_name + ",";
            }
            sqlQuery.pop_back();
            sqlQuery = sqlQuery + ") VALUES (";
            if (is_prepared) {
                for (int i = 1; i <= this->preparedValues.size(); i++) {
                    sqlQuery += "$" + std::to_string(i) + ",";
                }
                sqlQuery.pop_back();
                sqlQuery = sqlQuery + ")";
            } else {
                for (std::string column_value :this->preparedValues) {
                    sqlQuery += column_value + ",";
                }
                sqlQuery.pop_back();
                sqlQuery = sqlQuery + ")";
            }
            break;
        case 2: {
            sqlQuery = "UPDATE " + this->table_name + " SET ";
            std::string setColumns;
            if (this->isPrepared) {
                for (int i = 0; i < this->customColumns.size(); i++) {
                    setColumns += this->customColumns[i] + " = $" + std::to_string(this->preparedIndex) + ",";
                    this->preparedIndex++;
                }
                setColumns.pop_back();
            } else {
                for (int i = 0; i < this->customColumns.size(); i++) {
                    setColumns += this->customColumns[i] + " = " + this->customValues[i] + ",";
                }
                setColumns.pop_back();
            }

            break;
        }
        case 3:

            break;
        default:

            break;
    }
    //
    if (Type != 1) {
        if (!this->where_statements.empty()) {
            std::string prepare = "$?";
            while (this->where_statements.find(prepare) != std::string::npos) {
                this->where_statements.replace(this->where_statements.find(prepare), prepare.length(),
                                               "$" + this->preparedIndex);
                this->preparedIndex++;
            }
            if (this->customValues.size() != 0) {
                std::vector<std::string> bcPreparedValues = this->preparedValues;
                this->preparedValues = this->customValues;
                for (std::string value :bcPreparedValues) {
                    this->preparedValues.push_back(value);
                }
            }
            sqlQuery += " " + this->where_statements;
            this->where_statements = "";
        }
        if (!this->order_by_query.empty()) {
            sqlQuery += " " + this->order_by_query;
            this->order_by_query = "";
        }
        if (!this->groupByQuery.empty()) {
            sqlQuery += " " + this->groupByQuery;
            this->groupByQuery = "";
        }
        if (!this->limit_query.empty()) {
            sqlQuery += " " + this->limit_query;
            this->limit_query = "";
        }
    }
    return sqlQuery;
}

/**
 *  set table name
 */
querybuilder querybuilder::setTableName(std::string TableName) {
    this->table_name = std::move(TableName);
    return *this;
}

/**
 *  select
 */

querybuilder querybuilder::select(std::string column_name) {
    this->select_columns = std::move(column_name);
    return *this;
}

/**
 *  select
 */

querybuilder querybuilder::select(std::vector<std::string> column_names) {
    this->select_columns = "";
    for (std::string column_name : column_names) {
        this->select_columns += column_name + ",";
    }
    this->select_columns.pop_back();
    return *this;
}

/**
 * add select
 */
querybuilder querybuilder::addSelect(std::string column_name) {
    this->select_columns = this->select_columns + "," + column_name;
    return *this;
}

/**
 * select Raw
 */
querybuilder querybuilder::selectRaw(std::string sql) {
    if (this->selectRaw_sql.empty()) {
        this->selectRaw_sql = std::move(sql);
    } else {
        this->selectRaw_sql += "," + sql;
    }
    return *this;
}

/**
 * add select raw
 */
querybuilder querybuilder::addSelectRaw(std::string sql) {
    if (this->selectRaw_sql.empty()) {
        this->selectRaw_sql = std::move(sql);
    } else {
        this->selectRaw_sql += "," + sql;
    }
    return *this;
}

/**
 * where statement
 */
querybuilder querybuilder::where(std::string column_name, std::string column_value) {
    std::string before = " AND ";
    if (this->where_statements == "" || this->where_statements.empty()) {
        before = " WHERE ";
    }
    if (this->isPrepared) {
        this->where_statements = this->where_statements + before + std::move(column_name) + "= $? ";
        this->preparedValues.push_back(column_value);
    } else {
        this->where_statements =
                this->where_statements + before + std::move(column_name) + "=" + std::move(column_value);
    }
    return *this;
}

/**
 * where statement with operation
 */
querybuilder querybuilder::where(std::string column_name, std::string operation, std::string column_value) {
    std::string before = " AND ";
    if (this->where_statements == "" || this->where_statements.empty()) {
        before = " WHERE ";
    }
    if (this->isPrepared) {
        this->where_statements =
                this->where_statements + before + std::move(column_name) + " " + std::move(operation) + " $? ";
        this->preparedValues.push_back(column_value);
    } else {
        this->where_statements =
                this->where_statements + before + std::move(column_name) + " " + std::move(operation) + " " +
                std::move(column_value);
    }
    return *this;
}

/**
 * or Where
 */
querybuilder querybuilder::orWhere(std::string column_name, std::string column_value) {
    if (this->isPrepared) {
        this->where_statements = this->where_statements + " OR " + std::move(column_name) + "= $? ";
        this->preparedValues.push_back(column_value);
    } else {
        this->where_statements =
                this->where_statements + " OR " + std::move(column_name) + "=" + std::move(column_value);
    }
    return *this;
}

/**
 * or Where with operation
 */
querybuilder querybuilder::orWhere(std::string column_name, std::string operation, std::string column_value) {
    if (this->isPrepared) {
        this->where_statements =
                this->where_statements + " OR " + std::move(column_name) + " " + std::move(operation) + " $? ";
        this->preparedValues.push_back(column_value);
    } else {
        this->where_statements =
                this->where_statements + " OR " + std::move(column_name) + " " + std::move(operation) + " " +
                std::move(column_value);
    }
    return *this;
}

/**
 * where statement with custom raw
 */
querybuilder querybuilder::whereRaw(std::string whereRaw) {
    std::string before = " AND ";
    if (this->where_statements == "" || this->where_statements.empty()) {
        before = " WHERE ";
    }
    this->where_statements = before + whereRaw;
    return *this;
}

/**
 * order by
 */
querybuilder querybuilder::orderBy(std::string column_name, std::string order_type) {
    this->order_by_query = "ORDER BY " + std::move(column_name) + " " + std::move(order_type);
    return *this;
}

/**
 * limit
 */
querybuilder querybuilder::limit(int limit, int offset) {
    this->limit_query = " LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    return *this;
}

/**
 * group by
 */
querybuilder querybuilder::groupBy(std::string column_name) {
    this->groupByQuery = " GROUP BY " + column_name;
    return *this;
}

/**
 * group by
 */
querybuilder querybuilder::groupBy(std::vector<std::string> column_names) {
    std::string columns;
    for (std::string column :column_names) {
        columns += column + ",";
    }
    columns.pop_back();
    this->groupByQuery = " GROUP BY " + columns;
    return *this;
}


pqxx::result querybuilder::insert(std::string column_name, std::string column_value) {
    this->customColumns = {std::move(column_name)};
    this->preparedValues.push_back(column_value);
    return this->execute(this->QueryBuilder(1));
}

pqxx::result querybuilder::insert(std::vector<std::string> column_names, std::vector<std::string> column_values) {
    this->customColumns = column_names;
    for (std::string value :column_values) {
        this->preparedValues.push_back(value);
    }
    return this->execute(this->QueryBuilder(1));
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
    return this->execute(this->QueryBuilder());
}

/**
 * get with limitation
 */
pqxx::result querybuilder::get(int limit) {
    this->limit(limit);
    return this->execute(this->QueryBuilder());
}

pqxx::result querybuilder::query(std::string sql) {
    return this->execute(sql);
}

int querybuilder::count() {
    this->selectRaw("COUNT(*)");
    pqxx::result result = this->execute(this->QueryBuilder());
}

pqxx::result querybuilder::execute(std::string query) {
    pqxx::work W{*this->db};
    try {
        if (this->isPrepared) {
            this->db->prepare("example", query);
            return W.exec_prepared("example", this->preparedValues);
        } else {
            pqxx::result R{W.exec(query)};
            W.commit();
            return R;
        }
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

pqxx::result querybuilder::update(std::string column_name, std::string column_value) {
    this->customColumns = {column_name};
    this->customValues = {column_value};
    return this->execute(this->QueryBuilder(2));
}





