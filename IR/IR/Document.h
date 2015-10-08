#pragma once
#include <string>
#include "DocumentMetaData.h"

using namespace std;

class Document {

private:
	DocumentMetaData documentMetaData;
	string text;

public:
	Document(DocumentMetaData docMetaData, string text);
	void incrementWordsNumber();
	string getText();
	DocumentMetaData getDocumentMetaData();
};