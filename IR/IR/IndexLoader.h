#pragma once
#include "IDictionary.h"
#include "Index.h"
class IndexLoader {
private:
	IDictionary *dictionary;
	string invertedFilePath;
public:
	IndexLoader(string invertedFilePath);
	IndexLoader * setDictionary(IDictionary * dictionary);
	Index * load();
	~IndexLoader();
};