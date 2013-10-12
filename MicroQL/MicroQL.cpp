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

#include "Select.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Select s;
	vector<string> validVec, invalidVec;
	bool b;

	// valid
	validVec.push_back("SeLect a from b");
	validVec.push_back("Select a from b where c = d");
	validVec.push_back("Select a from b where c = \"123\"");
	validVec.push_back("Select a from b where \"123\" = c");
	validVec.push_back("Select a from b where \"123\" = c and \"abc\" = e");
	validVec.push_back("Select a from b where \"123\" = c and e = f");
	validVec.push_back("Select a from b inner join c on a = d");
	validVec.push_back("Select a from b inner join c on a = d");
	validVec.push_back("Select a from b inner join c on e = d");
	validVec.push_back("Select a from b inner join c on e = d where \"123\" = d");
	validVec.push_back("Select a from b inner join c on e = d where f = g");
	validVec.push_back("Select a from b inner join c on e = d where f = g and f = \"abcd\"");


	// invalid
	invalidVec.push_back(" ");
	invalidVec.push_back("Select ");
	invalidVec.push_back("a");
	invalidVec.push_back("Select a");
	invalidVec.push_back("Select r.a");
	invalidVec.push_back("Select a from");
	invalidVec.push_back("Select a from r.a");
	invalidVec.push_back("Select a from b where");
	invalidVec.push_back("Select a from b where c");
	invalidVec.push_back("Select a from b where c = ");
	invalidVec.push_back("Select a from b where c = d");
	invalidVec.push_back("Select a from b where c = b.d");
	invalidVec.push_back("Select a from b where c = 123");
	invalidVec.push_back("Select a from b where c = d and");
	invalidVec.push_back("Select a from b where c = d and e");
	invalidVec.push_back("Select a from b inner");
	invalidVec.push_back("Select a from b inner join");
	invalidVec.push_back("Select a from b inner join on");
	invalidVec.push_back("Select a from b inner join c on ");
	invalidVec.push_back("Select a from b inner join c on d");
	invalidVec.push_back("Select a from b inner join c on d = ");
	invalidVec.push_back("Select a from b inner join c on d = 123");
	invalidVec.push_back("Select a from b inner join c on d = \"abc\"");
	invalidVec.push_back("Select a from b inner join c on d = b.e");

	for(int i = 0; i < validVec.size(); i++)
	{
		b = s.evaluateQuery(validVec.at(i));
		if(b == false)
		{
			break;
		}
	}

	for(int i = 0; i < invalidVec.size(); i++)
	{
		b = s.evaluateQuery(invalidVec.at(i));
		if(b == true)
		{
			break;
		}
	}

	if(!b)
	{
		int i = 0;
	}


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

