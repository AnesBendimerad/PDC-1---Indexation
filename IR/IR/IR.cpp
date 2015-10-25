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
	
	//Index Builder case :
	string repositoryPath;
	string indexPath;
	unsigned int indiceTokenizer = 0;
	unsigned int indiceBuilder = 0;
	IIndexBuilder *indexBuilder;
	unsigned int indiceCompressor = 0;
	ICompressor *compressor = 0;

	cout << "write the path of the corpus to index : ";
	cin >> repositoryPath;
	cout << endl;

	cout << "write the path of the serialized index (inverted file) : ";
	cin >> indexPath;
	cout << endl;

	cout << "select the tokenizer : " << endl;
	cout << "\t 1 : for simple tokenizer" << endl;
	cout << "\t 2 : for strtk based tokenizer" << endl;
	cout << "choice : ";
	cin >> indiceTokenizer;
	cout << endl;

	cout << "select the indexation method : " << endl;
	cout << "\t 1 : for In memory inversion" << endl;
	cout << "\t 2 : for sort based inversion" << endl;
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

	switch (indiceBuilder) {
	case 0:
		compressor = new NoCompressor();
		break;
	case 1:
		compressor = new VByteCompressor();
		break;
	case 2:
		compressor = new GammaCompressor();
		break;
	default:
		compressor = new NoCompressor();
		break;
	}
	
	indexBuilder->setIDictionary(dictionary)->setOutputFilePath(indexPath)->setICompressor(compressor);
	
	return 0;
}
