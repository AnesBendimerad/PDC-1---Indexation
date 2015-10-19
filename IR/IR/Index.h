#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include <vector>
class Index : public IIndex {
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	string invertedFilePath;

	DocumentTerm* getTermPostingList(string token);
	vector<vector<pair<int, double>>> calculateTF_IDF(string query);
	int findPositionOfDocument(int documentIndex,vector<pair<int, double>> vector);
public:
	Index(IDictionary *dictionary, DocumentTable* documentTable, string invertedFilePath);
	list<int> search(int topK,string query);
	~Index();
};

bool sort_by_tf_idf(const pair<int, double>& left, const pair<int, double>& right);