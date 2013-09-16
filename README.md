MicroQL
=======

Simple database engine that runs. 


Assumptions
===========
1. Each table will has different columnname with each other table. 
2. Only varchar is supported.

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
      addColumns(columnName)
      addTable(string tableName)
      addCondition(conditionList)
  4. Update
  5. Delete
  6. ResultTable 
      Populate()
      Merge(ResulTable, conditionList)
      getTotalRows()
      getItemValue(string columnName, int rowIndex)
