#pragma once
#include "IDictionary.h"
#include "IIndex.h"
#define SIMPLE_INDEX_TYPE 1
class IndexLoader {
private:
	IDictionary *dictionary;
	string postingFilePath;
	class indexType;
public:
	IndexLoader(string postingFilePath);
	IndexLoader * setDictionary(IDictionary * dictionary);
	IndexLoader * setIndexType(int type);
	IIndex * load();
	~IndexLoader();
};