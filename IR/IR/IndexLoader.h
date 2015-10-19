#pragma once
#include "IDictionary.h"
#include "IIndex.h"
class IndexLoader {
private:
	IDictionary *dictionary;
	string invertedFilePath;
	int indexType;
public:
	IndexLoader(string invertedFilePath);
	IndexLoader * setDictionary(IDictionary * dictionary);
	IndexLoader * setIndexType(int indexType);
	IIndex * load();
	~IndexLoader();
};