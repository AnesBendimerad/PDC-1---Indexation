#pragma once
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "IDocumentProvider.h"
using namespace std;
class DocumentProvider : public IDocumentProvider {
private:
	ifstream  currentFile;
	vector<string> pathList;
	unsigned int currentFileIndex = 0;
	string transformToken(string&);
public:
	DocumentProvider(string repositoryFile);
	Document* getNextDocument();
	~DocumentProvider();
};