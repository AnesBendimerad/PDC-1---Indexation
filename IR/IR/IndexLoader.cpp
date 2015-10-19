#include "stdafx.h"
#include "IndexLoader.h"
#include <fstream>
#include "Dictionary.h"
#include "Hasher.h"
#include "DocumentTable.h"
#include "SimpleIndex.h"

IndexLoader::IndexLoader(string postingFilePath)
{
	IndexLoader::postingFilePath = postingFilePath;
	dictionary = nullptr;
	indexType = SIMPLE_INDEX_TYPE;
}

IndexLoader * IndexLoader::setDictionary(IDictionary * dictionary)
{
	IndexLoader::dictionary = dictionary;
	return this;
}

IndexLoader * IndexLoader::setIndexType(int indexType)
{
	IndexLoader::indexType = indexType;
	return this;
}

IIndex  * IndexLoader::load()
{
	if (dictionary == nullptr) {
		int dictionarySize = 10009; 
		dictionary= new Dictionary(dictionarySize, new Hasher());
	}
	ifstream inputStream(postingFilePath, ios::in | ios::binary);

	// get information about dictionary and documentTable
	unsigned int * dictionaryOffset= (unsigned int *)malloc(sizeof(unsigned int));
	unsigned long long * termNumbers = (unsigned long long *)malloc(sizeof(unsigned long long));
	unsigned long long * documentMetaDataNumber = (unsigned long long *)malloc(sizeof(unsigned long long));
	inputStream.read((char *)dictionaryOffset, sizeof(unsigned int));
	inputStream.read((char *)termNumbers, sizeof(unsigned long long));
	inputStream.read((char *)documentMetaDataNumber, sizeof(unsigned long long));

	// load the documentTable
	DocumentMetaData * docTable= (DocumentMetaData*)malloc(sizeof(DocumentMetaData)*(*documentMetaDataNumber));
	inputStream.read((char *)docTable, sizeof(DocumentMetaData)*(*documentMetaDataNumber));
	DocumentTable * documentTable = new DocumentTable(*documentMetaDataNumber, docTable);

	// load the dictionary
	inputStream.seekg((unsigned int) *dictionaryOffset);
	for (int i = 0; i < *termNumbers; i++) {
		Term currentTerm;
		inputStream >> &currentTerm;
		dictionary->addTerm(&currentTerm);
	}
	IIndex *myIndex=nullptr;
	if (indexType == SIMPLE_INDEX_TYPE) {
		myIndex = new SimpleIndex(dictionary, documentTable, postingFilePath);;
	}
	return myIndex;
}

IndexLoader::~IndexLoader()
{

}
