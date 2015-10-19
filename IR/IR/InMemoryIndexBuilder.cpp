#include "stdafx.h"
#include "InMemoryIndexBuilder.h"
#include "Index.h"
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
InMemoryIndexBuilder::InMemoryIndexBuilder(string repositoryPath)
{
	InMemoryIndexBuilder::repositoryPath = repositoryPath;
	iDictionary = nullptr;
	outputFilePath = "";
}

IIndex* InMemoryIndexBuilder::createIndex()
{
	if (iDictionary == nullptr) {
		iDictionary = new HashTableDictionary();
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

IIndexBuilder * InMemoryIndexBuilder::setIDictionary(IDictionary * iDictionary)
{
	InMemoryIndexBuilder::iDictionary = iDictionary;
	return this;
}

IIndexBuilder * InMemoryIndexBuilder::setOutputFilePath(string outputFilePath)
{
	InMemoryIndexBuilder::outputFilePath = outputFilePath;
	return this;
}

InMemoryIndexBuilder::~InMemoryIndexBuilder()
{
	//[Aymen] i don't know what we should do in this destructor
}
