#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <exception>
#include <regex>
#include <sstream>

enum RelationType{ UNDEFINED, ONE_TO_ONE, ONE_TO_MANY, MANY_TO_ONE };

class Database
{
    //std::string tableName = "";
    std::string* tables = nullptr;
    int noTables=0;
    RelationType linkType = RelationType::UNDEFINED;
    std::string** relationsMatrix = nullptr;
    int noRelations=0;

public:
    static const int MIN_NO_TABLES = 1;
    static const int MIN_TABLE_NAME_LENGTH = 3;
    static const int NO_COLUMNS_RELATION_MATRIX = 3;

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
        this->noRelations = object.noRelations;

        this->relationsMatrix = new std::string * [this->noRelations];
        for (int i = 0; i < this->noRelations; i++)
        {
            this->relationsMatrix[i] = new std::string[NO_COLUMNS_RELATION_MATRIX];
        }
        for (int i = 0; i < object.noRelations; i++)
        {
            for (int j = 0; j < NO_COLUMNS_RELATION_MATRIX; j++)
                this->relationsMatrix[i][j] = object.relationsMatrix[i][j];
        }
    }

    //destructor
    ~Database()
    {
        delete[] this->tables;
        for (int i = 0; i < this->noRelations; i++)
        {
            delete[] this->relationsMatrix[i];
        }
        delete[] this->relationsMatrix;
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
        //std::cout << "Relation type: "<< this->showRelationType()<<std::endl;
        if (this->relationsMatrix != nullptr && this->noRelations > 0)
        {
            for (int i = 0; i < this->noRelations; i++)
            {
                std::cout << "The tables " << relationsMatrix[i][0] << " and " << relationsMatrix[i][1] << " have a " << relationsMatrix[i][2] << " relation."<<std::endl;
            }
        }
        else std::cout << "There are no relations established between any of your tables."<<std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
    }

    std::string** copyMatrixOfStrings(std::string** matrix, int rows)
    {
        std::string** copy = new std::string * [rows];
        for (int i = 0; i < rows; i++)
            copy[i] = new std::string[NO_COLUMNS_RELATION_MATRIX];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < NO_COLUMNS_RELATION_MATRIX; j++)
                copy[i][j] = matrix[i][j];
        }

        return copy;
    }


    std::string showRelationType(RelationType type)
    {
        switch (type)
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

    bool checkTableName(std::string table)
    {
        for (int i = 0; i < this->noTables; i++)
        {
            if (this->tables[i] == table)
                return true;
        }
        return false;
    }

    void establishRelation(const std::string table1, const std::string table2, RelationType type)
    {
        if (checkTableName(table1) && checkTableName(table2))
        {
            //copy initial matrix in order to preserve it's elements
            std::string** copy = copyMatrixOfStrings(this->relationsMatrix,this->noRelations);

            //allocate memory for the new matrix
            this->noRelations++;
            std::string** newMatrix = new std::string * [this->noRelations];
            for (int i = 0; i < this->noRelations; i++)
            {
                newMatrix[i] = new std::string[NO_COLUMNS_RELATION_MATRIX];
            }

            for (int i = 0; i < this->noRelations - 1; i++)
            {
                for (int j = 0; j < NO_COLUMNS_RELATION_MATRIX; j++)
                    newMatrix[i][j] = copy[i][j];
            }

            //dealocate memory for the copy
            for (int i = 0; i < this->noRelations-1; i++)
            {
                delete[] copy[i];
            }
            delete[] copy;

            //add new relation 
            newMatrix[this->noRelations - 1][0] = table1;
            newMatrix[this->noRelations - 1][1] = table2;
            newMatrix[this->noRelations - 1][2] = showRelationType(type);

            for (int i = 0; i < this->noRelations - 1; i++)
            {
                delete[] this->relationsMatrix[i];
            }
            delete[] this->relationsMatrix;

            this->relationsMatrix = copyMatrixOfStrings(newMatrix, this->noRelations);

            for (int i = 0; i < this->noRelations; i++)
            {
                delete[] newMatrix[i];
            }
            delete[] newMatrix;
        }
        else
        {
            throw std::exception("The name doesn t match any tables from this database.");
        }
    }

    //operators
    Database operator=(const Database source)
    {
        if (&source == this)
        {
            return *this;
        }
        else
        {
            this->linkType = source.linkType;
            this->setTables(source.tables, source.noTables);
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& console, Database database);

};

std::ostream& operator<<(std::ostream& console, Database database)
{
    console << std::endl;
    console << "Database info: " << std::endl;
    console << "Number of tables: " << database.getNoTables()<<std::endl;

    if (database.getTables() != nullptr && database.getNoTables() > 0)
    {
        console << "The names of your tables are: ";
        std::string* tableArray = database.getTables();
        for (int i = 0; i < database.getNoTables(); i++)
        {
            console << tableArray[i] << " ";
        }
        console << std::endl;

        if (database.relationsMatrix != nullptr && database.noRelations > 0)
        {
            for (int i = 0; i < database.noRelations; i++)
            {
                console << "Tables " << database.relationsMatrix[i][0] << " and " << database.relationsMatrix[i][1] << " have a " << database.relationsMatrix[i][2] << " relation." << std::endl;
            }
        }
        else console << std::endl << "There are no relations established between any of your tables.";
        console << std::endl;
    }
    else console << "There are no tables in your database."<<std::endl;
    
    return console;
}

std::istream& operator>>(std::istream& console, Database& database)
{
    std::cout << "Introduce the number of tables for your databse: ";
    int noTables;
    console >> noTables;
    while (noTables < 1)
    {
        std::cout << "The number can t be negative or 0 please try again: ";
        console >> noTables;
    }
    std::cout << "Introduce the name for these tables: ";
    std::string* tables = new std::string[noTables];
    for (int i = 0; i < noTables; i++)
    {
        console >> tables[i];
    }
    database.setTables(tables, noTables);

    return console;
}

class IndexManager
{
    std::string indexName = "";
    int* indexedColumns = nullptr;
    int noColumns = 0;

public:
    static const int MIN_INDEXED_COLUMNS = 1;
    static const int MIN_INDEX_NAME_LENGTH = 2;

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
            std::cout << std::endl << "Index Name: " << this->indexName << std::endl;
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

    void addIndex(int newIndex)
    {
        if (newIndex < 1)
        {
            throw std::exception("Invalid index number was introduced.");
        }
        int* copy = new int[this->noColumns + 1];
        for (int i = 0; i < this->noColumns; i++)
        {
            copy[i] = this->indexedColumns[i];
        }
        copy[this->noColumns] = newIndex;
        delete[] this->indexedColumns;
        indexedColumns = copy;

        this->noColumns++;
    }

    void subtractIndex(int index)
    {
        bool found = false;
        int subtract = 0;
        for (int i = 0; i < this->noColumns; i++)
        {
            if (this->indexedColumns[i] == index)
            {
                found = true;
                subtract = i;
                break;
            }
        }
        if (!found)
        {
            throw std::exception("This index does not exist.");
        }

        // Checked if there's only one element and handled separately
        if (this->noColumns == 1)
        {
            delete[] this->indexedColumns;
            this->indexedColumns = nullptr;
            this->noColumns = 0;
            return;
        }

        int* copy = new int[this->noColumns-1];
        //int j = 0;
        for (int i = 0, j = 0; i < this->noColumns; i++)
        {
            if (i != subtract)
            {
                copy[j++] = this->indexedColumns[i];
                //j++;
            }

        }
        delete[] this->indexedColumns;
        this->indexedColumns = copy;

        this->noColumns--;
    }

    //operators
    IndexManager operator=(const IndexManager source)
    {
        if (&source == this)
        {
            return *this;
        }
        else
        {
            this->setIndexName(source.indexName);
            this->setIndexedColumns(source.indexedColumns, source.noColumns);
        }
        return *this;
    }

};

class NumericalDataManipulator
{
    std::string columnName = "";
    float* dataValues=nullptr;
    int noValues;

    static const int MIN_NAME_LENGHT = 2;
    static const int MIN_NO_INPUTS = 1;

public:

    //getters
    std::string getColumnName()
    {
        return this->columnName;
    }

    float* getDataValues()
    {
        float* copy = new float[this->noValues];
        for (int i = 0; i < this->noValues; i++)
        {
            copy[i] = this->dataValues[i];
        }
        return copy;
    }

    int getNoValues()
    {
        return this->noValues;
    }

    //setters
    void setColumnName(std::string name)
    {
        if (name.length() < MIN_NAME_LENGHT)
        {
            throw std::exception("The column name must have at least 2 characters.");
        }
        this->columnName = name;
    }

    void setDataValues(float* data, int noInputs)
    {
        if (this->dataValues != nullptr)
        {
            delete[] this->dataValues;
        }

        if (noInputs < MIN_NO_INPUTS)
        {
            throw std::exception("There were no inputs passed on.");
        }

        this->dataValues = new float[noInputs];
        for (int i = 0; i < noInputs; i++)
        {
            this->dataValues[i] = data[i];
        }

        this->noValues = noInputs;
    }

    //constructor
    NumericalDataManipulator()
    {

    }

    NumericalDataManipulator(std::string columnName)
    {
        this->setColumnName(columnName);
    }

    NumericalDataManipulator(std::string columnName, float* dataValues, int noValues): columnName(columnName)
    {
        this->setDataValues(dataValues, noValues);
    }

    //copyctr
    NumericalDataManipulator(const NumericalDataManipulator& object): columnName(object.columnName)
    {
        this->setDataValues(object.dataValues, object.noValues);
    }

    //destructor
    ~NumericalDataManipulator()
    {
        delete[] this->dataValues;
    }

    //methods
    float maxValue()
    {
        if (this->dataValues == nullptr || this->noValues < MIN_NO_INPUTS)
        {
            throw std::exception("No data was passed.");
        }

        float max = this->dataValues[0];
        for (int i = 1; i < this->noValues; i++)
        {
            if (this->dataValues[i] > max)
                max = this->dataValues[i];
        }
        return max;
    }

    float minValue()
    {
        if (this->dataValues == nullptr || this->noValues < MIN_NO_INPUTS)
        {
            throw std::exception("No data was passed.");
        }

        float min = this->dataValues[0];
        for (int i = 1; i < this->noValues; i++)
        {
            if (this->dataValues[i] < min)
                min = this->dataValues[i];
        }
        return min;
    }

    void displayNumericalData()
    {
        std::cout << std::endl << "Column name: " << this->columnName<<std::endl;
        std::cout << "The values are: ";
        for (int i = 0; i < this->noValues; i++)
        {
            std::cout << this->dataValues[i] << " ";
        }
    }

    void ascendingOrder()
    {
        if (this->dataValues == nullptr || this->noValues < MIN_NO_INPUTS)
        {
            throw std::exception("No data was passed.");
        }

        ascendingOrderRecursive(0, this->noValues - 1);
    }
    
    void ascendingOrderRecursive(int first, int last)
    {
        if (first < last)
        {
            float pivot = this->dataValues[last];
            int i = first - 1;

            for (int j = first; j < last; j++)
            {
                if (this->dataValues[j] <= pivot)
                {
                    i++;
                    std::swap(this->dataValues[i], this->dataValues[j]);
                }
            }

            std::swap(this->dataValues[last], this->dataValues[i + 1]);

            ascendingOrderRecursive(first, i);
            ascendingOrderRecursive(i + 2, last);
        }
    }

    void descendingOrder()
    {
        if (this->dataValues == nullptr && this->noValues < this->MIN_NO_INPUTS)
        {
            throw std::exception("No data was passed.");
        }

        descendingOrderRecursive(0, this->noValues-1);
    }

    void descendingOrderRecursive(int first, int last)
    {
        if (first < last)
        {
            float pivot = this->dataValues[last];
            int i = first - 1;
            for (int j = first; j < last; j++)
            {
                if (this->dataValues[j] >= pivot)
                {
                    i++;
                    std::swap(this->dataValues[i],this->dataValues[j]);
                }
            }
            std::swap(this->dataValues[i+1],this->dataValues[last]);

            descendingOrderRecursive(first, i);
            descendingOrderRecursive(i + 2, last);
        }
    }

    //operators
    NumericalDataManipulator operator=(const NumericalDataManipulator source)
    {
        if (&source == this)
        {
            return *this;
        }
        else
        {
            this->setColumnName(source.columnName);
            this->setDataValues(source.dataValues, source.noValues);
        }
        return *this;
    }
};

class CommandProcessor {
    char* commandString = nullptr;
    static const int COMMAND_TYPES_COUNT = 9;
    const char* commandTypes[COMMAND_TYPES_COUNT] = { "CREATE TABLE", "CREATE INDEX", "DROP TABLE",
    "DROP INDEX", "DISPLAY TABLE", "INSERT", "SELECT", "UPDATE", "DELETE" };

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

    bool ExtractTableName(const std::string& insertCommand, std::string& tableName) {
        std::regex tableNameRegex(R"(\s*INSERT\s+INTO\s+(\w+)\s+VALUES)");
        std::smatch match;
        if (std::regex_search(insertCommand, match, tableNameRegex)) {
            tableName = match[1];
            return true;
        }
        else {
            std::cout << "Error: Missing or invalid table name." << std::endl;
            return false;
        }
    }

    bool ExtractAndValidateValues(const std::string& insertCommand, std::string values[], int& numValues) {
        std::regex valuesRegex(R"(\((.*?)\))");
        std::smatch match;
        if (std::regex_search(insertCommand, match, valuesRegex)) {
            std::string valuesString = match[1];

            // Split values using comma as a delimiter
            std::istringstream iss(valuesString);
            std::string value;
            int maxValues = 100; // I will adjust this as I see fit, ATM it is ok
            while (std::getline(iss, value, ',')) {
                // Remove leading and trailing whitespaces
                value = std::regex_replace(value, std::regex("^\\s+|\\s+$"), "");
                if (numValues < maxValues) {
                    values[numValues++] = value;
                }
                else {
                    std::cout << "Error: Exceeded maximum number of values." << std::endl;
                    return false;
                }
            }

            // Validate the number of values
            if (numValues == 0) {
                std::cout << "Error: No values provided." << std::endl;
                return false;
            }

            return true;
        }
        else {
            std::cout << "Error: Missing or invalid values." << std::endl;
            return false;
        }
    }

    bool ExtractColumnNames(const std::string& selectCommand, std::string& columnNames) {
        std::regex columnNamesRegex(R"(\s*SELECT\s+(ALL|\([\w\s,]+\))\s+FROM)");
        std::smatch match;
        if (std::regex_search(selectCommand, match, columnNamesRegex)) {
            columnNames = match[1];
            return true;
        }
        else {
            std::cout << "Error: Missing or invalid column names." << std::endl;
            return false;
        }
    }

    bool ExtractFilterCriteria(const std::string& selectCommand, std::string& filterColumnName, std::string& filterColumnValue) {
        std::regex filterCriteriaRegex(R"(\s*WHERE\s+(\w+)\s*=\s*'([^']+)'\s*)");
        std::smatch match;
        if (std::regex_search(selectCommand, match, filterCriteriaRegex)) {
            filterColumnName = match[1];
            filterColumnValue = match[2];
            return true;
        }
        else {
            std::cout << "Error: Missing or invalid filter criteria." << std::endl;
            return false;
        }
    }

    //table names for insert etc to bo resolved

    bool ExtractDeleteTableName(const std::string& deleteCommand, std::string& tableName) {
        std::regex tableNameRegex(R"(\s*FROM\s+(\w+)\s+WHERE)");
        std::smatch match;
        if (std::regex_search(deleteCommand, match, tableNameRegex)) {
            tableName = match[1];
            return true;
        }
        else {
            std::cout << "Error: Missing or invalid table name." << std::endl;
            return false;
        }
    }

    bool ExtractDeleteFilterCriteria(const std::string& deleteCommand, std::string& filterColumnName, std::string& filterColumnValue) {
        std::regex filterCriteriaRegex(R"(\s*WHERE\s+(\w+)\s*=\s*'([^']+)'\s*)");
        std::smatch match;
        if (std::regex_search(deleteCommand, match, filterCriteriaRegex)) {
            filterColumnName = match[1];
            filterColumnValue = match[2];
            return true;
        }
        else {
            std::cout << "Error: Missing or invalid filter criteria." << std::endl;
            return false;
        }
    }

    // Helper function to validate the UPDATE command
    void ValidateUpdateCommand(const std::string& updateCommand) {
        // Validate the basic syntax of the UPDATE command
        std::regex updateRegex(R"(\s*UPDATE\s+(\w+)\s+SET\s+(\w+)\s*=\s*'([^']+)'\s+WHERE\s+(\w+)\s*=\s*'([^']+)'\s*)");
        std::smatch match;
        if (std::regex_match(updateCommand, match, updateRegex)) {
            // Extract and validate the table name, column name, value, filter column, and filter value
            std::string tableName = match[1];
            std::string columnName = match[2];
            std::string columnValue = match[3];
            std::string filterColumn = match[4];
            std::string filterValue = match[5];

            ValidateTableName(tableName);
            ValidateColumnName(columnName);
            ValidateColumnValue(columnValue);
            ValidateFilterColumn(filterColumn);
            ValidateFilterValue(filterValue);
        }
        else {
            std::cout << "Error: Invalid UPDATE command format." << std::endl;
        }
    }

    // Helper function to validate the table name
    void ValidateTableName(const std::string& tableName) {
        // Validate if the table name is alphanumeric
        if (!std::regex_match(tableName, std::regex("^[a-zA-Z0-9_]+$"))) {
            std::cout << "Error: Invalid characters in table name." << std::endl;
        }
    }

    // Helper function to validate the column value
    void ValidateColumnValue(const std::string& value) {
        // No specific validation for column value in this example
    }

    // Helper function to validate the filter column
    void ValidateFilterColumn(const std::string& filterColumn) {
        // Validate if the filter column name is alphanumeric
        if (!std::regex_match(filterColumn, std::regex("^[a-zA-Z0-9_]+$"))) {
            std::cout << "Error: Invalid characters in filter column name." << std::endl;
        }
    }

    // Helper function to validate the filter value
    void ValidateFilterValue(const std::string& filterValue) {
        // No specific validation for filter value in this example
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

    void ValidateDropTableCommand(const std::string& commandString) {
        // Validate if the command contains "DROP TABLE"
        if (commandString.find("DROP TABLE") == std::string::npos) {
            std::cout << "Error: Missing 'DROP TABLE' keyword." << std::endl;
            return;
        }

        // Extract the table name
        std::string tableName;
        std::regex tableNameRegex(R"(\s*DROP\s+TABLE\s+(\w+))");
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

        // Check for additional characters after the table name
        size_t pos = commandString.find(tableName) + tableName.length();
        std::string remaining = commandString.substr(pos);
        if (!remaining.empty() && !std::all_of(remaining.begin(), remaining.end(), ::isspace)) {
            std::cout << "Error: Extra characters after table name." << std::endl;
            return;
        }

        std::cout << "DROP TABLE command is valid. Table to delete: " << tableName << std::endl;
    }

    void ValidateDisplayTableCommand(const std::string& commandString) {
        // Validate if the command contains "DISPLAY TABLE"
        if (commandString.find("DISPLAY TABLE") == std::string::npos) {
            std::cout << "Error: Missing 'DISPLAY TABLE' keywords." << std::endl;
            return;
        }

        // Extract the table name
        std::string tableName;
        std::regex tableNameRegex(R"(\s*DISPLAY\s+TABLE\s+(\w+))");
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

        // Check for additional characters after the table name
        size_t pos = commandString.find(tableName) + tableName.length();
        std::string remaining = commandString.substr(pos);
        if (!remaining.empty() && !std::all_of(remaining.begin(), remaining.end(), ::isspace)) {
            std::cout << "Error: Extra characters after table name." << std::endl;
            return;
        }

        std::cout << "DISPLAY TABLE command is valid. Table to display: " << tableName << std::endl;
    }

    void ValidateInsertCommand(const std::string& insertCommand) {
        // Check if the command starts with "INSERT INTO"
        if (insertCommand.find("INSERT INTO") == std::string::npos) {
            std::cout << "Error: Missing 'INSERT INTO' keyword." << std::endl;
            return;
        }

        // Extract table name
        std::string tableName;
        if (!ExtractTableName(insertCommand, tableName)) {
            return;
        }

        // Extract and validate values
        const int maxValues = 10; // Adjust the maximum number of values as needed
        std::string values[maxValues];
        int numValues = 0;
        if (!ExtractAndValidateValues(insertCommand, values, numValues)) {
            return;
        }

        // Optional: Correlate values with column sequence from CREATE (not implemented)

        // Display information
        std::cout << "INSERT command is valid." << std::endl;
        std::cout << "Table: " << tableName << std::endl;
        std::cout << "Number of values: " << numValues << std::endl;
        for (int i = 0; i < numValues; ++i) {
            std::cout << "Value for column " << i + 1 << ": " << values[i] << std::endl;
        }
    }

    void ValidateSelectCommand(const std::string& selectCommand) {
        // Check if the command starts with "SELECT"
        if (selectCommand.find("SELECT") == std::string::npos) {
            std::cout << "Error: Missing 'SELECT' keyword." << std::endl;
            return;
        }

        // Extract column names, ALL or specific columns
        std::string columnNames;
        if (!ExtractColumnNames(selectCommand, columnNames)) {
            return;
        }

        // Extract table name
        std::string tableName;
        if (!ExtractTableName(selectCommand, tableName)) {
            return;
        }

        // Check if WHERE clause is present
        bool hasWhereClause = (selectCommand.find("WHERE") != std::string::npos);

        // Optional: Extract and validate filter criteria (not implemented)
        std::string filterColumnName;
        std::string filterColumnValue;
        if (hasWhereClause) {
            if (!ExtractFilterCriteria(selectCommand, filterColumnName, filterColumnValue)) {
                return;
            }
        }

        // Display information
        std::cout << "SELECT command is valid." << std::endl;
        std::cout << "Table: " << tableName << std::endl;
        std::cout << "Columns: " << columnNames << std::endl;
        std::cout << "Filter: " << (hasWhereClause ? "yes" : "no") << std::endl;
        if (hasWhereClause) {
            std::cout << "Filter Column: " << filterColumnName << " with value: " << filterColumnValue << std::endl;
        }
    }

    void ValidateDeleteCommand(const std::string& deleteCommand) {
        // Check if the command starts with "DELETE"
        if (deleteCommand.find("DELETE") == std::string::npos) {
            std::cout << "Error: Missing 'DELETE' keyword." << std::endl;
            return;
        }

        // Extract table name
        std::string tableName;
        if (!ExtractTableName(deleteCommand, tableName)) {
            return;
        }

        // Check if WHERE clause is present
        bool hasWhereClause = (deleteCommand.find("WHERE") != std::string::npos);

        // Optional: Extract and validate filter criteria (not implemented)
        std::string filterColumnName;
        std::string filterColumnValue;
        if (hasWhereClause) {
            if (!ExtractFilterCriteria(deleteCommand, filterColumnName, filterColumnValue)) {
                return;
            }
        }
        else {
            std::cout << "Error: Missing 'WHERE' keyword." << std::endl;
            return;
        }

        // Display information
        std::cout << "DELETE command is valid." << std::endl;
        std::cout << "Table: " << tableName << std::endl;
        std::cout << "Filter Column: " << filterColumnName << " with value: " << filterColumnValue << std::endl;
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
    
    int noTables = 3; 
    std::string* tables = new std::string[noTables];
    tables[0] = "Employees";
    tables[1] = "Managers";
    tables[2] = "Items Sold";

    try {
         
        //test set and get methods
        /*myDatabase.setTables(tables, noTables);
        std::string* retrievedTables = myDatabase.getTables();
        for (int i = 0; i < myDatabase.getNoTables(); i++) {
            std::cout << "Table " << i + 1 << ": " << retrievedTables[i] << std::endl;
        }*/

        Database db(tables, noTables);
        db.displayDatabaseInfo();
        myDatabase = db;
        myDatabase.establishRelation(tables[0], tables[1], RelationType::ONE_TO_MANY);
        myDatabase.displayDatabaseInfo();
        myDatabase.establishRelation("Employees", "Items Sold", RelationType::ONE_TO_MANY);
        myDatabase.displayDatabaseInfo();

        std::cout << myDatabase;
        std::cin >> db;
        std::cout << db;
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
    std::cout << std::endl;
    try {
        myIndex.setIndexName("Salary");
        std::cout << myIndex.getIndexName() << std::endl;

        int noColumns = 3;
        int indices[] = { 2,3,5 };
        myIndex.setIndexedColumns(indices, noColumns);
        std::cout << "Number of Columns:" << myIndex.getNoColumns() << std::endl;

        int* retrievedColumns = myIndex.getIndexedColumns();
        for (int i = 0; i < noColumns; i++)
        {
            std::cout << retrievedColumns[i] << " ";
        }

        delete[] retrievedColumns;
        std::cout << std::endl;

        IndexManager newIndex("Salary", indices, noColumns);
        newIndex.displayIndexInfo();

        newIndex.addIndex(8);
        newIndex.displayIndexInfo();
        newIndex.subtractIndex(2);
        newIndex.displayIndexInfo();

        std::cout<<::std::endl << "initial values";
        myIndex.displayIndexInfo();
        std::cout << ::std::endl << "new stuff";
        myIndex = newIndex;
        myIndex.displayIndexInfo();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try 
    {
        float testData[] = { 14.2, 3.5, 7, 8.9 };
        NumericalDataManipulator price("price", testData, 4);
        std::cout << price.maxValue() << std::endl;
        std::cout << price.minValue();
        price.ascendingOrder();
        price.displayNumericalData();
        price.descendingOrder();
        price.displayNumericalData();

        NumericalDataManipulator test("test");
        test = price;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error " << e.what()<<std::endl;
    }

    return 0;
}
