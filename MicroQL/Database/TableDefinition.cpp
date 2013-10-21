#include "stdafx.h"
#include "TableDefinition.h"
#include "..\Storage\PageManager.h"
#include "CommonUtility.h"
#include "MQLColumn.h"
/* Format of table definition stored in file is similiar to dictionary file

[NumberOfColumn]
[NumberOfChars]+[ColumnName]+[ColumnSize]+[ColumnType]

NumberOfChars = 2 bytes
NumberOfColumn = 2 Bytes;
ColumnType = 2 bytes
*/
ListChar TableDefinition::serialize() {
	ListChar resultList;

	// No Of Column;
	resultList.push_back(PageManager::TABLEPAGE);
	CommonUtility::appendIntToList (resultList, columnList.size());
	CommonUtility::appendIntToList (resultList, startRecordPageNo);	
	ColumnList::iterator iter;
	
	
	for (iter = columnList.begin(); iter != columnList.end(); iter++) {		
		CommonUtility::appendList(resultList, iter->serialize());
	}
	return resultList;
}

TableDefinition * TableDefinition::deSerialize(string &page) {
	TableDefinition *td = new TableDefinition();
	
	if (page[0] != PageManager::TABLEPAGE) {
		return NULL;
	}

	int currPosition = 1;
	int NoOfBytes = 2;
	int columnLength ;

	
	int NoOfColumn = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
	currPosition += NoOfBytes;

	int startRecordPageNo = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
	td->setRecordPage(startRecordPageNo);

	currPosition += NoOfBytes;

	string columnName;
	int columnSize;
	int columnType;
	bool isPrimaryKey;
	for (int i = 0; i < NoOfColumn ; i++) {
		// read number of chars for the table;
		 columnLength= CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		 currPosition += NoOfBytes;
		 columnName= page.substr(currPosition, columnLength);
		 currPosition += columnLength;
		 		 
		 columnType = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		 currPosition += NoOfBytes;

		 columnSize= CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes)); // columnsize;
		 currPosition += NoOfBytes;// columnsize;

		 isPrimaryKey =  (page[currPosition] == '1');
		 currPosition++;

		 MQLColumn column(columnName, CT_VARCHAR, columnSize, isPrimaryKey); // always VARCHAR now
		 td->addColumn(column);
	}

	return td;
}