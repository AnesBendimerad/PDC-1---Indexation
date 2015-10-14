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
#include "DocumentTerm.h"
#include <fstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	//Indexer * indexer = new Indexer("C:\\myFiles\\tests");
	//IIndex  * index = indexer->createIndex();
	Term t;
	t.documentNumber = 1;
	t.token = "";
	t.totalFrequency = 1;
	int size = sizeof(t.token);
	cout << "the end" << endl;
	
	return 0;
}

