#include "stdafx.h"
#include <iostream>
#include <string>
#include "Hasher.h"
#include <list>
#include <typeinfo>
#include "DocumentTable.h"
#include "Dictionary.h"
#include "IIndex.h"
#include "Index.h"
#include "Indexer.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	//Indexer * indexer = new Indexer("C:\\myFiles\\tests");
	//IIndex  * index = indexer->createIndex();
	IndexLoader * indexLoader = new IndexLoader(DEFAULT_OUTPUT_FILE);
	IIndex * index = indexLoader->load();
	
	return 0;
}
