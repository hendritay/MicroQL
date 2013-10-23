// MicroQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "MainType.h"
#include <stdio.h>
#include "Storage\FileManager.h"
#include "Storage\StorageManager.h"
#include "Database\TableDefinition.h";
#include "Database\InsertDefinition.h";
#include "Database\MQLCondition.h";

#include "Database\TableDictionary.h";
#include "Storage\BTree.h";
#include "Storage\BTreePage.h";
#include "Storage\BTreeKey.h";

#include "Database\MQLColumn.h";

#include <iostream>
#include <fstream>

#include "Select.h"

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Welcome to MicroQL 1.0\n";
	cout << "CS4221 Project - Implementation\n";
	cout << "Proudly done by Kim Li, Terence, Liang Rou & Hendri\n";
	cout << "=================================================================\n";



	string path = "c:\\study\\cs4221\\hellodb.txt";

	if (remove(path.c_str())) 
		cout << "deleted\n";

	
	FileManager::createAFile(path);	
	
	Parser lrParser;
	FileManager * fm = new FileManager(path);
	StorageManager *sm  = new StorageManager(fm);
	TableDictionary *tdictionary = new TableDictionary(fm, sm);



	
	BTree * bt = new BTree(fm, sm);

	string command;
	while (true) {
		cout << "microql>";
		getline(cin, command);

		if (CommonUtility::trim(command).find("CREATE TABLE") != string::npos){
			TableDefinition *td = lrParser.parseCreateCmd(command);
			if (td == NULL)  {
				cout << "Syntax error\n";
				continue;
			}
			tdictionary->storeTableToDictionary(td);
			cout << "Table created.\n";			
		}else if (CommonUtility::trim(command).find("DELETE FROM") != string::npos){
			DeleteDefinition *dd = lrParser.parseDeleteCmd(command);
			if (dd == NULL)  {
				cout << "Syntax error\n";
				continue;
			}
			delete dd;
		}else if (CommonUtility::trim(command).find("UPDATE") != string::npos){
			UpdateDefinition *ud = lrParser.parseUpdateCmd(command);
			if (ud == NULL)  {
				cout << "Syntax error\n";
				continue;
			}
			delete ud;
		}else if (CommonUtility::trim(command).find("INSERT INTO") != string::npos){
			InsertDefinition *id = lrParser.parseInsertCmd(command)   ;		
			if (id == NULL)  {
				cout << "Syntax error\n";
				continue;
			} else if (!id->verify(tdictionary)) {
				delete id;
				continue;
			}

			id->execute(tdictionary, bt);		
			cout << "Record inserted.\n";
			delete id;
		}else if (CommonUtility::trim(command).find("SELECT") != string::npos){
			// for kimli
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

			Select select = Select(tdictionary, bt);
			TableResult * tr = select.evaluateQuery(command);
			if(tr == NULL)
			{
				cout << "Syntax or Semantics error\n";
				continue;
			}
			else
			{
				cout << "complete\n";
				continue;
				//tr.print();

			}
			/*
			TableResult tr(tdictionary, bt);

			// CONSTRUCTING 
			// SELECT employeeid, name FROM employeeinformation WHERE age = "10";
			string colName ="employeeid";
			MQLColumn column1(colName, CT_VARCHAR);
			colName = "name";
			MQLColumn column2(colName, CT_VARCHAR);

			vector<MQLColumn> colList; 
			colList.push_back(column1);
			colList.push_back(column2);

			string age = "age";
			string cond = "10";
			MQLColumn columnAge(age, CT_VARCHAR);
			MQLColumn columnCond(cond, CT_CONSTANT_STRING);

			string oper="=";
			MQLCondition cond1(columnAge,oper, columnCond);
			vector<MQLCondition> conditionList; 
			conditionList.push_back(cond1);
			tr.loadResult("employeeinformation", colList, conditionList);

			// CONSTRUCTING
			// SELECT * FROM employeecar;
			string colNamecar ="employeeid";
			MQLColumn columncar(colNamecar , CT_VARCHAR);

			string colNamecarplate ="carplate";
			MQLColumn columncarplate(colNamecarplate, CT_VARCHAR);

			vector<MQLColumn> colcarLIst;
			colcarLIst.push_back(columncar);
			colcarLIst.push_back(columncarplate);

			vector<MQLCondition> condCarList;
			TableResult trcar(tdictionary, bt);
			trcar.loadResult("employeecar", colcarLIst, condCarList);


			// merging

			TableResult trNew = TableResult::merge(&tr, &trcar, tdictionary, bt);

			for (int i = 0; i < tr.getNoOfRows(); i++) {
				for (int j = 0; j < trcar.getNoOfRows(); j++) {
					trNew.addMergedRow(&tr, i, &trcar, j);
				}
			}
			*/

			// for kimli 
		}else if (CommonUtility::trim(command).find("quit") != string::npos){
			break;
		}
	}



	//TableDefinition *td = lrParser.parseCreateCmd("CREATE TABLE employeeinformation(employeeid varchar(10), name varchar(30), age varchar(10), PRIMARY KEY(employeeid));");
	return 0;
}

