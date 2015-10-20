#include "stdafx.h"
#include "sortBasedIndexBuilder.h"
#include "Index.h"
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
#include "NoCompressor.h"
#include <map>
#include <iostream>

sortBasedIndexBuilder::sortBasedIndexBuilder(string repositoryPath, unsigned int numberOfBlock, unsigned int numberOfTripletInBlock)
{
	sortBasedIndexBuilder::repositoryPath = repositoryPath;
	sortBasedIndexBuilder::numberOfBlock = numberOfBlock;
	sortBasedIndexBuilder::numberOfTripletInBlock = numberOfTripletInBlock;
	iDictionary = nullptr;
	iCompressor = nullptr;
	outputFilePath = "";
	indexType = FAGIN_INDEX_TYPE;
}

IIndexBuilder * sortBasedIndexBuilder::setIDictionary(IDictionary * iDictionary)
{
	sortBasedIndexBuilder::iDictionary = iDictionary;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setICompressor(ICompressor * iCompressor)
{
	sortBasedIndexBuilder::iCompressor = iCompressor;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setOutputFilePath(string outputFilePath)
{
	sortBasedIndexBuilder::outputFilePath = outputFilePath;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setIndexType(int indexType)
{
	sortBasedIndexBuilder::indexType = indexType;
	return this;
}

IIndex * sortBasedIndexBuilder::createIndex()
{
	// Initialisation phase
	if (iDictionary == nullptr) {
		iDictionary = new HashTableDictionary();
	}
	
	if (iCompressor == nullptr) {
		iCompressor = new NoCompressor();
	}
	
	DocumentTable * documentTable = new DocumentTable();

	if (outputFilePath.compare("") == 0) {
		outputFilePath = DEFAULT_OUTPUT_FILE;
	}

	// Run Phase (Creation of sorted Runs)
	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);

	unsigned int runNumber = 0;
	unsigned int bufferSize = numberOfBlock*numberOfTripletInBlock;
	Triplet *tripletBuffer = (Triplet*)malloc(sizeof(Triplet)*bufferSize);
	unsigned int bufferFirstFreeIndex = 0;


	Document* document;
	list<Triplet>* tripletList = new list<Triplet>();
	list<Triplet>::iterator it;
	string path;

	while ((document = documentProvider->getNextDocument()) != nullptr)
	{
		tripletList = parseDocumentToTriplet(document, documentTable);
		it = tripletList->begin();
		while (it != tripletList->end())
		{
			tripletBuffer[bufferFirstFreeIndex++] = *it;
			if (bufferFirstFreeIndex == bufferSize)
			{
				qsort(tripletBuffer, bufferFirstFreeIndex, sizeof(Triplet), sort_by_termId_docId);
				path = TEMPORARY_FILES_DIRECTORY_PATH +string("\\")+ TEMPORARY_FILE_PREFIX_NAME + string("_")+ to_string(runNumber);
				ofstream outputFile(path, ios::out | ios::binary);
				outputFile.write((char *)tripletBuffer, sizeof(Triplet)*bufferFirstFreeIndex);
				outputFile.close();
				bufferFirstFreeIndex = 0;
				runNumber++;
			}
			it++;
		}
	}

	unsigned int lastRunSize = bufferFirstFreeIndex;
	if (lastRunSize == 0) {
		lastRunSize = bufferSize;
	}

	if (bufferFirstFreeIndex > 0)
	{
		qsort(tripletBuffer, bufferFirstFreeIndex, sizeof(Triplet), sort_by_termId_docId);
		path = TEMPORARY_FILES_DIRECTORY_PATH + string("\\") + TEMPORARY_FILE_PREFIX_NAME + string("_") + to_string(runNumber);
		ofstream outputFile(path, ios::out | ios::binary);
		outputFile.write((const char *)tripletBuffer, sizeof(Triplet)*bufferFirstFreeIndex);
		outputFile.close();
		bufferFirstFreeIndex = 0;
		runNumber++;
	}
	delete documentProvider;

	// Merging Runs Phase
	path = TEMPORARY_FILES_DIRECTORY_PATH + string("\\") + TEMPORARY_FILE_PREFIX_NAME + string("_") + to_string(0);
	ifstream inputStream(path, ios::in | ios::binary);

	Triplet *ReadtripletBuffer = (Triplet*)malloc(sizeof(Triplet)*bufferSize);

	inputStream.read((char *)ReadtripletBuffer, sizeof(Triplet)*bufferSize);

	for (int k = 0;k < bufferSize;k++)
	{
		Triplet t = ReadtripletBuffer[k];
		Term *tt = iDictionary->getTermById(t.termId);
		cout << "<" << t.termId << ":" << tt->token << "," << t.documentId << "," << t.ftd << ">" << endl;
	}

	// Finalisation
	IIndex *index = nullptr;
	if (indexType == FAGIN_INDEX_TYPE) {
		index = new Index(iDictionary, documentTable, iCompressor, outputFilePath);
	}
	return index;
}





list<Triplet>* sortBasedIndexBuilder::parseDocumentToTriplet(Document* document, DocumentTable *documentTable)
{
	ITokenizer* tokenizer = new Tokenizer(document);
	string token;
	unsigned long long termId;
	Term* term;

	map<unsigned long long, unsigned int> termDocumentFrequencyMap;
	map<unsigned long long, unsigned int>::iterator it;

	while ((token = tokenizer->getNextToken()).compare("") != 0)
	{
		term = iDictionary->addTerm(token);
		termId = iDictionary->getTokenId(token);
		
		it = termDocumentFrequencyMap.find(termId);

		if (it == termDocumentFrequencyMap.end())
		{
			termDocumentFrequencyMap[termId] = 1;
		}
		else 
		{
			termDocumentFrequencyMap[termId] += 1;
		}
	}
	
	unsigned int documentId=documentTable->addDocument(document->getDocumentMetaData());
	delete document;
	delete tokenizer;

	it = termDocumentFrequencyMap.begin();

	Triplet triplet;
	list<Triplet>* tripletList = new list<Triplet>();
	while (it != termDocumentFrequencyMap.end())
	{
		triplet.termId = it->first;
		triplet.ftd = it->second;
		triplet.documentId = documentId;
		tripletList->push_back(triplet);
		it++;
	}
	return tripletList;
}

int sortBasedIndexBuilder:: sort_by_termId_docId(const void *left, const void *right)
{
	Triplet triplet1 = *(Triplet*)left;
	Triplet triplet2 = *(Triplet*)right;
	if ((triplet1.termId == triplet2.termId) && (triplet1.documentId == triplet2.documentId))
	{
		return 0;
	} 
	else if ((triplet1.termId < triplet2.termId) || (triplet1.documentId == triplet2.documentId && triplet1.documentId < triplet2.documentId))
	{
		return -1;
	}
	return 1;
}

sortBasedIndexBuilder::~sortBasedIndexBuilder()
{
	//[Aymen] i don't know what we should do in this destructor
}