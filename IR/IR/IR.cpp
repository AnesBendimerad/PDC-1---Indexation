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
	ofstream outputFile("example");
	Term t1,t2;
	t1.documentNumber = 1;
	t1.token = "ddoeoeoeozzpzpzpzpzpeoeoeoeorororororo";
	t1.totalFrequency = 1;
	t2.documentNumber = 2;
	t2.token = "niyahahaha";
	t2.totalFrequency = 4;
	outputFile.write((const char *)&t1, sizeof(Term));
	outputFile.write((const char *)&t2, sizeof(Term));
	outputFile.close();
	ifstream inputStream("example");
	Term * cTerm1 = (Term*)malloc(sizeof(Term));
	Term * cTerm2 = (Term*)malloc(sizeof(Term));
	inputStream.read((char *)cTerm1, sizeof(Term));
	inputStream.read((char *)cTerm2, sizeof(Term));
	cout << "the end" << endl;
	
	return 0;
}

