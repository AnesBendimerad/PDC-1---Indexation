#include "stdafx.h"
#include "SimpleIndex.h"
#include <fstream>
SimpleIndex::SimpleIndex(IDictionary * dictionary, DocumentTable * documentTable, string postingFilePath)
{
	SimpleIndex::dictionary = dictionary;
	SimpleIndex::documentTable = documentTable;
	SimpleIndex::postingFilePath = postingFilePath;
}

list<int> SimpleIndex::search(string querry)
{
	return list<int>();
}

DocumentTerm * SimpleIndex::getTermPostingList(string token)
{
	ifstream inputStream(postingFilePath);
	Term * term = dictionary->getTerm(token);
	if (term != nullptr) {
		DocumentTerm* documentTermTable = (DocumentTerm*)malloc(sizeof(DocumentTerm)*term->documentNumber);
		inputStream.seekg((unsigned int)term->postingList);
		inputStream.read((char *)documentTermTable, sizeof(DocumentTerm)*term->documentNumber);
		inputStream.close();
		return documentTermTable;
	}
	inputStream.close();
	return nullptr;
}
SimpleIndex::~SimpleIndex()
{
	delete dictionary;
	delete documentTable;
}