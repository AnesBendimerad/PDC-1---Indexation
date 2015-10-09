#pragma once
#include "DocumentMetaData.h"

class DocumentTable {
private:
	unsigned int documentNumber;
	bool finalized;
	void * docTable; // it's a list at first, and a table in the end
public:
	DocumentTable();
	unsigned int addDocument(DocumentMetaData documentMetaData);
	unsigned int getDocumentNumber();
	void finalize();
	DocumentMetaData* getDocument(int documentIndex);
	~DocumentTable();
};