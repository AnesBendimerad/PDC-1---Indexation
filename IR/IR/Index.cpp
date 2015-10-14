#include "stdafx.h"
#include <fstream>
#include "Index.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include "Term.h"
#include <iostream>



Index::Index(IDictionary *dictionary, string outputFilePath)
{
	Index::dictionary = dictionary;
	documentTable = new DocumentTable();
	finalized = false;
	postingFilePath = outputFilePath;
}

int Index::addDocument(DocumentMetaData documentMetaData)
{
	return documentTable->addDocument(documentMetaData);
}

void Index::addTerm(string token)
{
	Term* term = Index::dictionary->addTerm(token);
	// The next step is to update the term posting list and document number
	if (term->postingList == nullptr) {
		term->postingList= new list<DocumentTerm>();
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
		if (documentTable->getDocumentNumber()==it->documentIndex) {
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

void Index::finalize()
{
	if (!finalized) {
		documentTable->finalize();
		// write the posting list on file and change the pointer to offset in file (postingFilePath)
		// the offstream structure file is :
		//		Dictionary offset
		//		Terms number in Dictionary
		//		DocumentMetaDatas number in DocumentTable
		//		DocumentMetaDatas
		//		posting lists
		//		Dictionary
		ofstream outputFile(postingFilePath);
		// prepare a place for the dictionary offset
		outputFile.write((const char *)nullptr, sizeof(void *));
		// write the terms number in Dictionary
		outputFile.write((const char *)dictionary->getTermsNumber(), sizeof(unsigned long long));
		// write the DocumentMetaDatas number in DocumentTable
		outputFile.write((const char *)documentTable->getDocumentNumber(), sizeof(unsigned long long));
		// write the DocumentMetaDatas
		outputFile.write((const char *)documentTable->getFinalizedDocumentTable(),documentTable->getDocumentNumber()*sizeof(DocumentMetaData));
		// write the posting lists contiguously
		IIterator* termIterator = dictionary->getIterator();
		Term * term;
		while ((term = static_cast<Term*>(termIterator->getNext())) != nullptr) {
			list<DocumentTerm>* postingListAsList = static_cast<list<DocumentTerm>*>(term->postingList);
			DocumentTerm * docTermTable= (DocumentTerm*)malloc(sizeof(DocumentTerm)*term->documentNumber);
			list<DocumentTerm>::iterator it;
			int i = 0;
			for (it = postingListAsList->begin(); it != postingListAsList->end(); ++it)
			{
				docTermTable[i++] = *it;
			}
			delete term->postingList;
			term->postingList = (void *) (unsigned int)outputFile.tellp();
			outputFile.write((const char *)docTermTable, (sizeof(DocumentTerm)*term->documentNumber));
			free(docTermTable);
		}
		delete termIterator;
		// write the terms of the dictionary
		// TODO
		outputFile.close();
		finalized = true;
		
	}
	
	else 
	{
		throw runtime_error("Index finalized yet");
	}
}

list<int> Index::search(string querry)
{
	// return the most relevent  document to the query
	return list<int>();
}

DocumentTerm * Index::getTermPostingList(string token)
{
	ifstream inputStream(postingFilePath);
	Term * term = dictionary->getTerm(token);
	if (term != nullptr) {
		DocumentTerm* documentTermTable= (DocumentTerm*)malloc(sizeof(DocumentTerm)*term->documentNumber);
		inputStream.seekg((unsigned int) term->postingList);
		inputStream.read((char * )documentTermTable, sizeof(DocumentTerm)*term->documentNumber);
		inputStream.close();
		return documentTermTable;
	}
	inputStream.close();
	return nullptr;
}



Index::~Index()
{
	delete dictionary;
	delete documentTable;
}


