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
	iDictionary = nullptr;
	outputFilePath = "";
}

IIndex* Indexer::createIndex()
{
	if (iDictionary == nullptr) {
		int dictionarySize = 10009; //1021 is a prime number used for this indexer
		iDictionary = new Dictionary(dictionarySize, new Hasher());
	}
	if (outputFilePath.compare("")==0) {
		outputFilePath = DEFAULT_OUTPUT_FILE;
	}
	Index* index = new Index(iDictionary, outputFilePath);
	
	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);

	Document* document;
	while ((document = documentProvider->getNextDocument()) != nullptr)
	{
		ITokenizer* tokenizer = new Tokenizer(document);
		string token;
		while ((token = tokenizer->getNextToken()).compare("") != 0)
		{
			index->addTerm(token);
		}
		index->addDocument(document->getDocumentMetaData());
		delete document;
		delete tokenizer;
	}
	delete documentProvider;
	index->finalize();
	
	return index;
}

IIndexer * Indexer::setIDictionary(IDictionary * iDictionary)
{
	Indexer::iDictionary = iDictionary;
	return this;
}

IIndexer * Indexer::setOutputFilePath(string outputFilePath)
{
	Indexer::outputFilePath = outputFilePath;
	return this;
}

Indexer::~Indexer()
{
	//[Aymen] i don't know what we should do in this destructor
}
