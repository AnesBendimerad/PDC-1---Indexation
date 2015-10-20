#pragma once
#include "ICompressor.h"
class VByteCompressor : public ICompressor {
private:
	int compressorId;
	void compressUnsignedInt(ofstream *invertedFile, unsigned int number);
	void decompressUnsignedInt(ifstream *invertedFile, unsigned int * number);
public:
	VByteCompressor();
	void compressAndWrite(ofstream *outputInvertedFile, list<DocumentTerm>* postingList);
	void readAndDecompress(ifstream *inputInvertedFile, DocumentTerm ** postingList, int size);
	int& getCompressorId();
};
