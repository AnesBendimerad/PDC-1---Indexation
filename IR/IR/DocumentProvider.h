#pragma once
#include <stdio.h>
#include <list>
#include <string>
#include "IDocumentProvider.h"
using namespace std;
class DocumentProvider : IDocumentProvider{
private:
	FILE * currentFile;
	list<string> pathList;
	int currentFileIndex;
public:
	DocumentProvider(string repositoryFile);
	Document getNextDocument();
	~DocumentProvider();
};