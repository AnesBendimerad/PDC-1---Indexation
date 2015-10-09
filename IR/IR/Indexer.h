#pragma once
#include <string>
#include "IIndexer.h"
using namespace std;
class Indexer : public IIndexer {
private:
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
public:
	Indexer(string repositoryPath);
	IIndex* createIndex();
	IIndexer* setIDictionary(IDictionary *iDictionary);
	IIndexer* setOutputFilePath(string outputFilePath);
	~Indexer();
};