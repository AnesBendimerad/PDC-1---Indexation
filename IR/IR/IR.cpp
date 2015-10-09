// IR.cpp : Defines the entry point for the console application.
//

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
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	/*string path = "C:\\Users\\LENOVO\\Desktop\\Etudes\\5IF - PDC\\PDC 1\\Exemple Data";
	IIndexer* indexer = new Indexer(path);
	IIndex* index = indexer->createIndex();*/
	Dictionary *dictionary = new Dictionary(1021, new Hasher());
	return 0;
}

