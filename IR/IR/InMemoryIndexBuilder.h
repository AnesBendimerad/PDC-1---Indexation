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
	ICompressor * iCompressor;
	int iTokenizerType;

	void addTerm(string token, DocumentTable *documentTable);
	void finalize(DocumentTable *documentTable);
public:
	InMemoryIndexBuilder(string repositoryPath);
	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setICompressor(ICompressor *iCompressor);
	IIndexBuilder* setITokenizer(int iTokenizerType);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	Index* createIndex();
	~InMemoryIndexBuilder();
};