#pragma once
#include <string>
#include "IIndexBuilder.h"
#include "DocumentTable.h"
#include "Document.h"
#include "Triplet.h"

#define TEMPORARY_FILES_DIRECTORY_PATH "C:\\Users\\anes\\Documents\\projets"
#define TEMPORARY_FILE_PREFIX_NAME "~tmp"

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

	// private methods
	list<Triplet>* parseDocumentToTriplet(Document* document, DocumentTable* documentTable);
	unsigned int createFirstLevelSortedTripletsFiles(Triplet *tripletBuffer, DocumentTable* documentTable);
	string getFinalSortedTripletsFilesByFusion(Triplet *tripletBuffer, DocumentTable* documentTable, unsigned int lastRunNumber);
	void finalize(DocumentTable *documentTable, string sortedTripletFilePath);
	static int sort_by_termId_docId(const void *left, const void *right);

public:
	sortBasedIndexBuilder(string repositoryPath,unsigned int numberOfBlock, unsigned int numberOfTripletInBlock);
	IIndexBuilder* setIDictionary(IDictionary *iDictionary);
	IIndexBuilder* setICompressor(ICompressor *iCompressor);
	IIndexBuilder* setOutputFilePath(string outputFilePath);
	IIndexBuilder* setIndexType(int indexType);
	IIndex* createIndex();
	~sortBasedIndexBuilder();
};