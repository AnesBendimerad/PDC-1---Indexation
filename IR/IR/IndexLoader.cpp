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
IndexLoader::IndexLoader(string invertedFilePath)
{
	IndexLoader::invertedFilePath = invertedFilePath;
	dictionary = nullptr;
	indexType = FAGIN_INDEX_TYPE;
}

IndexLoader * IndexLoader::setDictionary(IDictionary * dictionary)
{
	IndexLoader::dictionary = dictionary;
	return this;
}

IndexLoader * IndexLoader::setIndexType(int indexType)
{
	if (indexType != FAGIN_INDEX_TYPE) {
		throw runtime_error("No Index with such ID");
	}
	IndexLoader::indexType = indexType;
	return this;
}

IIndex  * IndexLoader::load()
{
	if (dictionary == nullptr) {
		dictionary= new HashTableDictionary();
	}
	ifstream inputStream(invertedFilePath, ios::in | ios::binary);

	// get information about dictionary and documentTable
	unsigned int  dictionaryOffset;
	unsigned long long  termNumbers;
	int compressorId;
	unsigned long long  documentMetaDataNumber;

	inputStream.read((char *)&dictionaryOffset, sizeof(unsigned int));
	inputStream.read((char *)&termNumbers, sizeof(unsigned long long));
	inputStream.read((char *)&compressorId, sizeof(int));
	inputStream.read((char *)&documentMetaDataNumber, sizeof(unsigned long long));
	
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
	inputStream.read((char *)docTable, sizeof(DocumentMetaData)*(documentMetaDataNumber));
	DocumentTable * documentTable = new DocumentTable(documentMetaDataNumber, docTable);

	// load the dictionary
	inputStream.seekg((unsigned int) dictionaryOffset);
	for (int i = 0; i < termNumbers; i++) {
		Term currentTerm;
		inputStream >> &currentTerm;
		dictionary->addTerm(&currentTerm);
	}
	
	
	IIndex *myIndex=nullptr;
	switch (indexType) {
	case FAGIN_INDEX_TYPE:
			myIndex = new Index(dictionary, documentTable, iCompressor, invertedFilePath);
			break;
	}
	return myIndex;
}

IndexLoader::~IndexLoader()
{

}
