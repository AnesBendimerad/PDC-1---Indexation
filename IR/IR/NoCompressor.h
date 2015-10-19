#pragma once
#include "ICompressor.h"
class NoCompressor : public ICompressor {
public:
	void compress(ofstream outputInvertedFile, list<DocumentTerm>* postingList);
	void decompress(ifstream inputInvertedFile, DocumentTerm ** postingList, int size);
};
