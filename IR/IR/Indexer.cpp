#include "stdafx.h"
#include "Indexer.h"
#include "Index.h"
#include "Dictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
Indexer::Indexer(string repositoryPath)
{
	Indexer::repositoryPath = repositoryPath;
}

IIndex* Indexer::createIndex()
{
	int dictionarySize = 1021; //1021 is a prime number used for this indexer
	IHasher* hasher = new Hasher();
	IDictionary* dictionary = new Dictionary(dictionarySize, hasher);

	Index* index = new Index(dictionary);
	/*
	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);

	Document* document;
	while ((document = documentProvider->getNextDocument()) != nullptr)
	{
		ITokenizer* tokenizer = new Tokenizer(*document);
		string token;
		while ((token = tokenizer->getNextToken()).compare("") != 0)
		{
			index->addTerm(token);
		}
		index->addDocument(document->getDocumentMetaData());
	}
	index->finalize();
	*/
	return index;
}

Indexer::~Indexer()
{
	//[Aymen] i don't know what we should do in this destructor
}
