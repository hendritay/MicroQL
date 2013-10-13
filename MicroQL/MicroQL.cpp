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

	// table ra: attribute PK:a, b, c, d
	// table rb: attribute PK:e, f, g
	// table rc: attribute PK:h

	// valid syntax & semantics
	validVec.push_back("SeLect * from ra");
	validVec.push_back("Select a from ra");
	validVec.push_back("Select a, b, c from ra");
	validVec.push_back("Select a,b,c from ra");
	validVec.push_back("Select a from ra where c = d");
	validVec.push_back("Select a from ra where c = \"123\"");
	validVec.push_back("Select a from ra where \"123\" = c");
	validVec.push_back("Select a from ra where \"123\" = c and \"abc\" = b");
	validVec.push_back("Select a from ra where \"123\" = c and b = d");
	validVec.push_back("Select a from ra where \"123\" = c and b = c");
	validVec.push_back("Select a from ra inner join rb on a = e");
	validVec.push_back("Select * from ra inner join rb on e = a");
	validVec.push_back("Select * from ra inner join rb on b = f");	
	validVec.push_back("Select a from ra inner join rb on e = d where \"123\" = d");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g");
	validVec.push_back("Select a from ra inner join rb on e = d where \"ABC\" = e");
	validVec.push_back("Select a from ra inner join rb on e = d where \"ABC\" = b");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\"");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\" and b = e");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\" and d = f");

	// invalid syntax
	invalidVec.push_back(" ");
	invalidVec.push_back("Select ");
	invalidVec.push_back("a");
	invalidVec.push_back("Select a");
	invalidVec.push_back("Select r.a");
	invalidVec.push_back("Select a from");
	invalidVec.push_back("Select a from r.a");
	invalidVec.push_back("Select *,a from ra");
	invalidVec.push_back("Select *, a from ra");
	invalidVec.push_back("Select a from ra where");
	invalidVec.push_back("Select a from ra where c");
	invalidVec.push_back("Select a from ra where c = ");
	invalidVec.push_back("Select a from ra where c = b.d");
	invalidVec.push_back("Select a from ra where c = 123");
	invalidVec.push_back("Select a from ra where c = d and");
	invalidVec.push_back("Select a from ra where c = d and b");
	invalidVec.push_back("Select a from ra inner");
	invalidVec.push_back("Select a from ra inner join");
	invalidVec.push_back("Select a from ra inner join on");
	invalidVec.push_back("Select a from ra inner join rb on ");
	invalidVec.push_back("Select a from ra inner join rb on a");
	invalidVec.push_back("Select a from ra inner join rb on a = ");
	invalidVec.push_back("Select a from ra inner join rb on a = 123");
	invalidVec.push_back("Select a from ra inner join rb on a = \"123\"");
	invalidVec.push_back("Select a from ra inner join rb on \"123\" = a");
	invalidVec.push_back("Select a from ra inner join rb on d = \"abc\"");
	invalidVec.push_back("Select a from ra inner join rb on d = b.e");

	// invalid semantics & type
	/*
	invalidVec.push_back("Select a from ra"); // a is not from table b
	invalidVec.push_back("Select d from b"); // a is not an atrribute
	invalidVec.push_back("Select a from c"); // c is not a table
	invalidVec.push_back("Select a, b, c from b"); // 1 or all attributes is not from table b
	invalidVec.push_back("Select * from ra where a = b");	// b is not an attribute
	invalidVec.push_back("Select * from ra where a = i");	// i doesn't exist
	invalidVec.push_back("Select * from ra where a = e");	// e doesn't belong to ra
	invalidVec.push_back("Select * from ra where e = a");	// e doesn't belong to ra
	invalidVec.push_back("Select * from ra inner join rd on a = b");	// rd doesn't exist
	invalidVec.push_back("Select * from ra inner join rb on h = a");	// h does not belong to rb
	invalidVec.push_back("Select * from ra inner join rb on a = b");	// both a,b from same table!
	invalidVec.push_back("Select * from ra inner join rb on a = i");	// i doesn't exist
	*/	
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

