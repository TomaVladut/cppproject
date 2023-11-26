#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <exception>

enum RelationType{ UNDEFINED, ONE_TO_ONE, ONE_TO_MANY, MANY_TO_ONE };

class Database
{
    //std::string tableName = "";
    std::string* tables = nullptr;
    int noTables=0;
    RelationType linkType = RelationType::UNDEFINED;

public:
    static const int MIN_NO_TABLES = 1;
    static const int MIN_TABLE_NAME_LENGTH = 3;

public:
    //getters
    std::string* getTables()
    {
       std::string* copy = new std::string[this->noTables];
        for (int i = 0; i < this->noTables; i++)
        {
            copy[i] = this->tables[i];
        }
        return copy;
        return tables;
    }

    int getNoTables()
    {
        return this->noTables;
    }

    RelationType getRelationType()
    {
        return this->linkType;
    }

    //setters
    void setTables(std::string* tablesArray, int noTables)
    {
        if (this->tables != nullptr)
        {
            delete[] this->tables;
        }
        if (noTables < this->MIN_NO_TABLES)
        {
            throw std::exception("No tables were sent.");
        }
        if (tablesArray->length() < MIN_TABLE_NAME_LENGTH)
        {
            throw std::exception("Table name is too short. ");
        }

        this->tables = new std::string[noTables];
        for (int i = 0; i < noTables; i++)
        {
            this->tables[i] = tablesArray[i];
        }
        this->noTables = noTables;
    }

    void setRelationType(RelationType type)
    {
        this->linkType = type;
    }

    std::string showRelationType()
    {
        switch (this->linkType)
        {
        case UNDEFINED:
            return "UNDEFINED";
        case ONE_TO_ONE:
            return "ONE_TO_ONE";
        case ONE_TO_MANY:
            return "ONE_TO_MANY";
        case MANY_TO_ONE:
            return"MANY_TO_ONE";
        }
    }

    //constructors
    Database()
    {

    }

    Database(std::string* tables, int noTables) :linkType(UNDEFINED)
    {
        this->setTables(tables, noTables);
    }

    //copy ctr
    Database(const Database& object)
    {
        this->tables = new std::string[object.noTables];
        for (int i = 0; i < object.noTables; i++)
        {
            this->tables[i] = object.tables[i];
        }
        this->noTables = object.noTables;
        this->linkType = object.linkType;
    }

    //destructor
    ~Database()
    {
        delete[] this->tables;
    }

    //methods
    void displayDatabaseInfo()
    {
        if (this->tables->length() > MIN_TABLE_NAME_LENGTH && this->noTables > MIN_NO_TABLES)
        {
            std::cout << "There are " << this->noTables << " tables. "<<std::endl;
            for (int i = 0; i < this->noTables; i++)
                std::cout << "Table: " << this->tables[i] << std::endl;
        }
        else std::cout << "There are no tables." << std::endl;
        std::cout << "Relation type: "<< this->showRelationType()<<std::endl;
    }

    //after consulting multilple internet sources i don t think i have the necessary knowledge to implement this method yet 
    void establishRelation(const std::string table1, const std::string table2, RelationType type)
    {

    }
};

class IndexManager
{
    std::string indexName = "";
    int* indexedColumns = nullptr;
    int noColumns = 0;

public:
    const int MIN_INDEXED_COLUMNS = 1;
    const int MIN_INDEX_NAME_LENGTH = 2;

    //getters

    std::string getIndexName()
    {
        return this->indexName;
    }

    int getNoColumns()
    {
        
        return this->noColumns;
    }

    int* getIndexedColumns()
    {
        if (this->indexedColumns == nullptr && this->noColumns < MIN_INDEXED_COLUMNS)
        {
            return nullptr;
        }

        int* copy = new int[this->noColumns];
        for (int i = 0; i < this->noColumns; i++)
        {
            copy[i] = this->indexedColumns[i];
        }
        return copy;
    }

    //setters
    void setIndexName(std::string name)
    {
        if (name.length() < MIN_INDEX_NAME_LENGTH)
        {
            throw std::exception("Index name has to have at least 3 characters");
        }
        this->indexName = name;
    }

    void setIndexedColumns(int* indexedColumns, int noColumns)
    {
        if (this->indexedColumns != nullptr)
            delete[] this->indexedColumns;
        if (noColumns < MIN_INDEXED_COLUMNS)
        {
            throw std::exception("No columns were passed.");
        }

        this->indexedColumns = new int[noColumns];
        for (int i = 0; i < noColumns; i++)
        {
            this->indexedColumns[i] = indexedColumns[i];
        }
        this->noColumns = noColumns;
    }

    //Constructors
    IndexManager()
    {

    }

    IndexManager(std::string newIndexName, int* indexColumns, int newNoColumns) :indexName(newIndexName)
    {
        this->setIndexedColumns(indexColumns, newNoColumns);
    }

    //copy ctr
    IndexManager(const IndexManager& object) :indexName(object.indexName)
    {
        this->setIndexedColumns(object.indexedColumns, object.noColumns);
    }

    //destructor
    ~IndexManager()
    {
        delete[] this->indexedColumns;
    }

    //methods
    void displayIndexInfo()
    {
        if (this->indexName.length() > MIN_INDEX_NAME_LENGTH)
            std::cout << "Index Name: " << this->indexName << std::endl;
        else throw std::exception("Index name has to have at least 3 characters");

        if (this->indexedColumns != nullptr && this->noColumns != 0)
        {
            std::cout << "Number of columns: " << this->noColumns << std::endl;
            std::cout << "Indices used: ";
            for (int i = 0; i < this->noColumns; i++)
            {
                std::cout << this->indexedColumns[i] << " ";
            }
        }
        else throw std::exception("No columns were passed.");
    }

};
    

class CommandProcessor {
    char* commandString = nullptr;
    static const int COMMAND_TYPES_COUNT = 9;
    const char* commandTypes[COMMAND_TYPES_COUNT] = { "CREATE TABLE", "CREATE INDEX", "DROP TABLE",
    "DROP INDEX", "DISPLAY TABLE", "INSERT", "SELECT", "UPDATE", "DELETE" };

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
        if (this->commandString != nullptr)
        {
            delete[] this->commandString;
        }
        this->commandString = new char[strlen(other.commandString) + 1];
        strcpy_s(this->commandString, strlen(other.commandString) + 1, other.commandString);
    }

    ~CommandProcessor() {
        delete[] commandString;
    }

    void SetCommandString(const char* newCommandString)
    {
        if (this->commandString != nullptr)
        {
            delete[] this->commandString;
        }
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
                std::cout << std::endl << "Command type: " << commandTypes[i];
                commandIdentified = true;
                break;
            }
        }
        if (!commandIdentified) {
            std::cout << "Error: Unrecognized command type." << std::endl;
        }
    }

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
    char* columnName = nullptr;
    char* dataType = nullptr;

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

    ColumnDefinition(const ColumnDefinition& object)
    {
        this->SetColumnName(object.columnName);
        this->SetDataType(object.dataType);
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

    ColumnDefinition GetColumnDefinitions()
    {
        ColumnDefinition copy;
        return copy;
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
        delete[] tableName;
    }

    TableCreator(const TableCreator& object)
    {
        this->SetTableName(object.tableName);
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
        myQuery.SetQueryString("INSERT");

        QueryBuilder myQueryCopy(myQuery);
        std::cout << "\nCopied Query:";
        myQueryCopy.BuildQuery();
        myQueryCopy.DisplayQueryInfo();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }



    Database myDatabase;
    
    int noTables;
    std::cout << std::endl<< "Number of tables for the database: ";
    std::cin >> noTables;
 
        std::string* tables = new std::string[noTables];
        for (int i = 0; i < noTables; i++)
        {
            std::cout << "add a table: ";
            std::cin >> tables[i];
        }
        std::cout << std::endl;
    try {
         
        //test set and get methods
        /*myDatabase.setTables(tables, noTables);
        std::string* retrievedTables = myDatabase.getTables();
        for (int i = 0; i < myDatabase.getNoTables(); i++) {
            std::cout << "Table " << i + 1 << ": " << retrievedTables[i] << std::endl;
        }*/

        Database db(tables, noTables);
        db.displayDatabaseInfo();
        }
    catch (const std::exception& e)
    {
        std::cerr<< "Error: " << e.what() << std::endl;
    }
    //test set get and show for enum
    /* myDatabase.setRelationType(ONE_TO_MANY);
    std::cout<<myDatabase.getRelationType();
    std::cout<<myDatabase.showRelationType()<<std::endl;*/

    IndexManager myIndex;
    try {
        myIndex.setIndexName("Salary");
        std::cout << myIndex.getIndexName() << std::endl;

        int noColumns = 3;
        int indices[] = { 2,3,5 };
        myIndex.setIndexedColumns(indices, noColumns);
        std::cout << "Number of Columns:" << myIndex.getNoColumns() << std::endl;

        int* retrieveColumns = myIndex.getIndexedColumns();
        for (int i = 0; i < noColumns; i++)
        {
            std::cout << retrieveColumns[i] << " ";
        }

        delete[] retrieveColumns;
        std::cout << std::endl;

        IndexManager newIndex("Salary", indices, noColumns);
        newIndex.displayIndexInfo();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;
}
