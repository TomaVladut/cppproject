#define _CRT_SECURE_NO_WARNINGS
#include <regex>
#include <iostream>
#include <string>
#include <string.h>

class CommandProcessor {
    char* commandString = nullptr;
    static const int COMMAND_TYPES_COUNT = 9;
    const char* commandTypes[COMMAND_TYPES_COUNT] = {"CREATE TABLE", "CREATE INDEX", "DROP TABLE",
    "DROP INDEX", "DISPLAY TABLE", "INSERT", "SELECT", "UPDATE", "DELETE"};

    // Helper function to validate the column definition
    void ValidateColumnDefinition(const std::string& column) {
        // Split the column definition into parts (name, type, size, default_value)
        std::string parts[4];
        std::regex partsRegex(R"(\s*\((\w+),\s*(\w+),\s*(\d+),\s*'([^']*)'\)\s*)");
        std::smatch match;
        if (std::regex_match(column, match, partsRegex)) {
            for (size_t i = 1; i < match.size(); ++i) {
                parts[i - 1] = match[i];
            }
        }
        else {
            std::cout << "Error: Invalid column definition format." << std::endl;
            return;
        }

        // Validate each part of the column definition
        ValidateColumnName(parts[0]);
        ValidateColumnType(parts[1]);
        ValidateColumnSize(parts[2]);
        ValidateColumnDefaultValue(parts[3]);
    }

    // Helper function to validate the column name
    void ValidateColumnName(const std::string& name) {
        // Validate if the column name is alphanumeric
        if (!std::regex_match(name, std::regex("^[a-zA-Z0-9_]+$"))) {
            std::cout << "Error: Invalid characters in column name." << std::endl;
        }
    }

    // Helper function to validate the column type
    void ValidateColumnType(const std::string& type) {
        // Validate if the column type is one of the accepted types (text, integer, float)
        if (type != "text" && type != "integer" && type != "float") {
            std::cout << "Error: Invalid column type." << std::endl;
        }
    }

    // Helper function to validate the column size
    void ValidateColumnSize(const std::string& size) {
        // Validate if the column size is a valid positive integer
        if (!std::regex_match(size, std::regex("^[1-9]\\d*$"))) {
            std::cout << "Error: Invalid column size." << std::endl;
        }
    }

    // Helper function to validate the column default value
    void ValidateColumnDefaultValue(const std::string& defaultValue) {
        // No specific validation for default value in this example
    }


public:
    static const int MIN_COMMAND_SIZE = 5;

    CommandProcessor() {
        this->commandString = new char[1];
        strcpy_s(commandString, 1, "");
    }

    CommandProcessor(const char* initialCommand) {
        if (strlen(initialCommand) >= MIN_COMMAND_SIZE) {
            this->commandString = new char[strlen(initialCommand) + 1];
            strcpy_s(this->commandString, strlen(initialCommand) + 1, initialCommand);
        }
        else {
            throw std::invalid_argument("Invalid Command");
        }
    }

    CommandProcessor(CommandProcessor& other) {
        this->commandString = new char[strlen(other.commandString) + 1];
        strcpy_s(this->commandString, strlen(other.commandString) + 1, other.commandString);
    }

    ~CommandProcessor() {
        delete[] this->commandString;
    }

    void SetCommandString(const char* newCommandString)
    {
        if (strlen(newCommandString) >= MIN_COMMAND_SIZE)
        {
            this->commandString = new char[strlen(newCommandString) + 1];
            strcpy_s(this->commandString, strlen(newCommandString) + 1, newCommandString);
        }
        else 
            throw new std::exception("Invalid Command");
    }

    char* GetCommandString()
    {
        char* valueOfCommandString = new char[strlen(this->commandString) + 1];
        strcpy_s(valueOfCommandString, strlen(this->commandString) + 1, this->commandString);
        return valueOfCommandString;
    }

    void IdentifyCommandType() {
        bool commandIdentified = false;
        for (int i = 0; i < COMMAND_TYPES_COUNT; i++)
        {
            if (strstr(commandString, commandTypes[i]) != nullptr)
            {
                std::cout << std::endl << "Command type: " << commandTypes[i] << std::endl;
                commandIdentified = true;
                break;
            }
        }
        if (!commandIdentified) {
            std::cout << "Error: Unrecognized command type." << std::endl;
        }
    }

    //this one is kinda useless now (ValidateCommand), I wanted to do a fucntion for all the commands.

    void ValidateCommand() {
        bool validCommandType = false;
        for (int i = 0; i < COMMAND_TYPES_COUNT; ++i) {
            if (strstr(commandString, commandTypes[i]) != nullptr) {
                validCommandType = true;
                break;
            }
        }
        if (!validCommandType) {
            std::cout << "Error: Unrecognized command type." << std::endl;
            return;
        }
        if (strstr(commandString, "CREATE TABLE") != nullptr) {
            // check if it contains "(column_1_name,type,size, default_value)"
            // I am thinking of using REGEX for validations because it will get messy...
            if (strstr(commandString, "(column_1_name,type,size, default_value)") == nullptr) {
                std::cout << "Error: Invalid format for CREATE TABLE command." << std::endl;
                return;
            }
        }
        std::cout << "Command is valid." << std::endl;
    }

    void ValidateCreateTableCommand(const std::string& commandString) {
        // Validate if the command contains "CREATE TABLE"
        if (commandString.find("CREATE TABLE") == std::string::npos) {
            std::cout << "Error: Missing 'CREATE TABLE' keyword." << std::endl;
            return;
        }

        // Validate if the command contains "(" and ")"
        if (commandString.find("(") == std::string::npos || commandString.find(")") == std::string::npos) {
            std::cout << "Error: Missing '(' or ')' in column definition." << std::endl;
            return;
        }

        // Extract the table name
        std::string tableName;
        std::regex tableNameRegex(R"(\s*CREATE\s+TABLE\s+(\w+)\s*\()");
        std::smatch match;
        if (std::regex_search(commandString, match, tableNameRegex)) {
            tableName = match[1];
        }
        else {
            std::cout << "Error: Missing or invalid table name." << std::endl;
            return;
        }

        // Validate if the table name is alphanumeric
        if (!std::regex_match(tableName, std::regex("^[a-zA-Z0-9_]+$"))) {
            std::cout << "Error: Invalid characters in table name." << std::endl;
            return;
        }

        // Extract and validate column definitions
        std::string columnDefinitions;
        std::regex columnDefinitionsRegex(R"(\((.*?)\))");
        if (std::regex_search(commandString, match, columnDefinitionsRegex)) {
            columnDefinitions = match[1];
        }
        else {
            std::cout << "Error: Missing or invalid column definitions." << std::endl;
            return;
        }

        // Split column definitions into individual columns
        std::vector<std::string> columns;
        std::regex columnsRegex(R"(\s*(\w+)\s+(\w+)\s*(,\s*\w+\s+(\w+))*\s*\)");
        auto columnIterator = std::sregex_iterator(columnDefinitions.begin(), columnDefinitions.end(), columnsRegex);
        auto endIterator = std::sregex_iterator();
        for (; columnIterator != endIterator; ++columnIterator) {
            columns.push_back((*columnIterator)[0]);
        }

        // Validate each column definition
        for (const auto& column : columns) {
            // Replace with the appropriate function for validating column definitions
            std::cout << "Column definition: " << column << std::endl;
        }

        std::cout << "CREATE TABLE command is valid." << std::endl;
    }

    void ExecuteCommand() {
        std::cout << std::endl << "Executing command: " << commandString;
        // I will parse all the commands and execute specific tasks after validations and 
        // using methods, but right now I do not feel like I possess the necassary knowledge
    }

    void DisplayCommandInfo() {
        std::cout << std::endl << "Command Info: ";
        std::cout << std::endl << "Command String: " << commandString;
        // I will also display the command parameters but for now it looks decent
    }


};

class ColumnDefinition {
    char* columnName;
    char* dataType;

public:
    void SetColumnName(const char* name)
    {
        delete[] this->columnName;
        columnName = new char[strlen(name) + 1];
        strcpy_s(this->columnName, strlen(name) + 1, name);
    }

    void SetDataType(const char* type)
    {
        delete[] this->dataType;
        dataType = new char[strlen(type) + 1];
        strcpy_s(this->dataType, strlen(type) + 1, type);
    }

    char* GetColumnName()
    {
        char* valueOfColumnName = new char[strlen(this->columnName) + 1];
        strcpy_s(valueOfColumnName, strlen(this->columnName) + 1, this->columnName);
        return valueOfColumnName;
    }

    char* GetDataType()
    {
        char* valueOfDataType = new char[strlen(this->dataType) + 1];
        strcpy_s(valueOfDataType, strlen(this->dataType) + 1, dataType);
        return valueOfDataType;
    }

    ColumnDefinition(const char* name, const char* type)
    {
        this->SetColumnName(name);
        this->SetDataType(type);
    }

    ColumnDefinition()
    {

    }

    ~ColumnDefinition()
    {
        delete[] this->dataType;
        delete[] this->columnName;
    }
};

class TableCreator {
    char* tableName = nullptr;
    static const int MAX_COLUMNS = 10;
    ColumnDefinition columnDefinitions[MAX_COLUMNS];
    int numColumns = 0;

public:
    void SetTableName(const char* newTableName)
    {
        delete[] this->tableName;
        tableName = new char[strlen(newTableName) + 1];
        strcpy_s(this->tableName, strlen(newTableName) + 1, newTableName);
    }

    void SetColumnDefinitions(const char* columnName, const char* dataType)
    {
        if (numColumns < MAX_COLUMNS)
        {
            this->columnDefinitions[numColumns].SetColumnName(columnName);
            this->columnDefinitions[numColumns].SetDataType(dataType);
            ++numColumns;
        }
        else
        {
            std::cerr << std::endl << "Error: Max columns already reached.";
        }
    }

    char* GetTableName()
    {
        char* valueOfTableName = new char[strlen(this->tableName) + 1];
        strcpy_s(valueOfTableName, strlen(this->tableName) + 1, this->tableName);
        return valueOfTableName;
    }

    ColumnDefinition* GetColumnDefinitions()
    {
        return columnDefinitions;
    }

    void CreateTable()
    {
        std::cout << std::endl << std::endl << "Creating table: " << this->tableName;
        std::cout << std::endl << std::endl << "Column Definitions:";
        for (int i = 0; i < this->numColumns; ++i) {
            std::cout << std::endl << "Column Name: " << this->columnDefinitions[i].GetColumnName()
                << ", Data Type: " << this->columnDefinitions[i].GetDataType();
        }
    }

    void DisplayTableInfo()
    {
        std::cout << std::endl << std::endl << "Table Info:";
        std::cout << std::endl << "Table Name: " << this->tableName;
        std::cout << std::endl << "Number of Columns: " << this->numColumns;
    }

    TableCreator() 
    {
        
    }

    ~TableCreator() {
        delete[] this->tableName;
    }
};

class QueryBuilder
{
    std::string queryString = "";
    static const int MAX_COLUMNS = 10;
    char* selectedColumns[MAX_COLUMNS] = { 0 };
    int numColumns = 0;

public:
    static const int MIN_QUERY_SIZE = 4;
    void SetQueryString(std::string newQueryString)
    {
        if (newQueryString.size() < MIN_QUERY_SIZE)
            throw std::exception("Query too short");
        this->queryString = newQueryString;
    }

    void SetSelectedColumns(const char* columnName) 
    {
        if (this->numColumns < MAX_COLUMNS)
        {
            this->selectedColumns[numColumns] = new char[strlen(columnName) + 1];
            strcpy_s(this->selectedColumns[numColumns], strlen(columnName) + 1, columnName);
            ++numColumns;
        }
        else
        {
            std::cerr << std::endl << "Max number of selected columns reached";
        }
    }

    std::string GetQueryString()
    {
        return this->queryString;
    }

    char* GetSelectedColumns()
    {
        char* valueOfSelectedColumns = new char[strlen(this->selectedColumns[numColumns]) + 1];
        strcpy_s(valueOfSelectedColumns, strlen(this->selectedColumns[numColumns] + 1), this->selectedColumns[numColumns]);
        return valueOfSelectedColumns;
    }

    void BuildQuery()
    {
        std::cout << std::endl << std::endl << "Building query: " << this->queryString;
        std::cout << std::endl << "Selected Columns: ";
        for (int i = 0; i < this->numColumns; ++i) {
            std::cout << this->selectedColumns[i] << " ";
        }
        std::cout << std::endl;
    }

    void DisplayQueryInfo()
    {
        std::cout << std::endl << std::endl << "Query Info:";
        std::cout << std::endl << "Query String: " << this->queryString;
        std::cout << std::endl << "Number of Selected Columns: " << this->numColumns;
        std::cout << std::endl;
    }

    QueryBuilder()
    {
        
    }

    ~QueryBuilder()
    {
        for (int i = 0; i < this->numColumns; ++i) {
            delete[] this->selectedColumns[i];
        }
    }

    QueryBuilder(QueryBuilder& other) {
        this->queryString = other.queryString;
        this->numColumns = other.numColumns;
        for (int i = 0; i < this->numColumns; ++i) {
            this->selectedColumns[i] = new char[strlen(other.selectedColumns[i]) + 1];
            strcpy_s(this->selectedColumns[i], strlen(other.selectedColumns[i]) + 1, other.selectedColumns[i]);
        }
    }
};

int main()
{
    try {
        CommandProcessor cmd1("CREATE TABLE employees (id INT, name TEXT, salary FLOAT);");
        CommandProcessor cmd2("INSERT INTO employees VALUES (1, 'John Doe', 50000.0);");
        CommandProcessor cmd3;

        std::cout << "Initial Commands:" << std::endl;
        std::cout << "cmd1: " << cmd1.GetCommandString() << std::endl;
        std::cout << "cmd2: " << cmd2.GetCommandString() << std::endl;
        std::cout << "cmd3: " << (cmd3.GetCommandString() ? cmd3.GetCommandString() : "Empty Command") << std::endl;

        cmd3.SetCommandString("SELECT * FROM products;");
        cmd3.IdentifyCommandType();
        cmd3.ValidateCommand();
        cmd3.DisplayCommandInfo();
        cmd3.ExecuteCommand();

        TableCreator myTable;
        myTable.SetTableName("ExampleTable");
        myTable.SetColumnDefinitions("Column1", "INT");
        myTable.SetColumnDefinitions("Column2", "VARCHAR");

        myTable.CreateTable();
        myTable.DisplayTableInfo();

        QueryBuilder myQuery;
        myQuery.SetQueryString("SELECT");
        myQuery.SetSelectedColumns("Column1");
        myQuery.SetSelectedColumns("Column2");

        myQuery.BuildQuery();
        myQuery.DisplayQueryInfo();

        QueryBuilder myQueryCopy(myQuery);
        std::cout << "\nCopied Query:";
        myQueryCopy.BuildQuery();
        myQueryCopy.DisplayQueryInfo();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
