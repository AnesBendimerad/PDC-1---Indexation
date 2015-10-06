#include "stdafx.h"
#include "Document.h"

void Document::incrementWordsNumber() {
	documentMetaData.wordsNumber++;
}

string Document::getText()
{
	return text;
}

DocumentMetaData Document::getDocumentMetaData()
{
	return documentMetaData;
}
