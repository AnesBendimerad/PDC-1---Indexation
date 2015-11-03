#pragma once
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include "ICompressor.h"
#include <vector>

#define DEFAULT_OUTPUT_FILE "index"

class Index {
private:
	IDictionary *dictionary;
	DocumentTable *documentTable;
	ICompressor *compressor;
	string invertedFilePath;

	DocumentTerm* getTermPostingList(string token);

	//---- Fagin Search ------------------------------------------
	vector<vector<pair<int, double>>> calculateTF_IDF(string query);
    static int findPositionOfDocument(int documentIndex,vector<pair<int, double>> vector);
	static bool sort_by_tf_idf(const pair<int, double>& left, const pair<int, double>& right);
	
	//---- BM25 Search ------------------------------------------
	double calculateScore(int tf, int df, int nbwords, double avgdl);
	double calculateAVGDL();
	static 	bool sort_by_score(const pair<DocumentMetaData, double>& left, const pair<DocumentMetaData, double>& right);

public:
	Index(IDictionary *dictionary, DocumentTable* documentTable, ICompressor * compressor,string invertedFilePath);
	vector<pair<DocumentMetaData, double>>  searchFagin(int topK,string query);
	vector<pair<DocumentMetaData, double>>  searchBM25(int topK, string query);
	~Index();
};
