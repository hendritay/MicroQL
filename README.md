MicroQL
=======

Simple database engine that runs. 


Assumptions
===========
Each table will has different columnname with each other table. 
Only varchar is aparted.

CREATE syntax
 CREATE TABLE <tablename> (
    ColumnName1,
    [ColumnName2,]
    PK(ColumnName1)
  )
  
  
Initial Class 
  1. Dictionary
      Contains attribute (TableName, ColumnName)
      Contains methods:
         ExistTable(String tableName) : boolean
         ExistsColumnName(String columnName) : boolean

            
  2. Table 
  3. Select 
  4. Update
  5. Delete
  6. 
