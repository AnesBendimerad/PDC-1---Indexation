#include "stdafx.h"
#include "InMemoryIndexBuilder.h"
#include "Index.h"
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
InMemoryIndexBuilder::InMemoryIndexBuilder(string repositoryPath)
{
	InMemoryIndexBuilder::repositoryPath = repositoryPath;
	iDictionary = nullptr;
	outputFilePath = "";
	indexType = FAGIN_INDEX_TYPE;
}

IIndexBuilder * InMemoryIndexBuilder::setIDictionary(IDictionary * iDictionary)
{
	InMemoryIndexBuilder::iDictionary = iDictionary;
	return this;
}

IIndexBuilder * InMemoryIndexBuilder::setOutputFilePath(string outputFilePath)
{
	InMemoryIndexBuilder::outputFilePath = outputFilePath;
	return this;
}

IIndexBuilder * InMemoryIndexBuilder::setIndexType(int indexType)
{
	InMemoryIndexBuilder::indexType = indexType;
	return this;
}



IIndex* InMemoryIndexBuilder::createIndex()
{
	if (iDictionary == nullptr) {
		iDictionary = new HashTableDictionary();
	}
	DocumentTable * documentTable = new DocumentTable();

	if (outputFilePath.compare("")==0) {
		outputFilePath = DEFAULT_OUTPUT_FILE;
	}
	
	
	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);

	Document* document;
	while ((document = documentProvider->getNextDocument()) != nullptr)
	{
		ITokenizer* tokenizer = new Tokenizer(document);
		string token;
		while ((token = tokenizer->getNextToken()).compare("") != 0)
		{
			addTerm(token, documentTable);
		}
		documentTable->addDocument(document->getDocumentMetaData());
		delete document;
		delete tokenizer;
	}
	delete documentProvider;
	finalize(documentTable);
	IIndex *index = nullptr;
	if (indexType == FAGIN_INDEX_TYPE) {
		index = new Index(iDictionary, documentTable, outputFilePath);
	}
	return index;
}
void InMemoryIndexBuilder::addTerm(string token, DocumentTable *documentTable) {
	Term* term = iDictionary->addTerm(token);
	// The next step is to update the term posting list and document number
	if (term->postingList == nullptr) {
		term->postingList = new list<DocumentTerm>();
		list<DocumentTerm>* postingListAsList = static_cast<list<DocumentTerm>*>(term->postingList);
		DocumentTerm docTerm;
		docTerm.documentIndex = documentTable->getDocumentNumber();
		postingListAsList->insert(postingListAsList->begin(), docTerm);
		term->documentNumber++;

	}
	else {
		list<DocumentTerm>* postingListAsList = static_cast<list<DocumentTerm>*>(term->postingList);
		list<DocumentTerm>::iterator it = postingListAsList->end();
		it--;
		if (documentTable->getDocumentNumber() == it->documentIndex) {
			it->ftd++;
		}
		else {
			DocumentTerm docTerm;
			docTerm.documentIndex = documentTable->getDocumentNumber();
			postingListAsList->insert(postingListAsList->end(), docTerm);
			term->documentNumber++;
		}
	}
}
void InMemoryIndexBuilder::finalize(DocumentTable * documentTable)
{

		documentTable->finalize();
		// write the posting list on file and change the pointer to offset in file (invertedFilePath)
		// the offstream structure file is :
		//		HashTableDictionary offset in this file
		//		Terms number in HashTableDictionary
		//		DocumentMetaDatas number in DocumentTable
		//		DocumentMetaDatas
		//		posting lists
		//		HashTableDictionary
		ofstream outputFile(outputFilePath, ios::out | ios::binary);

		// prepare a place for the dictionary offset
		unsigned int offsetPlacer = 0;
		outputFile.write((const char *)&offsetPlacer, sizeof(unsigned int));

		// write the terms number in HashTableDictionary
		outputFile.write((const char *)&iDictionary->getTermsNumber(), sizeof(unsigned long long));

		// write the DocumentMetaDatas number in DocumentTable
		outputFile.write((const char *)&documentTable->getDocumentNumber(), sizeof(unsigned long long));

		// write the DocumentMetaDatas
		outputFile.write((const char *)documentTable->getFinalizedDocumentTable(), documentTable->getDocumentNumber()*sizeof(DocumentMetaData));

		// write the posting lists contiguously
		IIterator* termIterator = iDictionary->getIterator();
		Term * term;
		while ((term = static_cast<Term*>(termIterator->getNext())) != nullptr) {
			list<DocumentTerm>* postingListAsList = static_cast<list<DocumentTerm>*>(term->postingList);
			DocumentTerm * docTermTable = (DocumentTerm*)malloc(sizeof(DocumentTerm)*term->documentNumber);
			list<DocumentTerm>::iterator it;
			int i = 0;
			for (it = postingListAsList->begin(); it != postingListAsList->end(); ++it)
			{
				docTermTable[i++] = *it;
			}
			delete term->postingList;
			term->postingList = (void *)(unsigned int)outputFile.tellp();
			outputFile.write((const char *)docTermTable, (sizeof(DocumentTerm)*term->documentNumber));
			free(docTermTable);
		}
		delete termIterator;

		// save the dictionary offset
		unsigned int  dictionaryOffset = (unsigned int)outputFile.tellp();

		// write the terms of the dictionary
		termIterator = iDictionary->getIterator();
		while ((term = static_cast<Term*>(termIterator->getNext())) != nullptr) {
			outputFile << term;
		}
		outputFile.seekp(0);
		outputFile.write((const char *)&dictionaryOffset, sizeof(unsigned int));
		outputFile.close();
		delete termIterator;
		
}



InMemoryIndexBuilder::~InMemoryIndexBuilder()
{
	//[Aymen] i don't know what we should do in this destructor
}
