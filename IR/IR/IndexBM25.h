#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include "DocumentMetaData.h"
#include "ICompressor.h"
#include <vector>
class IndexBM25 : public IIndex
{
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	ICompressor *compressor;
	string invertedFilePath;

	DocumentTerm* getTermPostingList(string token);
	double calculateScore(int tf,int df,int nbwords,int avgdl);
    double calculateAVGDL();
	static 	bool sort_by_score(const pair<DocumentMetaData, double>& left, const pair<DocumentMetaData, double>& right);

public:
	IndexBM25(IDictionary *dictionary, DocumentTable* documentTable, ICompressor * compressor, string invertedFilePath);
	vector<pair<DocumentMetaData, double>>  search(int topK, string query);
	~IndexBM25();
};

