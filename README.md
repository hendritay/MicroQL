MicroQL
=======

Simple database engine that runs. 


Assumptions
===========
1. Each table has different columnname with each other table. 
2. Only varchar is supported.
3. No renaming allow in FROM clause.
4. INNER JOIN keyword must be used when there is a join between tables
```
CREATE syntax
 CREATE TABLE <tablename> (
    ColumnName1,
    [ColumnName2,]
    PK(ColumnName1)
  )
```  
  
Initial Class 
  1. Dictionary
      Contains attribute (TableName, ColumnName)
      Contains methods:
```
         ExistTable(String tableName) : boolean
         ExistsColumnName(String columnName) : boolean
```
            
  2. Table Class - For Creating Table)
      addColumn(Name, EnumColumnType)
      setPrimaryKey(Name)
  3. Select 
	  ResultTable evaluateQuery(string query)
  4. Update
  5. Delete
  6. ResultTable 
      Populate()
      Merge(ResulTable, conditionList)
      getTotalRows()
      getItemValue(string columnName, int rowIndex)
  7. QueryPlan
	  void addProjection(string columnName)
	  void addSelection(string tableName)
	  void addCondition(conditionList)