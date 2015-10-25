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
#include "MemoryManager.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{

	//unsigned int b = MemoryManager::getDiskSectorSize();
	//cout << b;
	
	
	IDictionary *dictionary = new HashTableDictionary();
	IIndexBuilder* indexBuilder= new sortBasedIndexBuilder("C:\\Users\\LENOVO\\Desktop\\Etudes\\5IF - PDC\\PDC 1\\Exemple Data",5, 100);
	indexBuilder->setIDictionary(dictionary);
	IIndex *index1= indexBuilder->createIndex();
	
	dictionary->writeCSVFile();
	//cout << MemoryManager::getWorkingSetSize() << endl;
	
	int a;
	cin >> a;
	return 0;
}
