//
// Created by meraj on 2/12/21.
//

#include "pinocchioQB.h"

#include <iostream>
#include <pqxx/pqxx>

/**
 *  pinocchioQB constructor
 */
pinocchioQB::pinocchioQB(std::string connect) {
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
 *  pinocchioQB constructor
 */
pinocchioQB::pinocchioQB(std::string dbName, std::string dbUser, std::string dbPass, std::string dbHost,
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
 *  pinocchioQB constructor
 */
pinocchioQB::pinocchioQB(pqxx::connection connection) {
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
 *  pinocchioQB constructor
 */
pinocchioQB::pinocchioQB() {}

/**
 *  Query Builder
 */
std::string pinocchioQB::QueryBuilder(int Type) {
    std::string sqlQuery;
    switch (Type) {
        case 0:
            sqlQuery = "SELECT " + this->select_columns + " FROM " + this->table_name;
            this->select_columns = " * ";
            break;
        case 1:
            sqlQuery = "INSERT INTO " + this->table_name + " (";
            for (auto column_name :this->customColumns) {
                sqlQuery += column_name + ",";
            }
            sqlQuery.pop_back();
            sqlQuery = sqlQuery + ") VALUES (";
            for (std::string column_value :this->queryValues) {
                sqlQuery += "'" + this->db->esc(column_value) + "',";
            }
            sqlQuery.pop_back();
            sqlQuery = sqlQuery + ")";

            break;
        case 2: {
            sqlQuery = "UPDATE " + this->table_name + " SET ";
            std::string setColumns;
            for (int i = 0; i < this->customColumns.size(); i++) {
                setColumns += this->customColumns[i] + " = '" + this->db->esc(this->customValues[i]) + "',";
            }
            setColumns.pop_back();
            sqlQuery += setColumns + " ";
            break;
        }
        case 3:
            sqlQuery = "DELETE FROM " + this->table_name + " ";
            break;
        default:

            break;
    }
    //
    if (Type != 1) {
        if (!this->where_statements.empty()) {
            sqlQuery += " " + this->where_statements;
        }
        if (!this->order_by_query.empty()) {
            sqlQuery += " " + this->order_by_query;
        }
        if (!this->groupByQuery.empty()) {
            sqlQuery += " " + this->groupByQuery;
        }
        if (!this->limit_query.empty()) {
            sqlQuery += " " + this->limit_query;
        }
    }
    std::cout << sqlQuery << std::endl;
    this->resetVariables();
    return sqlQuery;
}

/**
 *  set table name
 */
pinocchioQB pinocchioQB::setTableName(std::string TableName) {
    this->table_name = std::move(TableName);
    return *this;
}

/**
 *  select
 */

pinocchioQB pinocchioQB::select(std::string column_name) {
    this->select_columns = std::move(column_name);
    return *this;
}

/**
 *  select
 */

pinocchioQB pinocchioQB::select(std::vector<std::string> column_names) {
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
pinocchioQB pinocchioQB::addSelect(std::string column_name) {
    this->select_columns = this->select_columns + "," + column_name;
    return *this;
}

/**
 * select Raw
 */
pinocchioQB pinocchioQB::selectRaw(std::string sql) {
    this->select_columns = std::move(sql);
    return *this;
}

/**
 * select Raw with bind params
 */
pinocchioQB pinocchioQB::selectRaw(std::string sql, std::vector<std::string> bindParams) {
    std::string prepare = "$?";
    int i = 0;
    while (sql.find(prepare) != std::string::npos) {
        sql.replace(sql.find(prepare), prepare.length(), this->db->esc(bindParams[i]));
        i++;
    }
    this->select_columns = std::move(sql);
    return *this;
}


/**
 * add select raw
 */
pinocchioQB pinocchioQB::addSelectRaw(std::string sql) {
    if (this->select_columns.empty()) {
        this->select_columns = std::move(sql);
    } else {
        this->select_columns += "," + sql;
    }
    return *this;
}

/**
 * add select raw with bind params
 */
pinocchioQB pinocchioQB::addSelectRaw(std::string sql, std::vector<std::string> bindParams) {
    std::string prepare = "$?";
    int i = 0;
    while (sql.find(prepare) != std::string::npos) {
        sql.replace(sql.find(prepare), prepare.length(), this->db->esc(bindParams[i]));
        i++;
    }
    if (this->select_columns.empty()) {
        this->select_columns = std::move(sql);
    } else {
        this->select_columns += "," + sql;
    }
    return *this;
}


/**
 * where statement
 */
pinocchioQB pinocchioQB::where(std::string column_name, std::string column_value) {
    std::string before = " AND ";
    if (this->where_statements.empty()) {
        before = " WHERE ";
    }
    this->where_statements += before + std::move(column_name) + "='" + this->db->esc(std::move(column_value)) +"' ";
    return *this;
}

/**
 * where statement with operation
 */
pinocchioQB pinocchioQB::where(std::string column_name, std::string operation, std::string column_value) {
    std::string before = " AND ";
    if (this->where_statements.empty()) {
        before = " WHERE ";
    }
    this->where_statements =
            this->where_statements + before + std::move(column_name) + " " + std::move(operation) + " '" +
            this->db->esc(std::move(column_value)) + "'";

    return *this;
}

/**
 * or Where
 */
pinocchioQB pinocchioQB::orWhere(std::string column_name, std::string column_value) {
    this->where_statements =
            this->where_statements + " OR " + std::move(column_name) + "='" + this->db->esc(std::move(column_value)) +
            "'";
    return *this;
}

/**
 * or Where with operation
 */
pinocchioQB pinocchioQB::orWhere(std::string column_name, std::string operation, std::string column_value) {
    this->where_statements =
            this->where_statements + " OR " + std::move(column_name) + " " + std::move(operation) + " '" +
            this->db->esc(std::move(column_value)) + "'";

    return *this;
}

/**
 * where statement with custom raw
 */
pinocchioQB pinocchioQB::whereRaw(std::string whereRaw) {
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
pinocchioQB pinocchioQB::orderBy(std::string column_name, std::string order_type) {
    this->order_by_query = "ORDER BY " + std::move(column_name) + " " + std::move(order_type);
    return *this;
}

/**
 * limit
 */
pinocchioQB pinocchioQB::limit(int limit, int offset) {
    this->limit_query = " LIMIT " + std::to_string(limit) + " OFFSET " + std::to_string(offset);
    return *this;
}

/**
 * group by
 */
pinocchioQB pinocchioQB::groupBy(std::string column_name) {
    this->groupByQuery = " GROUP BY " + column_name;
    return *this;
}

/**
 * group by
 */
pinocchioQB pinocchioQB::groupBy(std::vector<std::string> column_names) {
    std::string columns;
    for (std::string column :column_names) {
        columns += column + ",";
    }
    columns.pop_back();
    this->groupByQuery = " GROUP BY " + columns;
    return *this;
}


pqxx::result pinocchioQB::insert(std::string column_name, std::string column_value) {
    this->customColumns = {std::move(column_name)};
    this->queryValues.push_back(column_value);
    return this->execute(this->QueryBuilder(1));
}

pqxx::result pinocchioQB::insert(std::vector<std::string> column_names, std::vector<std::string> column_values) {
    this->customColumns = column_names;
    for (std::string value :column_values) {
        this->queryValues.push_back(value);
    }
    return this->execute(this->QueryBuilder(1));
}


/**
 * first
 */
pqxx::row pinocchioQB::first() {
    return this->get(1)[0];
}

/**
 * get
 */
pqxx::result pinocchioQB::get() {
    std::string query = this->QueryBuilder();
    return this->execute(query);
}

/**
 * get with limitation
 */
pqxx::result pinocchioQB::get(int limit) {
    this->limit(limit);
    return this->execute(this->QueryBuilder());
}

pqxx::result pinocchioQB::query(std::string sql) {
    return this->execute(sql);
}

int pinocchioQB::count() {
    this->selectRaw("COUNT(*)");
    pqxx::result result = this->execute(this->QueryBuilder());
    try{
        return std::stoi(result[0][0].c_str());
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

pqxx::result pinocchioQB::execute(std::string query) {

    pqxx::work w(*this->db);
    this->resetVariables();
    try {
        pqxx::result result = w.exec(query);
        w.commit();
        return result;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

/**
 * update
 */
pqxx::result pinocchioQB::update(std::string column_name, std::string column_value) {
    this->customColumns = {std::move(column_name)};
    this->customValues = {std::move(column_value)};
    return this->execute(this->QueryBuilder(2));
}

/**
 * update
 */
pqxx::result pinocchioQB::update(std::vector<std::string> column_names, std::vector<std::string> column_values) {
    this->customColumns = std::move(column_names);
    this->customValues = std::move(column_values);
    return this->execute(this->QueryBuilder(2));
}

/**
 * delete row/rows
 */
pqxx::result pinocchioQB::Delete() {
    return this->execute(this->QueryBuilder(3));
}

void pinocchioQB::resetVariables() {
    this->queryValues.clear();
    this->select_columns = " * ";
    this->limit_query.clear();
    this->where_statements.clear();
    this->where_statements = "";
    this->order_by_query.clear();
    this->groupByQuery.clear();
    this->customColumns.clear();
    this->customValues.clear();
}
