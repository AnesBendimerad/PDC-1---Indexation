#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "DocumentTable.h"
class Index : public IIndex {
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	bool finalized;
	string postingFilePath;
	DocumentTerm * getTermPostingList(string token);
public:
	Index(IDictionary *dictionary,string outputFilePath);
	int addDocument(DocumentMetaData documentMetaData);
	void addTerm(string token);
	void finalize();
	list<int> search(string querry);
	~Index();
};