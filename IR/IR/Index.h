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
    static int findPositionOfDocument(int documentIndex,vector<pair<int, double>> vector);
	static bool sort_by_tf_idf(const pair<int, double>& left, const pair<int, double>& right);
public:
	Index(IDictionary *dictionary, DocumentTable* documentTable, string invertedFilePath);
	vector<pair<DocumentMetaData, double>>  search(int topK,string query);
	~Index();
};
