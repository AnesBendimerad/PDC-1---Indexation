#pragma once
#include "IIndex.h"
#include "IDictionary.h"
class IIndexer {
public:
	virtual IIndex* createIndex() = 0;
	virtual IIndexer* setIDictionary(IDictionary *iDictionary) = 0;
	virtual IIndexer* setOutputFilePath(string outputFilePath) = 0;
};