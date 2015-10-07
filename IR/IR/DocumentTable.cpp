#include "stdafx.h"
#include <list>
#include "DocumentTable.h"
#include "DocumentMetaData.h"
#include <typeinfo>

using namespace std;


DocumentTable::DocumentTable()
{
	documentNumber = 0;
	finilized = false;
	docTable = new list<DocumentMetaData>();
}

int DocumentTable::addDocument(DocumentMetaData documentMetaData)
{
	list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
	docTableAsList->insert(docTableAsList->end(), documentMetaData);
	documentNumber += 1;
	return documentNumber-1;
}

int DocumentTable::getDocumentNumber()
{
	return documentNumber;
}

void DocumentTable::finalize()
{
	if (not finilized) {
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
		finilized = true;
	}
	else
	{
		throw runtime_error("Index finilized yet");
	}
}

DocumentMetaData* DocumentTable::getDocument(int documentIndex)
{

	if (finilized) {
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
		throw runtime_error("Index not finilized yet");
	}
	
}

DocumentTable::~DocumentTable()
{
	delete docTable;
}
