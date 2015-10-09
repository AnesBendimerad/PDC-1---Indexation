#include "stdafx.h"
#include <fstream>
#include "Index.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include "Term.h"



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
		// we'll create a ofstream file to write on it
		// we'll iterate all the hash table, for each element that contains a non null list
		// we'll iterate for all terms in the list, for each term
		// we create a table and copy all posting list element on it,
		// we replace the void* postinglist by the offset
		// we write the posting list table in the file
		// we destruct the posting list
		ofstream outputFile(postingFilePath);

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



Index::~Index()
{
	delete dictionary;
	delete documentTable;
}


