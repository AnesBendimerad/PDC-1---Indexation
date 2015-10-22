#pragma once
#include "DocumentMetaData.h"

class DocumentTable {
private:
	unsigned int documentNumber;
	bool finalized;
	void * docTable; // it's a list at first, and a table in the end
public:
	DocumentTable();
	DocumentTable::DocumentTable(unsigned int documentNumber, DocumentMetaData * docTable);
	unsigned int addDocument(DocumentMetaData documentMetaData);
	unsigned int& getDocumentNumber();
	void finalize();
	DocumentMetaData* getDocument(unsigned int documentIndex);
	unsigned long long getMemorySize();
	DocumentMetaData* getFinalizedDocumentTable();
	~DocumentTable();
};