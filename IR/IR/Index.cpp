#include "stdafx.h"
#include "Index.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "Term.h"

Index::Index(IDictionary *dictionary)
{
	Index::dictionary = dictionary;
	documentTable = new DocumentTable();
}

int Index::addDocument(DocumentMetaData documentMetaData)
{
	return documentTable->addDocument(documentMetaData);
}

void Index::addTerm(string token)
{
	Term* term = Index::dictionary->addTerm(token);
	// The next step is to update the term posting list and document number
}

void Index::finalize()
{
	// write the posting list on file and change the pointer to offset in file
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


