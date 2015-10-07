#pragma once
#include <string>
#include "IIndexer.h"
using namespace std;
class Indexer : public IIndexer {
private:
	string repositoryPath;
public:
	Indexer(string repositoryPath);
	IIndex* createIndex();
	~Indexer();
};