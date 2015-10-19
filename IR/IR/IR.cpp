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
#include "InMemoryIndexBuilder.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	InMemoryIndexBuilder * indexer = new InMemoryIndexBuilder("D:\\repo");
	IIndex  * index = indexer->createIndex();
	//IndexLoader * indexLoader = new IndexLoader(DEFAULT_OUTPUT_FILE);
	//IIndex * index = indexLoader->load();
	
	vector<pair<DocumentMetaData, double>> topK = index->search(5, "maha hassan");

	for (int i = 0; i < topK.size(); i++)
	{
		cout << "id:" << topK.at(i).first.id << endl;
		cout << "wordsNumber:" << topK.at(i).first.wordsNumber << endl;
		cout << "offset:" << topK.at(i).first.offset << endl;
		cout << "adress:" << topK.at(i).first.address << endl;
		cout << "rank:" << topK.at(i).second << endl;
		cout << endl;
	}
	
	return 0;
}
