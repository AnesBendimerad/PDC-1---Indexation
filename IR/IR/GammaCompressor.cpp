#include "stdafx.h"
#include "GammaCompressor.h"

GammaCompressor::GammaCompressor()
{
	GammaCompressor::compressorId = GAMMA_COMPRESSOR;
}

int GammaCompressor::readBit(ifstream * invertedFile, unsigned char * bitCode, int * currentBit)
{
	(*currentBit)++;
	int bitRead = *bitCode/int(pow(2, 8-*currentBit));
	*bitCode=*bitCode%int(pow(2, 8 - *currentBit));
	if (*currentBit == 8) {
		*currentBit = 0;
		invertedFile->read((char *)bitCode, sizeof(unsigned char));
	}
	return bitRead;
}

void GammaCompressor::writeBit(ofstream * invertedFile, unsigned char * bitCode, int * currentBit, bool bit)
{
	// bit is 0 or 1
	(*currentBit)++;
	*bitCode = *bitCode * 2 + bit;
	if (*currentBit == 8) {
		invertedFile->write((char *)bitCode, sizeof(unsigned char));
		*currentBit = 0;
		*bitCode = 0;
	}
	
}

void GammaCompressor::compressUnsignedInt(ofstream * invertedFile, unsigned int number, unsigned char * bitCode, int * currentBit)
{
	int numberSizeByBit;
	int cpt;
	if (number > 0) {
		numberSizeByBit = int(floor(log(number) / log(2)))+1;
	}
	else {
		numberSizeByBit = 0;
	}
	cpt = numberSizeByBit;
	while (cpt > 1) {
		writeBit(invertedFile, bitCode, currentBit, true);
		cpt--;
	}
	writeBit(invertedFile, bitCode, currentBit, false);
	if (number == 0) {
		writeBit(invertedFile, bitCode, currentBit, false);
	}
	else if (number == 1) {
		writeBit(invertedFile, bitCode, currentBit, true);
	}
	else {
		cpt = numberSizeByBit-1;
		number =number % int(pow(2, cpt));
		while (cpt > 0) {
			writeBit(invertedFile, bitCode, currentBit, number / int(pow(2, cpt - 1)) == 1);
			number = number % int(pow(2, cpt-1));
			cpt--;
		}
	}

}

void GammaCompressor::decompressUnsignedInt(ifstream * invertedFile, unsigned int * number, unsigned char * bitCode, int * currentBit)
{
	int numberSizeByBit = 1;
	while (readBit(invertedFile, bitCode, currentBit) == 1) {
		numberSizeByBit++;
	}
	if (numberSizeByBit == 1) {
		if (readBit(invertedFile, bitCode, currentBit) == 0) {
			*number = 0;
		}
		else {
			*number = 1;
		}
	}
	else {
		*number = 1;
		while (numberSizeByBit > 1) {
			*number = *number * 2 + readBit(invertedFile, bitCode, currentBit);
			numberSizeByBit--;
		}
	}
}


void GammaCompressor::compressAndWrite(ofstream * outputInvertedFile, list<DocumentTerm>* postingList)
{
	list<DocumentTerm>::iterator it;
	int delta = 0;
	int precedendValue = 0;
	unsigned char bitCode = 0;
	int currentBit = 0;
	for (it = postingList->begin(); it != postingList->end(); ++it)
	{
		delta = it->documentIndex - precedendValue;
		precedendValue = it->documentIndex;
		compressUnsignedInt(outputInvertedFile, delta,&bitCode,&currentBit);
		compressUnsignedInt(outputInvertedFile, it->ftd, &bitCode, &currentBit);
	}
	if (currentBit != 0) {
		int initial = currentBit;
		for (int i = initial; i < 8; i++) {
			writeBit(outputInvertedFile, &bitCode, &currentBit, false);
		}
	}
}

void GammaCompressor::readAndDecompress(ifstream * inputInvertedFile, DocumentTerm ** postingList, int size)
{
	DocumentTerm *myPostingList = (DocumentTerm*)malloc(sizeof(DocumentTerm)*size);
	unsigned int delta = 0;
	unsigned int realNumber = 0;
	unsigned char bitCode = 0;
	int currentBit = 7;
	readBit(inputInvertedFile, &bitCode, &currentBit);
	for (int i = 0; i < size; i++) {
		DocumentTerm docTerm;
		decompressUnsignedInt(inputInvertedFile, &delta,&bitCode,&currentBit);
		realNumber += delta;
		docTerm.documentIndex = realNumber;
		decompressUnsignedInt(inputInvertedFile, &(docTerm.ftd), &bitCode, &currentBit);
		myPostingList[i] = docTerm;
	}
	*postingList = myPostingList;
}

int & GammaCompressor::getCompressorId()
{
	return compressorId;
}
