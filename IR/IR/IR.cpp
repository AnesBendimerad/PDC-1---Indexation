#include "stdafx.h"
#include <iostream>
#include <string>
#include "Hasher.h"
#include <list>
#include <typeinfo>
#include "DocumentTable.h"
#include "HashTableDictionary.h"
#include "IIndex.h"
#include "Index.h"
#include "sortBasedIndexBuilder.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
#include "VByteCompressor.h"
#include "InMemoryIndexBuilder.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	//IIndexBuilder* indexBuilder= new sortBasedIndexBuilder("C:\\Users\\LENOVO\\Desktop\\Etudes\\5IF - PDC\\PDC 1\\Exemple Data",5, 100);
	//indexBuilder->createIndex();
	//int a;
	//cin >> a;

	//ICompressor *compressor = new VByteCompressor();
	//InMemoryIndexBuilder * indexer = new InMemoryIndexBuilder("C:\\myFiles\\tests");
	//indexer->setICompressor(compressor);
	//IIndex  * index = indexer->createIndex();

	IndexLoader * indexLoader = new IndexLoader(DEFAULT_OUTPUT_FILE);
	IIndex * index = indexLoader->load();

	return 0;
}
