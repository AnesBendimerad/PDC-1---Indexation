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
	
	Index *index = new Index(new Dictionary(23,new Hasher()),"myFileOfPostingList");
	DocumentMetaData dMD1, dMD2, dMD3;
	dMD1.address = 1;
	dMD1.id = 1;
	dMD1.offset = 1;
	dMD2.address = 2;
	dMD2.id = 2;
	dMD2.offset = 2;
	dMD3.address = 3;
	dMD3.id = 3;
	dMD3.offset = 3;
	index->addTerm("anes");
	index->addTerm("miloud");
	index->addTerm("anes");
	index->addTerm("moh");
	index->addDocument(dMD1);
	index->addTerm("nta");
	index->addTerm("anes");
	index->addTerm("miloud");
	index->addDocument(dMD2);
	index->addTerm("hoho");
	index->addTerm("hahi");
	index->addTerm("anes");
	index->addTerm("miloud");
	index->addDocument(dMD3);
	index->finalize();
	DocumentTerm *docTermTable = index->getTermPostingList("anes");

	for (int i = 0; i < 3; i++) {
		DocumentTerm dc = docTermTable[i];
		cout << "heh :" + docTermTable[i].documentIndex << endl;
		cout << "hoh :" + docTermTable[i].ftd << endl;
	}
	return 0;
}

