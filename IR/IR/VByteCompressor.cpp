#include "stdafx.h"
#include "VByteCompressor.h"

VByteCompressor::VByteCompressor()
{
	VByteCompressor::compressorId = VBYTE_COMPRESSOR;
}


void VByteCompressor::compressUnsignedInt(ofstream * invertedFile, unsigned int number)
{
	int compressedSize;
	if (number != 0) {
		compressedSize= int(ceil((floor(log(double(number)) / log(2)) + 1) / 7));
	}
	else {
		compressedSize = 1;
	}
	unsigned char * compressedNumber = (unsigned char *)malloc(sizeof(unsigned char)*compressedSize);
	for (int i = 0; i < compressedSize; i++) {
		compressedNumber[i] = number % 128;
		number /= 128;
	}
	compressedNumber[compressedSize - 1] += 128;
	invertedFile->write((char *)compressedNumber, compressedSize*sizeof(unsigned char));
}

void VByteCompressor::decompressUnsignedInt(ifstream * invertedFile, unsigned int * number)
{
	*number = 0;
	int i = 0;
	int p = 1;
	bool continu = true;
	unsigned char readChar;
	while (i < 5 && continu) {
		invertedFile->read((char *)&readChar, sizeof(char));
		if (readChar < 128) {
			*number += p*readChar;
		}
		else {
			*number += p*(readChar - 128);
			continu = false;
		}
		i++;
		p *= 128;
	}
}

void VByteCompressor::compressAndWrite(ofstream * outputInvertedFile, list<DocumentTerm>* postingList)
{
	list<DocumentTerm>::iterator it;
	int delta = 0;
	int precedendValue = 0;
	for (it = postingList->begin(); it != postingList->end(); ++it)
	{
		delta = it->documentIndex - precedendValue;
		precedendValue = it->documentIndex;
		compressUnsignedInt(outputInvertedFile, delta);
		compressUnsignedInt(outputInvertedFile, it->ftd);
	}
}

void VByteCompressor::readAndDecompress(ifstream * inputInvertedFile, DocumentTerm ** postingList, int size)
{
	DocumentTerm *myPostingList = (DocumentTerm*)malloc(sizeof(DocumentTerm)*size);
	unsigned int delta = 0;
	unsigned int realNumber = 0;
	for (int i = 0; i < size; i++) {
		DocumentTerm docTerm;
		decompressUnsignedInt(inputInvertedFile, &delta);
		realNumber += delta;
		docTerm.documentIndex = realNumber;
		decompressUnsignedInt(inputInvertedFile, &(docTerm.ftd));
		myPostingList[i] = docTerm;
	}
	*postingList = myPostingList;
}

int & VByteCompressor::getCompressorId()
{
	return compressorId;
}
