#pragma once
#include "Index.h"
#include "IDictionary.h"
#include "ICompressor.h"
class IIndexBuilder {
public:
	virtual Index* createIndex() = 0;
	virtual IIndexBuilder* setIDictionary(IDictionary *iDictionary) = 0;
	virtual IIndexBuilder* setICompressor(ICompressor *iCompressor) = 0;
	virtual IIndexBuilder* setITokenizer(int iTokenizerType) = 0;
	virtual IIndexBuilder* setOutputFilePath(string outputFilePath) = 0;
};