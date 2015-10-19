#pragma once
#include <string>
#include "IIndexBuilder.h"
#include "DocumentTable.h"
using namespace std;
class InMemoryIndexBuilder : public IIndexBuilder {
private:
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
	int indexType;

	void addTerm(string token, DocumentTable *documentTable);
	void finalize(DocumentTable *documentTable);
public:
	InMemoryIndexBuilder(string repositoryPath);
	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	IIndexBuilder* setIndexType(int indexType);
	IIndex* createIndex();
	~InMemoryIndexBuilder();
};