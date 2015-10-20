#include "sortBasedIndexBuilder.h"
#include "Index.h"
#include "HashTableDictionary.h"
#include "Hasher.h"
#include "DocumentProvider.h"
#include "Tokenizer.h"
#include "NoCompressor.h"

sortBasedIndexBuilder::sortBasedIndexBuilder(string repositoryPath, unsigned int numberOfBlock, unsigned int numberOfTripletInBlock)
{
	sortBasedIndexBuilder::repositoryPath = repositoryPath;
	sortBasedIndexBuilder::numberOfBlock = numberOfBlock;
	sortBasedIndexBuilder::numberOfTripletInBlock = numberOfTripletInBlock;
	iDictionary = nullptr;
	iCompressor = nullptr;
	outputFilePath = "";
	indexType = FAGIN_INDEX_TYPE;
}

IIndexBuilder * sortBasedIndexBuilder::setIDictionary(IDictionary * iDictionary)
{
	sortBasedIndexBuilder::iDictionary = iDictionary;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setICompressor(ICompressor * iCompressor)
{
	sortBasedIndexBuilder::iCompressor = iCompressor;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setOutputFilePath(string outputFilePath)
{
	sortBasedIndexBuilder::outputFilePath = outputFilePath;
	return this;
}

IIndexBuilder * sortBasedIndexBuilder::setIndexType(int indexType)
{
	sortBasedIndexBuilder::indexType = indexType;
	return this;
}

IIndex * sortBasedIndexBuilder::createIndex()
{
	if (iDictionary == nullptr) {
		iDictionary = new HashTableDictionary();
	}
	
	if (iCompressor == nullptr) {
		iCompressor = new NoCompressor();
	}
	
	DocumentTable * documentTable = new DocumentTable();

	if (outputFilePath.compare("") == 0) {
		outputFilePath = DEFAULT_OUTPUT_FILE;
	}

	IDocumentProvider* documentProvider = new DocumentProvider(repositoryPath);
	
	Document* document;
	while ((document = documentProvider->getNextDocument()) != nullptr)
	{
		ITokenizer* tokenizer = new Tokenizer(document);
		string token;
		while ((token = tokenizer->getNextToken()).compare("") != 0)
		{
			// Add term 
		}
		documentTable->addDocument(document->getDocumentMetaData());
		delete document;
		delete tokenizer;
	}
	
	delete documentProvider;
	IIndex *index = nullptr;
	if (indexType == FAGIN_INDEX_TYPE) {
		index = new Index(iDictionary, documentTable, iCompressor, outputFilePath);
	}
	return index;
}

sortBasedIndexBuilder::~sortBasedIndexBuilder()
{
	//[Aymen] i don't know what we should do in this destructor
}
