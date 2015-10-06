#pragma once
#include "DocumentMetaData.h"

class DocumentTable {
private:
	int documentNumber;
	void * docTable; // it's a list at first, and a table in the end
public:
	DocumentTable();
	int addDocument(DocumentMetaData documentMetaData);
	int getDocumentNumber();
	void finalize();
	DocumentMetaData getDocument(int documentIndex);
	~DocumentTable();
};