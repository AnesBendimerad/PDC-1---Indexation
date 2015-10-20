#include "stdafx.h"
#include "NoCompressor.h"

NoCompressor::NoCompressor()
{
	NoCompressor::compressorId = NO_COMPRESSOR;
}

void NoCompressor::compressAndWrite(ofstream *outputInvertedFile, list<DocumentTerm>* postingList)
{
	DocumentTerm * docTermTable = (DocumentTerm*)malloc(sizeof(DocumentTerm)*(postingList->size()));
	list<DocumentTerm>::iterator it;
	int i = 0;
	for (it = postingList->begin(); it != postingList->end(); ++it)
	{
		docTermTable[i++] = *it;
	}
	outputInvertedFile->write((const char *)docTermTable, (sizeof(DocumentTerm)*(postingList->size())));
	free(docTermTable);
}

void NoCompressor::readAndDecompress(ifstream *inputInvertedFile, DocumentTerm ** postingList, int size)
{
	*postingList = (DocumentTerm*)malloc(sizeof(DocumentTerm)*size);
	inputInvertedFile->read((char *)*postingList, sizeof(DocumentTerm)*size);
}

int & NoCompressor::getCompressorId()
{
	return compressorId;
}