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
#include <fstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	IDictionary *dictionary = new Dictionary(1021, new Hasher());
	dictionary->addTerm("ana");
	dictionary->addTerm("houwa");
	dictionary->addTerm("ana");
	dictionary->addTerm("nta");
	dictionary->addTerm("l3ayla");
	dictionary->addTerm("voila");
	IIterator *iterator = dictionary->getIterator();
	Term* term;
	while ((term = (Term*)iterator->getNext()) != nullptr) {
		cout << "term :" + term->token << endl;
	}

	return 0;
}

