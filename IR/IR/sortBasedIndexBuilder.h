#pragma once
#include <string>
#include "IIndexBuilder.h"
#include "DocumentTable.h"
using namespace std;
class sortBasedIndexBuilder : public IIndexBuilder {
private:
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
	ICompressor * iCompressor;
	int indexType;

	// Temporary limitation of memory
	unsigned int numberOfBlock; //number of block in the memory
	unsigned int numberOfTripletInBlock; //number of triplet in each block
public:
	sortBasedIndexBuilder(string repositoryPath,unsigned int numberOfBlock, unsigned int numberOfTripletInBlock);
	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setICompressor(ICompressor *iCompressor);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	IIndexBuilder* setIndexType(int indexType);
	IIndex* createIndex();
	~sortBasedIndexBuilder();
};