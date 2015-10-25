#include "stdafx.h"
#include <iostream>
#include <string>
#include "Hasher.h"
#include <list>
#include <typeinfo>
#include "DocumentTable.h"
#include "HashTableDictionary.h"
#include "IIndex.h"
#include "StrTkTokenizer.h"
#include "Index.h"
#include "sortBasedIndexBuilder.h"
#include "InMemoryIndexBuilder.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
#include "GammaCompressor.h"
#include "VByteCompressor.h"
#include "InMemoryIndexBuilder.h"
#include "MemoryManager.h"
#include "NoCompressor.h"
#include "VByteCompressor.h"
#include "GammaCompressor.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	IIndex *index;
	vector<pair<DocumentMetaData, double>> searchResult;

	//Index Builder case :
	string repositoryPath;
	string indexPath;
	unsigned int indiceTokenizer = 0;
	unsigned int indiceBuilder = 1;
	IIndexBuilder *indexBuilder;
	unsigned int indiceCompressor = 0;
	ICompressor *compressor;
	unsigned int indiceSearch = 1;


	cout << "write the path of the corpus to index : ";
	cin >> repositoryPath;
	cout << endl;

	cout << "write the path of the serialized index (inverted file) : ";
	cin >> indexPath;
	cout << endl;

	cout << "select the tokenizer : " << endl;
	cout << "\t 0 : for simple tokenizer" << endl;
	cout << "\t 1 : for strtk based tokenizer" << endl;
	cout << "choice : ";
	cin >> indiceTokenizer;
	cout << endl;

	cout << "select the indexation method : " << endl;
	cout << "\t 0 : for In memory inversion" << endl;
	cout << "\t 1 : for sort based inversion" << endl;
	cout << "choice : ";
	cin >> indiceBuilder;
	cout << endl;

	cout << "select the compression method : " << endl;
	cout << "\t 0 : No compression" << endl;
	cout << "\t 1 : VByte compression" << endl;
	cout << "\t 2 : Gamma compression" << endl;
	cout << "choice : ";
	cin >> indiceCompressor;
	cout << endl;

	cout << "select the search method : " << endl;
	cout << "\t 0 : Fagin method" << endl;
	cout << "\t 1 : BM25 method" << endl;
	cout << "choice : ";
	cin >> indiceSearch;
	cout << endl;
	
	switch (indiceBuilder) {
	case 1:
		indexBuilder = new InMemoryIndexBuilder(repositoryPath);
		break;
	case 2:
		indexBuilder = new sortBasedIndexBuilder(repositoryPath);
		break;
	default :
		indexBuilder = new InMemoryIndexBuilder(repositoryPath);
		break;
	}
	
	IDictionary *dictionary = new HashTableDictionary();
	
	switch (indiceCompressor) {
	case NO_COMPRESSOR:
		compressor = new NoCompressor();
		break;
	case VBYTE_COMPRESSOR:
		compressor = new VByteCompressor();
		break;
	case GAMMA_COMPRESSOR:
		compressor = new GammaCompressor();
		break;
	default:
		compressor = new NoCompressor();
		break;
	}

	if (indiceTokenizer != SIMPLE_TOKENIZER && indiceTokenizer != STRTK_TOKENIZER)
	{
		indiceTokenizer = SIMPLE_TOKENIZER;
	}

	if (indiceSearch != FAGIN_INDEX_TYPE && indiceSearch != BM25_INDEX_TYPE)
	{
		indiceSearch = FAGIN_INDEX_TYPE;
	}

	cout << "Compressor ID : ";
	cout << compressor->getCompressorId() << endl;

	indexBuilder->setIDictionary(dictionary)->setOutputFilePath(indexPath)->setICompressor(compressor)->setITokenizer(indiceTokenizer)->setIndexType(indiceSearch);
	
	index = indexBuilder->createIndex();

	searchResult=index->search(3, "the");
	for (int i = 0;i < searchResult.size();i++)
	{
		cout << searchResult[i].first.id << endl;
	}
	return 0;
}
