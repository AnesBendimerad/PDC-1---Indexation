#pragma once
#include <string>
#include "IIndexBuilder.h"
using namespace std;
class InMemoryIndexBuilder : public IIndexBuilder {
private:
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
public:
	InMemoryIndexBuilder(string repositoryPath);
	IIndex* createIndex();
	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	~InMemoryIndexBuilder();
};