#include "stdafx.h"
#include "CommonUtility.h"
#include "TableDictionary.h"
#include "TableDefinition.h"


void TableDictionary::storeTableToDictionary(TableDefinition *td) {
	string page = myFile->readPage(StorageManager::DictionaryPage);

	int columnPage = myStorage->GiveMeFreePageNo();

	td->setRecordPage(myStorage->GiveMeFreePageNo());

	int currPosition = 0;
	int NoOfBytes = 2;
	
	int NoOfTables = 0;
	if (page.compare("") != 0)
		NoOfTables = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
	
	currPosition += NoOfBytes;
	// find a nice position to slot in. 
	for (int i = 0; i < NoOfTables; i++) {
		currPosition += CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes; // itself;
		currPosition += NoOfBytes; // columnPage;
	}

	string tableName = td->getTableName();
	string payload = CommonUtility::convertShortTo2Bytes(tableName.size());
	payload.append(tableName);
	payload.append(CommonUtility::convertShortTo2Bytes(columnPage));

	myFile->writeAt(payload, StorageManager::DictionaryPage, currPosition);
	myFile->writeAt(td->serialize(), columnPage, 0);
	// update internal table list
	
	
	if (populatedTableList)  {
		dicitionaryList[td->getTableName()] = pair<int, TableDefinition*>(columnPage, td);
		addColumnToColumnList(td); // nice
	}else {
		TableDefinition *tdempty = NULL;
		dicitionaryList[td->getTableName()] = pair<int, TableDefinition*>(columnPage, tdempty);
	}

	// update the tablenumber
	myFile->writeAt(CommonUtility::convertShortTo2Bytes(NoOfTables + 1), StorageManager::DictionaryPage, 0);

}


// First 2 bytes stores the number of tables
// 011
// SubSequent 2 bytes is how long is the tablename, 
// After that the next 2 chars store the pageno; 
/*  ----------------------------
Sample:
7
0x08employee0x12
0x04sale0x18                            

0x12 == represent 2 bytes
*/
// currently only page 2 stores the table, that will limit tables to a few hundred.

void TableDictionary::populateTableNameList() {
	string page = myFile->readPage(StorageManager::DictionaryPage);

	if (page.compare("") == 0) {
		return;
	}

	int currPosition = 0;
	int NoOfBytes = 2;
	int tableLength ;
	int NoOfTables = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
	currPosition += NoOfBytes;
	string tableName;
	int pageno;
		 TableDefinition *emptytd = NULL;
		 
	for (int i = 0; i < NoOfTables; i++) {
		// read number of chars for the table;
		 tableLength = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		 currPosition += NoOfBytes;
		 tableName = page.substr(currPosition, tableLength);
		 currPosition += tableLength;

		 pageno = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		 currPosition += NoOfBytes;
		 
		 pair<int, TableDefinition*> p(pageno, emptytd);

		 dicitionaryList[tableName] = p;
	}
}


bool TableDictionary::tableExists(string &tableName) {
	DictionaryList::iterator iter;
	iter = dicitionaryList.find(tableName);
	return iter  != dicitionaryList.end();
}

void TableDictionary::populateTableList() {
	if (!populatedTableList) {
		populatedTableList = true;
		DictionaryList::iterator iter;
		for (iter = dicitionaryList.begin(); iter != dicitionaryList.end(); iter++)  {
			string pageContent = myFile->readPage(iter->second.first);

			TableDefinition *td = TableDefinition::deSerialize(pageContent);
			iter->second.second = td;
			addColumnToColumnList(td);
		}
	}
}

void TableDictionary::addColumnToColumnList(TableDefinition *td) {
	int noOfColumn = td->getNoOfColumn();

	for (int i = 0; i < noOfColumn; i++) {
		MQLColumn &column = td->getColumnAt(i);
		columnNameList[column.getColumnName()] = true;
	}
}

bool TableDictionary::columnExists(string columnName) {
	if (!populatedTableList) 
		populateTableList();

	map<string, bool>::iterator iter = columnNameList.find(columnName);
	return iter != columnNameList.end();
}