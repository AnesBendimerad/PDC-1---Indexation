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
#include "InMemoryIndexBuilder.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
#include "GammaCompressor.h"
#include "VByteCompressor.h"
#include "InMemoryIndexBuilder.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	IIndexBuilder* indexBuilder= new sortBasedIndexBuilder("C:\\myFiles\\tests",5, 100);
	//IIndexBuilder* indexBuilder = new InMemoryIndexBuilder("C:\\myFiles\\tests");
	IIndex *index1= indexBuilder->createIndex();
	//IndexLoader* indexLoader = new IndexLoader(DEFAULT_OUTPUT_FILE);
	//IIndex *index2 = indexLoader->load();
	int a=1;
	return 0;
}
