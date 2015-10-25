#pragma once
#include <string>
#include "IIndexBuilder.h"
#include "DocumentTable.h"
#include "Document.h"
#include "Triplet.h"

using namespace std;

#define MEMORY_RATIO_USED_FOR_BUFFER 0.5
#define DEFAULT_MEMORY_LIMIT_IN_BYTE 65536

class sortBasedIndexBuilder : public IIndexBuilder {
private:
	string temporaryFilePrefixPath;
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
	ICompressor * iCompressor;
	int indexType;
	
	// Temporary limitation of memory
	unsigned int numberOfBlock; //number of block in the memory
	unsigned int numberOfTripletInBlock; //number of triplet in each block

	// private methods
	list<Triplet>* parseDocumentToTriplet(Document* document, DocumentTable* documentTable);
	unsigned int createFirstLevelSortedTripletsFiles(Triplet *tripletBuffer, DocumentTable* documentTable);
	string getFinalSortedTripletsFilesByFusion(Triplet *tripletBuffer, DocumentTable* documentTable, unsigned int lastRunNumber);
	void finalize(DocumentTable *documentTable, string sortedTripletFilePath);
	static int sort_by_termId_docId(const void *left, const void *right);

public:
	sortBasedIndexBuilder(string repositoryPath,unsigned int numberOfBlock, unsigned int numberOfTripletInBlock);
	sortBasedIndexBuilder(string repositoryPath, unsigned int memoryLimitInByte);
	sortBasedIndexBuilder(string repositoryPath); // use the defined DEFAULT_MEMORY_LIMIT_IN_BYTE

	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setICompressor(ICompressor *iCompressor);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	IIndexBuilder* setIndexType(int indexType);
	IIndex* createIndex();
	~sortBasedIndexBuilder();
};