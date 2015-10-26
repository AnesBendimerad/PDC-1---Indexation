#include "stdafx.h"
#include "sortBasedIndexBuilder.h"
#include "Index.h"
#include "IndexBM25.h"
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
#include "StrTkTokenizer.h"
#include "NoCompressor.h"
#include "MemoryManager.h"
#include <map>
#include <iostream>
#include <stdio.h>


void sortBasedIndexBuilder::init(string repositoryPath, unsigned int numberOfBlock, unsigned int numberOfTripletInBlock)
{
	string temporaryFileDirectory = ""; // should end with '\\' if not empty , ex : "E:\\tmp\\"
	string temporaryFilePrefixName = ".~tmp_";
	sortBasedIndexBuilder::temporaryFilePrefixPath = temporaryFileDirectory + temporaryFilePrefixName;
	sortBasedIndexBuilder::repositoryPath = repositoryPath;
	sortBasedIndexBuilder::numberOfBlock = numberOfBlock;
	sortBasedIndexBuilder::numberOfTripletInBlock = numberOfTripletInBlock;
	sortBasedIndexBuilder::iDictionary = nullptr;
	sortBasedIndexBuilder::iCompressor = nullptr;
	sortBasedIndexBuilder::outputFilePath = "";
	sortBasedIndexBuilder::indexType = FAGIN_INDEX_TYPE;
	sortBasedIndexBuilder::iTokenizerType = SIMPLE_TOKENIZER;
}

sortBasedIndexBuilder::sortBasedIndexBuilder(string repositoryPath, unsigned int numberOfBlock, unsigned int numberOfTripletInBlock)
{
	sortBasedIndexBuilder::init(repositoryPath, numberOfBlock, numberOfTripletInBlock);
}

sortBasedIndexBuilder::sortBasedIndexBuilder(string repositoryPath, unsigned int memoryLimitInByte)
{
	unsigned int memoryLimitForBufferInByte = (unsigned int)(MEMORY_RATIO_USED_FOR_BUFFER*memoryLimitInByte);
	unsigned int sizeOfDiskBlock = MemoryManager::getDiskSectorSize();
	unsigned int TripletInBlockNumber = sizeOfDiskBlock / sizeof(Triplet);
	unsigned int BlockNumber = memoryLimitForBufferInByte / sizeOfDiskBlock;
	sortBasedIndexBuilder::init(repositoryPath, BlockNumber, TripletInBlockNumber);
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

IIndexBuilder * sortBasedIndexBuilder::setITokenizer(int iTokenizerType)
{
	sortBasedIndexBuilder::iTokenizerType = iTokenizerType;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setOutputFilePath(string outputFilePath)
{
	sortBasedIndexBuilder::outputFilePath = outputFilePath;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setIndexType(int indexType)
{
	if (indexType != FAGIN_INDEX_TYPE && indexType != BM25_INDEX_TYPE) {
		throw runtime_error("No Index with such ID");
	}
	sortBasedIndexBuilder::indexType = indexType;
	return this;
}

IIndex * sortBasedIndexBuilder::createIndex()
{
	// Phase 1 : Initialisation
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

	unsigned int bufferSize = numberOfBlock*numberOfTripletInBlock;
	Triplet *tripletBuffer = (Triplet*)malloc(sizeof(Triplet)*bufferSize);

	// Phase 2 : Create The runs (First level sorted Triplets Files)
	unsigned int lastRunNumber = createFirstLevelSortedTripletsFiles(tripletBuffer,documentTable);
	
	// Phase 3 : Create The runs (Merge all the sorted Triplets Files into on sorted Triplet File)
	string sortedTripletFilePath = getFinalSortedTripletsFilesByFusion(tripletBuffer, documentTable, lastRunNumber);

	// Phase 4 : Finalisation and index serialization
	finalize(documentTable, sortedTripletFilePath);
	
	// Phase 5 : In Memory Index creation
	IIndex *index = nullptr;
	switch (indexType) {
	case FAGIN_INDEX_TYPE:
		index = new Index(iDictionary, documentTable, iCompressor, outputFilePath);
		break;
	case BM25_INDEX_TYPE:
		index = new IndexBM25(iDictionary, documentTable, iCompressor, outputFilePath);
		break;
	}
	return index;
}







list<Triplet>* sortBasedIndexBuilder::parseDocumentToTriplet(Document* document, DocumentTable *documentTable)
{
	ITokenizer* tokenizer;
	switch (iTokenizerType) {
	case SIMPLE_TOKENIZER:
		tokenizer = new Tokenizer(document);
		break;
	case STRTK_TOKENIZER:
		tokenizer = new StrTkTokenizer(document);
		break;
	default:
		throw runtime_error("No Tokenizer with such ID");
		break;
	}
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

unsigned int sortBasedIndexBuilder::createFirstLevelSortedTripletsFiles(Triplet * tripletBuffer, DocumentTable* documentTable)
{
	unsigned int bufferSize = numberOfBlock*numberOfTripletInBlock;
	unsigned int bufferFirstFreeIndex = 0;
	unsigned int runNumber = 0;
	Document* document;
	list<Triplet>* tripletList = new list<Triplet>();
	list<Triplet>::iterator it;
	string path;

	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);

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
				path = temporaryFilePrefixPath + to_string(runNumber);
				ofstream outputFile(path, ios::out | ios::binary);
				outputFile.write((char *)tripletBuffer, sizeof(Triplet)*bufferFirstFreeIndex);
				outputFile.close();
				bufferFirstFreeIndex = 0;
				runNumber++;
			}
			it++;
		}
		tripletList->erase(tripletList->begin(), tripletList->end());
		delete tripletList;
	}

	if (bufferFirstFreeIndex > 0)
	{
		qsort(tripletBuffer, bufferFirstFreeIndex, sizeof(Triplet), sort_by_termId_docId);
		path = temporaryFilePrefixPath + to_string(runNumber);
		ofstream outputFile(path, ios::out | ios::binary);
		outputFile.write((const char *)tripletBuffer, sizeof(Triplet)*bufferFirstFreeIndex);
		outputFile.close();
		bufferFirstFreeIndex = 0;
		runNumber++;
	}
	delete documentProvider;

	return runNumber-1;
}

string sortBasedIndexBuilder::getFinalSortedTripletsFilesByFusion(Triplet * tripletBuffer, DocumentTable * documentTable, unsigned int lastRunNumber)
{
	unsigned int* mergedInRunIndex = (unsigned int *)malloc(sizeof(unsigned int)*(numberOfBlock - 1));
	unsigned int* mergedInRunActualFileIndex = (unsigned int *)malloc(sizeof(unsigned int)*(numberOfBlock - 1));
	bool* mergedInRunIsFileClosed = (bool *)malloc(sizeof(bool)*(numberOfBlock - 1));
	unsigned int* mergedInRunLastIndex = (unsigned int *)malloc(sizeof(unsigned int)*(numberOfBlock - 1));
	ifstream** mergedTableStream = (ifstream **)malloc(sizeof(ifstream*)*(numberOfBlock - 1));
	
	unsigned int firstSortedRunNumber = 0;
	unsigned int lastSortedRunNumber = -1;
	unsigned int outputInTableIndex = 0;
	
	bool mergeingEnd = false;
	unsigned int outputNumber = 0;
	while (!mergeingEnd)
	{

		// Initialisation of a merge
		firstSortedRunNumber = lastSortedRunNumber + 1;
		lastSortedRunNumber = firstSortedRunNumber + numberOfBlock - 2;
		if (lastSortedRunNumber > lastRunNumber)
		{
			lastSortedRunNumber = lastRunNumber;
		}

		if (lastSortedRunNumber - firstSortedRunNumber > 0)
		{
			string outputPath = temporaryFilePrefixPath + string("Merged_") + to_string(outputNumber);
			ofstream outputFile(outputPath, ios::out | ios::binary);
			for (unsigned int i = 0;i <= lastSortedRunNumber - firstSortedRunNumber;i++)
			{
				string sourcePath = temporaryFilePrefixPath + to_string(unsigned int(i + firstSortedRunNumber));
				ifstream* inputStreamI = new ifstream(sourcePath, ios::in | ios::binary);
				mergedTableStream[i] = inputStreamI;
				mergedInRunIndex[i] = 0;
				mergedInRunActualFileIndex[i] = 0;
				mergedInRunIsFileClosed[i] = false;
				streampos begin = inputStreamI->tellg();
				inputStreamI->seekg(0, ios::end);
				streampos end = inputStreamI->tellg();
				inputStreamI->seekg(0, ios::beg);
				mergedInRunLastIndex[i] = (end - begin) / sizeof(Triplet);
			}

			bool localMergingEnd = true;
			Triplet minTriplet;
			bool minEmpty = true;
			unsigned int minIndex;
			do
			{
				localMergingEnd = true;
				minEmpty = true;
				for (unsigned int i = 0;i <= lastSortedRunNumber - firstSortedRunNumber;i++)
				{
					if (mergedInRunIsFileClosed[i])
					{
						localMergingEnd = localMergingEnd & true;
					}
					else if (mergedInRunIndex[i] == mergedInRunLastIndex[i])
					{
						localMergingEnd = localMergingEnd & true;
						mergedTableStream[i]->close();
						delete mergedTableStream[i];
						string sourcePath = temporaryFilePrefixPath  + to_string(unsigned int(i + firstSortedRunNumber));
						remove(sourcePath.c_str());
						mergedInRunIsFileClosed[i] = true;
					}
					else
					{
						localMergingEnd = localMergingEnd & false;
						if (mergedInRunIndex[i] == mergedInRunActualFileIndex[i])
						{
							mergedTableStream[i]->read((char *)(tripletBuffer + i*numberOfTripletInBlock), sizeof(Triplet)*numberOfTripletInBlock);
							mergedInRunActualFileIndex[i] += numberOfTripletInBlock;
						}
						if (minEmpty)
						{
							minTriplet = tripletBuffer[i*numberOfTripletInBlock + mergedInRunIndex[i] % numberOfTripletInBlock];
							minIndex = i;
							minEmpty = false;
						}
						else
						{
							Triplet actualTriplet = tripletBuffer[i*numberOfTripletInBlock + mergedInRunIndex[i] % numberOfTripletInBlock];
							if (actualTriplet<minTriplet)
							{
								minTriplet = actualTriplet;
								minIndex = i;
							}
						}
					}
				}
				if (!localMergingEnd)
				{
					tripletBuffer[(numberOfBlock - 1)*numberOfTripletInBlock + outputInTableIndex] = minTriplet;
					mergedInRunIndex[minIndex]++;
					outputInTableIndex++;
					if (outputInTableIndex == numberOfTripletInBlock)
					{
						outputFile.write((char *)(tripletBuffer + (numberOfBlock - 1)*numberOfTripletInBlock), sizeof(Triplet)*outputInTableIndex);
						outputInTableIndex = 0;
					}
				}
				else if (outputInTableIndex>0)
				{
					outputFile.write((char *)(tripletBuffer + (numberOfBlock - 1)*numberOfTripletInBlock), sizeof(Triplet)*outputInTableIndex);
					outputInTableIndex = 0;
				}
			} while (!localMergingEnd);


			// Finalize runs
			outputFile.close();
			//rename the file 
			string lastOutputPath = temporaryFilePrefixPath + to_string(outputNumber);
			rename(outputPath.c_str(), lastOutputPath.c_str());
		}
		else
		{
			string ancientName = temporaryFilePrefixPath + to_string(firstSortedRunNumber);
			string lastOutputPath = temporaryFilePrefixPath + to_string(outputNumber);
			rename(ancientName.c_str(), lastOutputPath.c_str());
		}
		if (lastSortedRunNumber < lastRunNumber)
		{
			outputNumber++;
		}
		else
		{
			lastSortedRunNumber = -1;
			lastRunNumber = outputNumber;
			if (outputNumber == 0)
			{
				mergeingEnd = true;
			}
			outputNumber = 0;
		}
	}

	string lastOutputPath = temporaryFilePrefixPath + to_string(outputNumber);
	string sortedTripletFilePath = temporaryFilePrefixPath + string("merged");
	rename(lastOutputPath.c_str(), sortedTripletFilePath.c_str());

	free(mergedInRunIndex);
	free(mergedInRunLastIndex);
	free(mergedInRunActualFileIndex);
	free(mergedInRunIsFileClosed);
	free(mergedTableStream);
	free(tripletBuffer);

	return sortedTripletFilePath;
}

void sortBasedIndexBuilder::finalize(DocumentTable * documentTable,string sortedTripletFilePath)
{
	ifstream* inputStreamI = new ifstream(sortedTripletFilePath, ios::in | ios::binary);
	ofstream outputFile(outputFilePath, ios::out | ios::binary);

	Triplet currentTriplet;
	DocumentTerm currentDocumentTerm;
	Term* currentTerm = nullptr;
	Term* ancientTerm = nullptr;
	list<DocumentTerm> currentPostingList;

	documentTable->finalize();
	// write the posting list on file and change the pointer to offset in file (invertedFilePath)
	// the offstream structure file is :
	//		Dictionary offset in this file
	//		Terms number in Dictionary
	//		Compressor ID
	//		DocumentMetaDatas number in DocumentTable
	//		DocumentMetaDatas
	//		posting lists
	//		Dictionary

	// prepare a place for the dictionary offset
	unsigned int offsetPlacer = 0;
	outputFile.write((const char *)&offsetPlacer, sizeof(unsigned int));

	// write the terms number in HashTableDictionary
	outputFile.write((const char *)&iDictionary->getTermsNumber(), sizeof(unsigned long long));

	// write the Compressor ID
	outputFile.write((const char *)&iCompressor->getCompressorId(), sizeof(int));

	// write the DocumentMetaDatas number in DocumentTable
	outputFile.write((const char *)&documentTable->getDocumentNumber(), sizeof(unsigned int));

	// write the DocumentMetaDatas
	outputFile.write((const char *)documentTable->getFinalizedDocumentTable(), documentTable->getDocumentNumber()*sizeof(DocumentMetaData));

	// write the posting lists contiguously
	do
	{
		*inputStreamI >> &currentTriplet;
		ancientTerm = currentTerm;
		currentTerm = iDictionary->getTermById(currentTriplet.termId);
		if (ancientTerm != nullptr && ancientTerm != currentTerm) {
			ancientTerm->documentNumber = currentPostingList.size();
			ancientTerm->postingList = (void *)(unsigned int)outputFile.tellp();
			iCompressor->compressAndWrite(&outputFile, &currentPostingList);
			currentPostingList.erase(currentPostingList.begin(), currentPostingList.end());
		}
		currentDocumentTerm.documentIndex = currentTriplet.documentId;
		currentDocumentTerm.ftd = currentTriplet.ftd;
		currentPostingList.push_back(currentDocumentTerm);

	} while (!inputStreamI->eof());


	// save the dictionary offset
	unsigned int  dictionaryOffset = (unsigned int)outputFile.tellp();

	// write the terms of the dictionary
	IIterator* termIteratorDictionary = iDictionary->getIterator();
	Term * termDictionary;
	while ((termDictionary = static_cast<Term*>(termIteratorDictionary->getNext())) != nullptr) {
		outputFile << termDictionary;
	}
	outputFile.seekp(0);
	outputFile.write((const char *)&dictionaryOffset, sizeof(unsigned int));
	outputFile.close();
	inputStreamI->close();
	
	delete inputStreamI;
	delete termIteratorDictionary;
	remove(sortedTripletFilePath.c_str());
}

int sortBasedIndexBuilder:: sort_by_termId_docId(const void *left, const void *right)
{
	Triplet triplet1 = *(Triplet*)left;
	Triplet triplet2 = *(Triplet*)right;
	if (triplet1==triplet2)
	{
		return 0;
	} 
	else if (triplet1<triplet2)
	{
		return -1;
	}
	return 1;
}

sortBasedIndexBuilder::~sortBasedIndexBuilder()
{
	//[Aymen] i don't know what we should do in this destructor
}