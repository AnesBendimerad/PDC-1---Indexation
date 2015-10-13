#include "stdafx.h"
#include "Document.h"

Document::Document(DocumentMetaData docMetaData, string text)
{
	Document::documentMetaData = docMetaData;
	Document::text = text;
}

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
