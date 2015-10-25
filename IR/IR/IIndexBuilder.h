#pragma once
#include "IIndex.h"
#include "IDictionary.h"
#include "ICompressor.h"
class IIndexBuilder {
public:
	virtual IIndex* createIndex() = 0;
	virtual IIndexBuilder* setIDictionary(IDictionary *iDictionary) = 0;
	virtual IIndexBuilder* setICompressor(ICompressor *iCompressor) = 0;
	virtual IIndexBuilder* setITokenizer(int iTokenizerType) = 0;
	virtual IIndexBuilder* setOutputFilePath(string outputFilePath) = 0;
	virtual IIndexBuilder* setIndexType(int indexType) = 0;
};