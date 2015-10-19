#include "stdafx.h"
#include <list>
#include "DocumentTable.h"
#include "DocumentMetaData.h"
#include <typeinfo>
#include <iostream>

using namespace std;


DocumentTable::DocumentTable()
{
	documentNumber = 0;
	finalized = false;
	docTable = new list<DocumentMetaData>();
}

unsigned int DocumentTable::addDocument(DocumentMetaData documentMetaData)
{
	if (finalized) {
		throw runtime_error("Document table already finalized");
	}
	list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
	docTableAsList->insert(docTableAsList->end(), documentMetaData);
	documentNumber += 1;
	return documentNumber-1;
}

unsigned long long& DocumentTable::getDocumentNumber()
{
	return documentNumber;
}

void DocumentTable::finalize()
{
	if (! finalized) {
		list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
		DocumentMetaData* docTableAsTable = (DocumentMetaData*)malloc(sizeof(DocumentMetaData)*documentNumber);
		list<DocumentMetaData>::iterator it;
		int i = 0;
		for (it = docTableAsList->begin(); it != docTableAsList->end(); ++it)
		{
			docTableAsTable[i++] = *it;
		}
		delete docTable;
		docTable = docTableAsTable;
		finalized = true;
	}
	else
	{
		throw runtime_error("Document table finalized yet");
	}
}

DocumentMetaData* DocumentTable::getDocument(int documentIndex)
{

	if (finalized) {
		if (documentIndex >= documentNumber)
		{
			return nullptr;
		}
		else
		{
			DocumentMetaData* docTableAsTable = static_cast<DocumentMetaData*>(docTable);
			return &(docTableAsTable[documentIndex]);
		}
	}
	else
	{
		throw runtime_error("Document table not finalized yet");
	}
	
}

unsigned long long DocumentTable::getMemorySize()
{
	return documentNumber*sizeof(DocumentMetaData)+sizeof(unsigned long long)+sizeof(bool) + sizeof(void*);
}

DocumentMetaData * DocumentTable::getFinalizedDocumentTable()
{
	if (!finalized) {
		throw runtime_error("Document table not finalized yet");
	}
	return static_cast<DocumentMetaData*>(docTable);
}


DocumentTable::~DocumentTable()
{
	delete docTable;
}
