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
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	IIndexBuilder* indexBuilder= new sortBasedIndexBuilder("C:\\Users\\LENOVO\\Desktop\\Etudes\\5IF - PDC\\PDC 1\\Exemple Data",5, 100);
	indexBuilder->createIndex();
	int a;
	cin >> a;
	return 0;
}
