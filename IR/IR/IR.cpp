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
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	
	/*DocumentTable* docTable = new DocumentTable();*/
	DocumentMetaData d1, d2;
	d1.id = 11;
	d1.address = 111;
	d1.offset = 1111;
	d1.wordsNumber = 1;
	d2.id = 22;
	d2.address = 222;
	d2.offset = 2222;
	d2.wordsNumber = 2;
	/*int i1 = docTable->addDocument(d1);
	int i2 = docTable->addDocument(d2);*/
	IDictionary * dictionary = new Dictionary(1021, new Hasher());
	Index * index = new Index(dictionary);
	index->addTerm("miloud");
	index->addTerm("milouda");
	index->addTerm("miloud");
	index->addDocument(d1);
	index->addTerm("miloud");
	index->addDocument(d2);
	
	Term *d11 = dictionary->getTerm("miloud");
	Term *d22 = dictionary->getTerm("milouda");
	index->finalize();
	
	cout << "kemmel" << endl;
	return 0;
}

