#pragma once
#include "ICompressor.h"
class GammaCompressor : public ICompressor {
private:
	int compressorId;
	
	int readBit(ifstream *invertedFile, unsigned char * bitCode, int *currentBit);
	void writeBit(ofstream *invertedFile, unsigned char * bitCode, int *currentBit, bool bit);
	void compressUnsignedInt(ofstream *invertedFile, unsigned int number,unsigned char * bitCode, int *currentBit);
	void decompressUnsignedInt(ifstream *invertedFile, unsigned int *number, unsigned char * bitCode, int *currentBit);
	
public:
	GammaCompressor();
	void compressAndWrite(ofstream *outputInvertedFile, list<DocumentTerm>* postingList);
	void readAndDecompress(ifstream *inputInvertedFile, DocumentTerm ** postingList, int size);
	int& getCompressorId();
};
