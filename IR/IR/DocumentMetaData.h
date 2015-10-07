#pragma once
typedef struct DocumentMetaData {
	unsigned int id;
	unsigned int address; // DOCNO
	unsigned int offset;  // document offset in the file
	unsigned int wordsNumber=0;
} DocumentMetaData;