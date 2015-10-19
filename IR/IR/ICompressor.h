#pragma once
#include <fstream>
#include <list>
#include "DocumentTerm.h"
#define NO_COMPRESSOR 0
#define VBYTE_COMPRESSOR 1
using namespace std;
class ICompressor {
public:
	virtual void compress(ofstream outputInvertedFile, list<DocumentTerm>* postingList) = 0;
	virtual void decompress(ifstream inputInvertedFile, DocumentTerm ** postingList, int size) = 0;
};