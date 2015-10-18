#include "IndexLoader.h"

IndexLoader::IndexLoader(string postingFilePath)
{
	IndexLoader::postingFilePath = postingFilePath;
}

IndexLoader * IndexLoader::setDictionary(IDictionary * dictionary)
{
	return this;
}

IndexLoader * IndexLoader::setIndexType(int type)
{
	return this;
}

IIndex  * IndexLoader::load()
{
	return nullptr;
}

IndexLoader::~IndexLoader()
{
}
