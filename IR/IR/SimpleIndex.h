#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
class SimpleIndex : public IIndex {
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	string postingFilePath;
public:
	SimpleIndex(IDictionary *dictionary, DocumentTable* documentTable, string postingFilePath);
	list<int> search(string querry);
	DocumentTerm* getTermPostingList(string token);
	~SimpleIndex();
};