#pragma once
#include <string>
#include "IIndexBuilder.h"
#include "DocumentTable.h"
#include "Document.h"
#include "Triplet.h"
#include "FileManager.h"

using namespace std;

#define MEMORY_RATIO_USED_FOR_BUFFER 0.5
#define DEFAULT_MEMORY_LIMIT_IN_BYTE 65536

class SortBasedIndexBuilder : public IIndexBuilder {
private:
	string temporaryFilePrefixPath;
	string repositoryPath;
	string outputFilePath;
	IDictionary * iDictionary;
	ICompressor * iCompressor;
	int iTokenizerType;
	
	unsigned int numberOfBlock; //number of block in the Triplet Buffer
	unsigned int numberOfTripletInBlock; //number of triplet in each block

	// private methods
	void init(string repositoryPath, unsigned int numberOfBlock, unsigned int numberOfTripletInBlock);
	list<Triplet>* parseDocumentToTriplet(Document* document, DocumentTable* documentTable);
	unsigned int createFirstLevelSortedTripletsFiles(Triplet *tripletBuffer, DocumentTable* documentTable);
	string getFinalSortedTripletsFilesByFusion(Triplet *tripletBuffer, DocumentTable* documentTable, unsigned int lastRunNumber);
	void finalize(DocumentTable *documentTable, string sortedTripletFilePath);
	static int sort_by_termId_docId(const void *left, const void *right);

public:
	SortBasedIndexBuilder(string repositoryPath,unsigned int numberOfBlock, unsigned int numberOfTripletInBlock);
	SortBasedIndexBuilder(string repositoryPath, unsigned int memoryLimitInByte = DEFAULT_MEMORY_LIMIT_IN_BYTE);

	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setICompressor(ICompressor *iCompressor);
	IIndexBuilder* setITokenizer(int iTokenizerType);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	Index* createIndex();
	~SortBasedIndexBuilder();
};