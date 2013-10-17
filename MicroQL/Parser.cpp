#include "stdafx.h"
#include "Parser.h"

Parser::Parser(void)
{ }

Parser::~Parser(void)
{}
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

std::vector<std::string> &split(const std::string &inputString, char delimiter, std::vector<std::string> &tokens) {
    std::stringstream strStream(inputString);
    std::string item;
    while (std::getline(strStream, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

std::vector<std::string> split(const std::string &inputString, char delimiter) {
    std::vector<std::string> tokens;
    split(inputString, delimiter, tokens);
    return tokens;
}

bool replaceSubstring(string& inputString, const string& replaceTarget, const string& replaceValue){
	int loc = -1;
	int size = replaceTarget.size();

	while((loc = inputString.find(replaceTarget, loc+1)) != std::string::npos){
		inputString.replace(loc, size, replaceValue); //Single space in quotes
		loc = loc - size;
	}

	return true;
}
bool preprocessCommand(string command){

	replaceSubstring(command, "  ", " ");
	replaceSubstring(command, "( ", "(");
	replaceSubstring(command, " (", "(");
	replaceSubstring(command, ") ", ")");
	replaceSubstring(command, " )", ")");
	replaceSubstring(command, " ;", ";");
	replaceSubstring(command, "  =", "=");
	replaceSubstring(command, "= ", "=");
	replaceSubstring(command, "  '", "'");
	//replaceSubstring(command, "' ", "'");
	trim(command);
	
	//cout << "last char: " << command.substr(command.length() -1, 1) << endl;

	if (command.substr(command.length() -1, 1) != ";")
		return false;
}
bool parse(string command){

	replaceSubstring(command, "  ", " ");
	replaceSubstring(command, "( ", "(");
	replaceSubstring(command, " (", "(");
	replaceSubstring(command, ") ", ")");
	replaceSubstring(command, " )", ")");
	replaceSubstring(command, " ;", ";");
	replaceSubstring(command, "  =", "=");
	replaceSubstring(command, "= ", "=");
	replaceSubstring(command, "  '", "'");
	//replaceSubstring(command, "' ", "'");
	trim(command);
	
	//cout << "last char: " << command.substr(command.length() -1, 1) << endl;

	if (command.substr(command.length() -1, 1) != ";")
		return false;

	if (trim(command).find("CREATE") != string::npos){
		return parseCreateCmd(command);
	}else if (trim(command).find("DELETE FROM") != string::npos){
		return parseDeleteCmd(command);
	}else if (trim(command).find("UPDATE") != string::npos){
		return parseUpdateCmd(command);
	}else if (trim(command).find("INSERT INTO") != string::npos){
		return parseInsertCmd(command);
	}else
		return false;

	return true;
}

string getTableName(string tableInfo, string command){

	string commandToFind = command;
	
	if(tableInfo.find(commandToFind)!= -1){
		string tableName = tableInfo.erase(0, commandToFind.length());
		//cout<< tableName << endl;
		return tableName;
	}else
		return "NULL";
}

string getPriKey(string priKeyInfo){
	int removeDelimiters = 4; //());
	int removeOpenBracket = 1; //(
	string priKey = priKeyInfo.substr(removeOpenBracket,priKeyInfo.length()-removeDelimiters);
	return priKey;
	//cout<< priKey <<endl;
}

bool getAndAddCol(string colInfo, string priKey, TableDefinition &td){

	vector<string> columnData = split(colInfo, ',');
	vector<string> columns;
	vector<string> colType;
	pair<string, pair<string, string>> colPair;
	vector<pair <string, pair<string, string>>> individualCol;
	MQLColumn mqlCol;
	
	for (vector<string>::size_type colCount = 0; colCount < columnData.size(); colCount++){
		columns = split(trim(columnData[colCount]), ' ');

		if (columns.size() != 2){
			return false;
		}

		if (!(columns[1].find("(") != string::npos && columns[1].find(")") != string::npos && (columns[1].find("(") < columns[1].find(")")))){
			return false;
		}

		colType = split(columns[1],'(');
		colType[1] = colType[1].substr(0, colType[1].length()-1);
		//cout<< colType[0] << endl;
		//cout<< colType[1] << endl;
		colPair = std::make_pair(columns[0], make_pair(colType[0],colType[1]));
		individualCol.push_back(colPair);
		//cout << colPair.first << ": " << colPair.second.first <<": "<<colPair.second.second << endl;
	}

	for(vector<string>::size_type counter = 0; counter < individualCol.size(); counter++){
		if (individualCol[counter].first== priKey){
			mqlCol = MQLColumn(individualCol[counter].first, CT_VARCHAR, atoi(individualCol[counter].second.second.c_str()), true);}
		else{
			mqlCol = MQLColumn(individualCol[counter].first, CT_VARCHAR, atoi(individualCol[counter].second.second.c_str()), false);}

		td.addColumn(mqlCol);

	
	}
}

TableDefinition* parseCreateCmd(string command){
	vector <string> createTableInformation;
	string tableInformation, columnInformation, priKeyInformation, tableName;
	string findPriKey = "PRIMARY KEY";
	bool success = preprocessCommand(command);
	if(!success){
		return NULL;
	}
	int colNamePos = command.find("(");
	
	tableInformation = command.substr(0, colNamePos);
	tableName = getTableName(tableInformation, "CREATE TABLE ");
	if (tableName == "NULL")
		return NULL;
	
	TableDefinition *td = new TableDefinition(tableName);

	int priKeyPos = command.find(findPriKey);
	//cout << priKeyPos << endl;
	if (priKeyPos!=std::string::npos){
	  priKeyInformation = command.substr(priKeyPos+findPriKey.length(), command.length()-priKeyPos);}
	else{
	  return NULL;}

	//cout<< "PriKeyInformation: " << priKeyInformation <<endl;
	
	//cout<<priKeyInformation <<endl;
	if (!(priKeyInformation.find("(") != string::npos && priKeyInformation.find(")") != string::npos && (priKeyInformation.find("(") < priKeyInformation.find(")"))&&(priKeyInformation.find_last_of(")")!= priKeyInformation.find(")")))){
			return NULL;
		}
	string priKey = getPriKey(priKeyInformation);
	columnInformation = trim(command.substr(colNamePos+1, priKeyPos - colNamePos-2));
	
	//cout<< tableInformation << endl;
	//cout << columnInformation << endl;
	//cout<<priKeyInformation << endl;
	bool getColSuccess = getAndAddCol(columnInformation, priKey, *td);
	if(!getColSuccess){
		return NULL;}
	else{
		return td;
	}
}

DeleteDefinition* parseDeleteCmd(string command){
	string tableInformation = command.substr(0, command.find("WHERE "));
	string tableName = getTableName(tableInformation, "DELETE FROM ");
	vector<pair<string, string>> conditionClauses;
	vector<string> splitResults;

	bool success = preprocessCommand(command);
	if(!success){
		return NULL;
	}

	DeleteDefinition *delDef = new DeleteDefiniton();
	delDef->setName(tableName);

	//cout << "table name: " << tableName << endl;

	string conditions = command.substr(command.find("WHERE ") + 6, command.length() - command.find("WHERE ") + 7);
	//cout << "condition: " << conditions << endl;

	replaceSubstring(conditions, ";", "");
	replaceSubstring(conditions, " AND ", "|");
	vector<string> tokens = split(conditions, '|');

	for (vector<string>::size_type counter = 0; counter < tokens.size(); counter++){
		//cout << "condition clauses: " << tokens[counter] << endl;
		splitResults = split(tokens[counter], '=');
		if (splitResults.size() != 2){
			//cout << "incorrect no. of arguments" << endl;
			return NULL;
		}
		if (!(splitResults[1].find("'") != string::npos && (splitResults[1].find_last_of("'") != splitResults[1].find("'") ))){
			return NULL;
		}
		replaceSubstring(splitResults[1], "'", " ");
		splitResults[1] = trim(splitResults[1]);
		conditionClauses.push_back(make_pair(splitResults[0], splitResults[1]));
	}

	for (vector<pair<string, string>>::size_type counter = 0; counter < conditionClauses.size(); counter++){
		MQLColumn col1;
		MQLColumn col2;
		col1 = MQLColumn(conditionClauses[counter].first, CT_VARCHAR);
		col2 = MQLColumn(conditionClauses[counter].second, CT_CONSTANT_STRING);
		string oper ="=";
		MQLCondition cond(col1, oper, col2);
		delDef->addWhere(cond);
		//cout << "condition " << counter << ": " << conditionClauses[counter].first << " VS " << conditionClauses[counter].second << endl;
	}

	return delDef;
}

UpdateDefinition* parseUpdateCmd(string command){
	vector<pair<string, string>> updateInfoClauses, conditionClauses;
	vector<string> splitResults, splitUpdateInfo, updateInfoDetails, splitConditions, conditionDetails;

	string tableInformation = command.substr(0, command.find("SET "));
	string tableName = getTableName(tableInformation, "UPDATE ");

	MQLColumn col1;
	MQLColumn col2;

	UpdateDefinition *upDef = new UpdateDefinition();
	upDef->setName(tableName);
	
	if (tableName == "NULL")
	return NULL;

	replaceSubstring(command, " SET ", "|");
	replaceSubstring(command, " WHERE ", "|");
	replaceSubstring(command, ", ", ",");
	replaceSubstring(command, " ,", ",");
	replaceSubstring(command, ";", "");

	splitResults = split(command, '|');

	if (splitResults.size() < 2)
		return NULL;
	else if (splitResults.size() == 2 || splitResults.size() == 3){ 
		//cout << "processing conditions and update information..." << endl;

		string updateInformation = splitResults[1];
		splitUpdateInfo = split(updateInformation, ',');

		//cout << splitUpdateInfo.size() << endl;

		for (vector<string>::size_type counter = 0; counter < splitUpdateInfo.size(); counter++){
			//cout << "splitting update information..." << endl;
			updateInfoDetails = split(splitUpdateInfo[counter], '=');
			if (updateInfoDetails.size() != 2)
				return NULL;
			updateInfoClauses.push_back(make_pair(updateInfoDetails[0], updateInfoDetails[1]));
		}

		for (vector<pair<string, string>>::size_type counter = 0; counter < updateInfoClauses.size(); counter++){

		col1 = MQLColumn(updateInfoClauses[counter].first, CT_VARCHAR);
		col2 = MQLColumn(updateInfoClauses[counter].second, CT_VARCHAR);
		// ask mr hendri to resolve
		//string oper = "=";
		//upDef->addSet(col1, oper, col2);

		//cout << "update info clauses: " << updateInfoClauses[counter].first << " VS " << updateInfoClauses[counter].second << endl;
		}

		if (splitResults.size() == 3){
			string conditions = splitResults[2];
			splitConditions = split(conditions, ',');
			for (vector<string>::size_type counter = 0; counter < splitConditions.size(); counter++){
				conditionDetails = split(splitUpdateInfo[counter], '=');
				if (conditionDetails.size() != 2)
					return NULL;
				conditionClauses.push_back(make_pair(conditionDetails[0], conditionDetails[1]));
			}

			for (vector<pair<string, string>>::size_type counter = 0; counter < conditionClauses.size(); counter++){
				col1 = MQLColumn(conditionClauses[counter].first, CT_VARCHAR);
				col2 = MQLColumn(conditionClauses[counter].second, CT_CONSTANT_STRING);
				string oper ="="; 
				MQLCondition cond(col1, oper, col2);
				upDef->addWhere(cond);
				//cout << "update condition clauses: " << conditionClauses[counter].first << " VS " << conditionClauses[counter].second << endl;
			}
		}

		return upDef;
	}else
		return NULL;
}

InsertDefinition* parseInsertCmd(string command){
	vector<pair<string, string>> insertClauses;

	bool success = preprocessCommand(command);
	if(!success){
		return NULL;
	}
	string tableInformation = command.substr(0, command.find("("));
	string tableName = getTableName(tableInformation, "INSERT INTO");
	InsertDefinition *insDef = new InsertDefinition();
	insDef->setName(tableName);

	replaceSubstring(command, "( ", "(");
	replaceSubstring(command, ", ", ",");
	replaceSubstring(command, " ,", ",");

	string colsvals = command.substr(command.find("(") + 1, command.length() - command.find("(") + 2);
	//cout << colsvals << endl;
	replaceSubstring(colsvals, "VALUES", "|");
	replaceSubstring(colsvals, "(", "");
	replaceSubstring(colsvals, ")", "");

	vector<string> seperateInformation = split(colsvals, '|');

	if (seperateInformation.size() != 2)
		return NULL;

	string cols = seperateInformation[0];
	string values = seperateInformation[1];

	vector<string> colsDetails = split(cols, ',');
	vector<string> valuesDetails = split(values, ',');

	if (colsDetails.size() != valuesDetails.size()) //number of cols to insert date into doesn't tally with number of values provided
		return NULL;

	for (vector<string>::size_type counter = 0; counter < colsDetails.size(); counter++){
		if (!(valuesDetails[counter].find("'") != string::npos && (valuesDetails[counter].find_last_of("'") != valuesDetails[counter].find("'") ))){
			return NULL;
		}
		replaceSubstring(valuesDetails[counter], "'", "");
		insertClauses.push_back(make_pair(colsDetails[counter], valuesDetails[counter]));
	}

	for (vector<pair<string, string>>::size_type counter = 0; counter < insertClauses.size(); counter++){
		
		MQLColumn col1(insertClauses[counter].first, VT_VARCHAR);
		MQLColumn col2(insertClauses[counter].second, CT_CONSTANT_STRING);
		string oper = "=";
		MQLCondition set(col1, oper, col2);
		insDef->addInsert(set);
		//cout << "insert clauses: " << insertClauses[counter].first << " VS " << insertClauses[counter].second << endl;
	}

	return insDef;
}