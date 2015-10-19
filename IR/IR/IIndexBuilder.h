#pragma once
#include "IIndex.h"
#include "IDictionary.h"
class IIndexBuilder {
public:
	virtual IIndex* createIndex() = 0;
	virtual IIndexBuilder* setIDictionary(IDictionary *iDictionary) = 0;
	virtual IIndexBuilder* setOutputFilePath(string outputFilePath) = 0;
	virtual IIndexBuilder* setIndexType(int indexType) = 0;
};