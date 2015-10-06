#include "stdafx.h"
#include <list>
#include "DocumentTable.h"
#include "DocumentMetaData.h"

using namespace std;

DocumentTable::DocumentTable()
{
	documentNumber = 0;
	docTable = new list<DocumentMetaData>();
}

int DocumentTable::addDocument(DocumentMetaData documentMetaData)
{
	list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
	docTableAsList->insert(docTableAsList->end(), documentMetaData);
	documentNumber += 1;
	return 0;
}

int DocumentTable::getDocumentNumber()
{
	return documentNumber;
}

void DocumentTable::finalize()
{
	list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
	DocumentMetaData* docTableAsTable = (DocumentMetaData*) malloc(sizeof(DocumentMetaData)*documentNumber);
	list<DocumentMetaData>::iterator it;
	int i = 0;
	for (it = docTableAsList->begin(); it != docTableAsList->end(); ++it)
	{
		docTableAsTable[i++] = *it;
	}
	delete docTable;
	docTable = docTableAsTable;
}

DocumentMetaData DocumentTable::getDocument(int documentIndex)
{
	list<DocumentMetaData>* docTableAsList = static_cast<list<DocumentMetaData>*>(docTable);
	list<DocumentMetaData>::iterator it;
	it = docTableAsList->begin();
	while (documentIndex > 0)
	{
		it++;
		documentIndex--;
	}
	return *it;

	// Error case : documentIndex > documentNumber not treated 
}

DocumentTable::~DocumentTable()
{
	delete docTable;
}
