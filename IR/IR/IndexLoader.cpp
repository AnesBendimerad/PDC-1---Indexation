#include "stdafx.h"
#include "IndexLoader.h"
#include <fstream>
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentTable.h"
#include "Index.h"
#include "NoCompressor.h"
#include "VByteCompressor.h"
#include "GammaCompressor.h"
#include "FileManager.h"
IndexLoader::IndexLoader(string invertedFilePath)
{
	IndexLoader::invertedFilePath = invertedFilePath;
	dictionary = nullptr;
}

IndexLoader * IndexLoader::setDictionary(IDictionary * dictionary)
{
	IndexLoader::dictionary = dictionary;
	return this;
}

Index  * IndexLoader::load()
{
	if (dictionary == nullptr) {
		dictionary= new HashTableDictionary();
	}
	ifstream *inputStream = FileManager::openIfstream(invertedFilePath);

	// get information about dictionary and documentTable
	unsigned int  dictionaryOffset;
	unsigned long long  termNumbers;
	int compressorId;
	unsigned int documentMetaDataNumber;

	inputStream->read((char *)&dictionaryOffset, sizeof(unsigned int));
	inputStream->read((char *)&termNumbers, sizeof(unsigned long long));
	inputStream->read((char *)&compressorId, sizeof(int));
	inputStream->read((char *)&documentMetaDataNumber, sizeof(unsigned int));
	
	ICompressor *iCompressor;
	switch (compressorId)
	{
	case NO_COMPRESSOR:
		iCompressor = new NoCompressor();
		break;
	case VBYTE_COMPRESSOR:
		iCompressor = new VByteCompressor();
		break;
	case GAMMA_COMPRESSOR:
		iCompressor = new GammaCompressor();
		break;
	default:
		throw runtime_error("No compressor with such ID");
		break;
	}
	// load the documentTable
	
	DocumentMetaData * docTable= (DocumentMetaData*)malloc(sizeof(DocumentMetaData)*(documentMetaDataNumber));
	inputStream->read((char *)docTable, sizeof(DocumentMetaData)*(documentMetaDataNumber));
	DocumentTable * documentTable = new DocumentTable(documentMetaDataNumber, docTable);

	// load the dictionary
	inputStream->seekg((unsigned int) dictionaryOffset);
	for (int i = 0; i < termNumbers; i++) {
		Term currentTerm;
		*inputStream >> &currentTerm;
		dictionary->addTerm(&currentTerm);
	}
	
	inputStream->close();
	delete inputStream;

	Index *myIndex=new Index(dictionary, documentTable, iCompressor, invertedFilePath);
	return myIndex;
}

IndexLoader::~IndexLoader()
{

}
