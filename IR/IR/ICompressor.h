#pragma once
#include <fstream>
#include <list>
#include "DocumentTerm.h"
#define NO_COMPRESSOR 0
#define VBYTE_COMPRESSOR 1
#define GAMMA_COMPRESSOR 2
using namespace std;
class ICompressor {
public:
	virtual void compressAndWrite(ofstream *outputInvertedFile, list<DocumentTerm>* postingList) = 0;
	virtual void readAndDecompress(ifstream *inputInvertedFile, DocumentTerm ** postingList, int size) = 0;
	virtual int& getCompressorId()=0;
};