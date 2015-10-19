#pragma once
#include "DocumentMetaData.h"

class DocumentTable {
private:
	unsigned long long documentNumber;
	bool finalized;
	void * docTable; // it's a list at first, and a table in the end
public:
	DocumentTable();
	unsigned int addDocument(DocumentMetaData documentMetaData);
	unsigned long long& getDocumentNumber();
	void finalize();
	DocumentMetaData* getDocument(int documentIndex);
	unsigned long long getMemorySize();
	DocumentMetaData* getFinalizedDocumentTable();
	~DocumentTable();
};