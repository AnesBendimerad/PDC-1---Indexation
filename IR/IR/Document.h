#pragma once
#include <string>
#include "DocumentMetaData.h"
using namespace std;

class Document {

private:
	DocumentMetaData documentMetaData;
	string text;

public:
	void incrementWordsNumber();
	string getText();
	DocumentMetaData getDocumentMetaData();

};