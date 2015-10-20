#pragma once
#include "ICompressor.h"
class NoCompressor : public ICompressor {
private:
	int compressorId;
public:
	NoCompressor();
	void compressAndWrite(ofstream *outputInvertedFile, list<DocumentTerm>* postingList);
	void readAndDecompress(ifstream *inputInvertedFile, DocumentTerm ** postingList, int size);
	int& getCompressorId();
};
