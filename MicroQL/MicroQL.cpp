// MicroQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Storage\FileManager.h"
#include "Storage\StorageManager.h"
#include "Database\TableDefinition.h";
#include "Database\TableDictionary.h";

#include "Database\MQLColumn.h";

#include <iostream>
#include <fstream>
int _tmain(int argc, _TCHAR* argv[])
{
	
	string path = "c:\\study\\cs4221\\hellodb.txt";
	FileManager::createAFile(path);
	
	FileManager * fm = new FileManager(path);
	StorageManager *sm  = new StorageManager(fm);

	TableDefinition *td = new TableDefinition("EmployeeInformation");
	string columnName = "employeeid";
	MQLColumn column1(columnName, CT_VARCHAR, 20, true);

	string columnName2 = "age";
	MQLColumn column2(columnName2, CT_VARCHAR, 10, false);
	
	td->addColumn(column1);
	td->addColumn(column2);

	TableDefinition *td2 = new TableDefinition("Sale");
	columnName = "saleid";
	MQLColumn column3(columnName, CT_VARCHAR, 28, true);

	columnName2 = "qty";
	MQLColumn column4(columnName2, CT_VARCHAR, 30, false);
	
	td2->addColumn(column3);
	td2->addColumn(column4);
	
	
	TableDictionary *tdictionary = new TableDictionary(fm, sm);
	tdictionary->storeTableToDictionary(td);
	tdictionary->storeTableToDictionary(td2);

	TableDictionary *tdictionary2 = new TableDictionary(fm, sm);
	
	tdictionary2->columnExists("a");

	return 0;
}

