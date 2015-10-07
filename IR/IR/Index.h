#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "DocumentTable.h"
class Index : public IIndex {
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	bool finilized;
	string postingFilePath;
public:
	Index(IDictionary *dictionary);
	int addDocument(DocumentMetaData documentMetaData);
	void addTerm(string token);
	void finalize();
	list<int> search(string querry);
	~Index();
};