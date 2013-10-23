// MicroQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"
#include "MainType.h"
#include "Select.h"

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
	string logPath = "c:\\study\\cs4221\\hellolog.txt";

	//if (remove(path.c_str()) == 0) 
	//	cout << "default empty HelloDB.txt loaded" << endl;
	
	//FileManager::createAFile(path);	
	
	Parser lrParser;
	FileManager * fm = new FileManager(path);
	StorageManager *sm  = new StorageManager(fm);
	TableDictionary *tdictionary = new TableDictionary(fm, sm);

	BTree * bt = new BTree(fm, sm);

	string command;
	while (true) {
		cout << "\nmicroql>";
		getline(cin, command);
		FileManager::writeLog(logPath, command);

		if (CommonUtility::trim(command).find("CREATE TABLE") != string::npos){
			TableDefinition *td = lrParser.parseCreateCmd(command);
			if (td == NULL)  {
				cout << "Syntax error\n";
				continue;
			} else if (!tdictionary->verifyCandidate(td)) {
				delete td;
				continue;
			}
			tdictionary->storeTableToDictionary(td);
			cout << "Table created.\n";			
		}else if (CommonUtility::trim(command).find("DELETE FROM") != string::npos){
			DeleteDefinition *dd = lrParser.parseDeleteCmd(command);
			if (dd == NULL)  {
				cout << "Syntax error\n";
				continue;
			} else if (!dd->verify(tdictionary, bt)) {
				delete dd;
				continue;
			}

			dd->execute(tdictionary, bt, fm);
			delete dd;
		}else if (CommonUtility::trim(command).find("UPDATE") != string::npos){
			UpdateDefinition *ud = lrParser.parseUpdateCmd(command);
			if (ud == NULL)  {
				cout << "Syntax error\n";
				continue;
			} else if (!ud->verify(tdictionary)) {
				delete ud;
				continue;
			}

			ud->execute(tdictionary, bt, fm, sm);
			delete ud;
		}else if (CommonUtility::trim(command).find("INSERT INTO") != string::npos){
			InsertDefinition *id = lrParser.parseInsertCmd(command)   ;		
			if (id == NULL)  {
				cout << "Syntax error\n";
				continue;
			} else if (!id->verify(tdictionary, bt)) {
				delete id;
				continue;
			}

			id->execute(tdictionary, bt);		
			cout << "Record inserted.\n";
			delete id;
		}else if (CommonUtility::trim(command).find("SELECT") != string::npos){
			Select select = Select(tdictionary, bt);
			TableResult * tr = select.evaluateQuery(command);
			tr->print();
			continue;

			// for kimli
			// insertion
			//command = "INSERT INTO EmployeeInformation (employeeid, name, age) VALUES ('1', 'KimmY', '20')";


			// ****attribute name, table name are case sensitive,must have a space between each word
			// valid 
			// SF
			//command = "SELECT * FROM EmployeeInformation";
			//command = "SELECT * from EmployeeInformation";
			//command = "SELECT employeeid, age FROM EmployeeInformation";
			//command = "SELECT age, employeeid FROM EmployeeInformation";
			//command = "SELECT age,employeeid FROM EmployeeInformation";

			// SFW
			//command = "SELECT * FROM EmployeeInformation WHERE employeeid = '1'";
			//command = "SELECT age, employeeid FROM EmployeeInformation WHERE employeeid = '10'";
			//command = "SELECT age, employeeid FROM EmployeeInformation WHERE age = '10'";
			//command = "SELECT age, employeeid FROM EmployeeInformation WHERE age = '12345678910'";
			//command = "SELECT employeeid FROM EmployeeInformation WHERE employeeid = age";	// may have result
			//command = "SELECT age, employeeid FROM EmployeeInformation WHERE age = '10' AND employeeid = '1'";


			// SFJ
			//command = "SELECT * FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid";
			//command = "SELECT employeeid, salepersonid FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid";
			//command = "SELECT employeeid, qty FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid";
			//command = "SELECT employeeid FROM EmployeeInformation INNER JOIN Sale ON saleid = employeeid";
			//command = "SELECT employeeid,qty , FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid INNER JOIN ON ";
			//command = "SELECT * FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid INNER JOIN ON ";


			// SFJW
			//command = "SELECT * FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid WHERE employeeid = '1'";
			//command = "SELECT salepersonid, name, qty FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid WHERE employeeid = '1' AND salepersonid = '1' ";
			//command = "SELECT * FROM EmployeeInformation INNER JOIN Sale ON salepersonid = employeeid WHERE employeeid = '1' AND salepersonid = '1' and age = '10'";

			// remove redundancy
			//command = "SELECT employeeid FROM EmployeeInformation INNER JOIN EmployeeInformation ON employeeid = employeeid";	// just get from a single table
			//command = "SELECT * FROM Sale INNER JOIN EmployeeInformation ON salepersonid = employeeid AND employeeid = salepersonid AND employeeid = '1'";

			// invalid syntax, type
			//command = "SELECT EmployeeId FROM EmployeeInformation"; // attribute does not exist
			//command = "SELECT employeeid FROM EmployeeInformation WHERE abc = def";	// attributes does not exist
			//command = "SELECT employeeid FROM Employee"; // table does not exist

			// invalid semantics
			//command = "SELECT qty FROM EmployeeInformation";	// qty does not belong to table
			//command = "SELECT employeeid FROM EmployeeInformation WHERE qty = '50'"; // qty does not belong to table
			//command = "SELECT employeeid FROM EmployeeInformation WHERE qty = age"; // qty does not belong to table
		
			//command = "SELECT employeeid FROM EmployeeInformation INNER JOIN Sale ON qty = employeeid";	// both from same table, diff datatype
			//command = "SELECT employeeid FROM EmployeeInformation INNER JOIN Sale ON name = employeeid";	// both from same table, same datatype
			
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

			tr.print();

			//// CONSTRUCTING
			//// SELECT * FROM employeecar;
			//string colNamecar ="employeeid";
			//MQLColumn columncar(colNamecar , CT_VARCHAR);

			//string colNamecarplate ="carplate";
			//MQLColumn columncarplate(colNamecarplate, CT_VARCHAR);

			//vector<MQLColumn> colcarLIst;
			//colcarLIst.push_back(columncar);
			//colcarLIst.push_back(columncarplate);

			//vector<MQLCondition> condCarList;
			//TableResult trcar(tdictionary, bt);
			//trcar.loadResult("employeecar", colcarLIst, condCarList);


			// merging
			*/
		
			// for kimli 
		}else if (CommonUtility::trim(command).find("STAR") != string::npos) {		
			string newCommand= CommonUtility::trim(command);
			size_t f = newCommand.find(";");
			string tableName;
			tableName = CommonUtility::trim(newCommand.substr(4));

			if (f != string::npos) 
				tableName = CommonUtility::trim(tableName.substr(0, tableName.size() -1));

			TableResult tr(tdictionary, bt);
			tr.loadResultStar (tableName);
			tr.print();

		}else if (CommonUtility::trim(command).find("SHOW INFO") != string::npos) {		
			


			string newCommand= CommonUtility::trim(command);
			size_t f = newCommand.find(";");
			string tableName;
			tableName = CommonUtility::trim(newCommand.substr(10));

			if (f != string::npos) 
				tableName = CommonUtility::trim(tableName.substr(0, tableName.size() -1));

			
			//tableName.replace(tableName.begin(), tableName.end(), ';', '');
			if (!tdictionary->tableExists(tableName)) {
				cout << "Table not found.\n";
				continue;
			} else {
				TableDefinition tdef = tdictionary->getTableDefinition(tableName);
				tdef.print();
			}
			
		} else if (CommonUtility::trim(command).compare("SHOW TABLES;") == 0) {
			tdictionary->print();
		} else if (CommonUtility::trim(command).find("quit") != string::npos){
			break;
		} else if (CommonUtility::trim(command).find("LOAD DATABASE") != string::npos){
			string newCommand= CommonUtility::trim(command);
			size_t f = newCommand.find(";");
			string pathName;
			pathName= CommonUtility::trim(newCommand.substr(14));

			if (f != string::npos) 
				pathName= CommonUtility::trim(pathName.substr(0, pathName.size() -1));


			if (FileManager::fileExists(pathName)) {
				try {
					FileManager fm1(pathName);					
					TableDictionary dict(&fm1, sm);
					delete fm;
					delete sm;					
					delete tdictionary;					
					delete bt;

					fm = new FileManager(pathName);
					sm  = new StorageManager(fm);
					tdictionary = new TableDictionary(fm, sm);

					bt = new BTree(fm, sm);

					cout << "Database ";
					cout << pathName;
					cout << " loaded";
					cout << endl;
				} catch (exception e) {
					cout << "Database not loaded.";

				}

			} else {
				
				if (FileManager::createAFile(pathName)) {
					delete fm;
					delete tdictionary;
					delete bt;
					delete sm;
					fm = new FileManager(pathName);
					sm = new StorageManager(fm);
					tdictionary = new TableDictionary(fm, sm);
					
					bt = new  BTree(fm,sm);
					cout << "Database created." << endl;
				} else {
					cout << "Database not loaded.";
				}
			}
		}
	}

	return 0;
}

