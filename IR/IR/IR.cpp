#include "stdafx.h"
#include <iostream>
#include <string>
#include "Hasher.h"
#include <list>
#include <typeinfo>
#include "DocumentTable.h"
#include "HashTableDictionary.h"
#include "Index.h"
#include "StrTkTokenizer.h"
#include "Index.h"
#include "SortBasedIndexBuilder.h"
#include "InMemoryIndexBuilder.h"
#include "DocumentTerm.h"
#include "IndexLoader.h"
#include <fstream>
#include "GammaCompressor.h"
#include "VByteCompressor.h"
#include "InMemoryIndexBuilder.h"
#include "MemoryManager.h"
#include "NoCompressor.h"
#include "VByteCompressor.h"
#include "GammaCompressor.h"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	Index *index = NULL;
	bool end = false;

	string repositoryPath;
	string indexPath;
	unsigned int indiceTokenizer = 0;
	unsigned int indiceBuilder = 1;
	unsigned int indiceCompressor = 0;
	unsigned int indiceSearch = 1;
	unsigned int topk;
	IIndexBuilder *indexBuilder;
	ICompressor *compressor;
	IDictionary *dictionary;
	IndexLoader *indexLoader;
	string invertedFilePath;
	string query;
	vector<pair<DocumentMetaData, double>> searchResult;

	while (!end)
	{
		unsigned int choice;
		std::cout << "----------------------------------------" << endl;
		std::cout << "What would you like to do ? " << endl;
		std::cout << "\t 0 : Build an index " << endl;
		std::cout << "\t 1 : load an index " << endl;
		std::cout << "\t 2 : search in a created index " << endl;
		std::cout << "\t 3 : quit " << endl;
		std::cout << "choice : ";
		std::cin >> choice;
		std::cout << endl;
		std::cout << "----------------------------------------" << endl;
		std::cin.clear();
		try
		{
			switch (choice)
			{
			case 0:
				//Index Builder case :

				std::cout << "Index Build :" << endl;
				std::cout << "----------------------------------------" << endl;

				std::cout << "write the path of the corpus to index : ";
				while (std::getline(std::cin, repositoryPath)) {
					if (repositoryPath == "") continue;
					else break;
				}
				std::cout << endl;
				std::cin.clear();

				std::cout << "write the path of the serialized index (inverted file) : ";
				while (std::getline(std::cin, indexPath)) {
					if (indexPath == "") continue;
					else break;
				}
				std::cout << endl;
				std::cin.clear();

				std::cout << "select the indexation method : " << endl;
				std::cout << "\t 0 : for In memory inversion" << endl;
				std::cout << "\t 1 : for sort based inversion" << endl;
				std::cout << "choice : ";
				std::cin >> indiceBuilder;
				std::cout << endl;
				std::cin.clear();

				std::cout << "select the tokenizer : " << endl;
				std::cout << "\t 0 : for simple tokenizer" << endl;
				std::cout << "\t 1 : for strtk based tokenizer" << endl;
				std::cout << "choice : ";
				std::cin >> indiceTokenizer;
				std::cout << endl;
				std::cin.clear();

				std::cout << "select the compression method : " << endl;
				std::cout << "\t 0 : No compression" << endl;
				std::cout << "\t 1 : VByte compression" << endl;
				std::cout << "\t 2 : Gamma compression" << endl;
				std::cout << "choice : ";
				std::cin >> indiceCompressor;
				std::cout << endl;
				std::cin.clear();

				switch (indiceBuilder) {
				case 0:
					indexBuilder = new InMemoryIndexBuilder(repositoryPath);
					break;
				case 1:
					indexBuilder = new SortBasedIndexBuilder(repositoryPath);
					break;
				default:
					indexBuilder = new InMemoryIndexBuilder(repositoryPath);
					break;
				}

				dictionary = new HashTableDictionary();

				switch (indiceCompressor) {
				case NO_COMPRESSOR:
					compressor = new NoCompressor();
					break;
				case VBYTE_COMPRESSOR:
					compressor = new VByteCompressor();
					break;
				case GAMMA_COMPRESSOR:
					compressor = new GammaCompressor();
					break;
				default:
					compressor = new NoCompressor();
					break;
				}

				if (indiceTokenizer != SIMPLE_TOKENIZER && indiceTokenizer != STRTK_TOKENIZER)
				{
					indiceTokenizer = SIMPLE_TOKENIZER;
				}

				std::cout << "Constructing the index ..." << endl;
				indexBuilder->setIDictionary(dictionary)->setOutputFilePath(indexPath)->setICompressor(compressor)->setITokenizer(indiceTokenizer);
				index = indexBuilder->createIndex();
				//dictionary->writeCSVFile();
				std::cout << "Index successfully created !" << endl;
				std::cout << "----------------------------------------" << endl;
				break;
			case 1:
				//Index load case :
				std::cout << "Index Load :" << endl;
				std::cout << "----------------------------------------" << endl;

				std::cout << "write the path of the serialized index (inverted file) : ";
				while (std::getline(std::cin, invertedFilePath)) {
					if (invertedFilePath == "") continue;
					else break;
				}
				std::cout << endl;
				std::cin.clear();

				std::cout << "Loading the index from " << invertedFilePath << " ..." << endl;
				indexLoader = new IndexLoader(invertedFilePath);
				index = indexLoader->load();
				std::cout << "Index successfully loaded !" << endl;
				std::cout << "----------------------------------------" << endl;
				break;
			case 2:
				// Index search
				std::cout << "search :" << endl;
				std::cout << "----------------------------------------" << endl;

				std::cout << "select the search method : " << endl;
				std::cout << "\t 0 : Fagin method" << endl;
				std::cout << "\t 1 : BM25 method" << endl;
				std::cout << "choice : ";
				std::cin >> indiceSearch;
				std::cout << endl;
				std::cin.clear();

				std::cout << "select the number of result (between 1 and 1000) : ";
				std::cin >> topk;
				std::cout << endl;
				std::cin.clear();
				if (topk < 1)
				{
					topk = 1;
				}
				else if (topk > 1000)
				{
					topk = 1000;
				}

				std::cout << "write the query (Bag of word) : " << endl;
				while (std::getline(std::cin, query)) {
					if (query == "") continue;
					else break;
				}
				std::cout << endl;
				std::cin.clear();


				if (index == NULL)
				{
					std::cout << "No index loaded, please build or load an index before" << endl;
				}
				else
				{
					switch (indiceSearch) 
					{
					case 0 : 
						std::cout << "Processing the query (Fagin) : '" << query << "' ..." << endl;
						searchResult = index->searchFagin(topk, query);
						break;
					case 1 :
						std::cout << "Processing the query (BM25) : '" << query << "' ..." << endl;
						searchResult = index->searchBM25(topk, query);
						break;
					default :
						std::cout << "Processing the query (BM25) : '" << query << "' ..." << endl;
						searchResult = index->searchBM25(topk, query);
						break;
					}
					for (unsigned int i = 0;i < searchResult.size();i++)
					{
						unsigned int j = i + 1;
						unsigned long documentNumber = searchResult[i].first.address%10000;
						unsigned long documentDate = searchResult[i].first.address/10000;
						std::cout << j << ": document " << searchResult[i].first.id << " (document " << documentNumber;
						std::printf(" in file la%06d", documentDate);
					    std:cout << ") : Score = " << searchResult[i].second << endl;
					}
				}
				std::cout << "----------------------------------------" << endl;
				break;
			case 3:
				end = true;
				break;
			default:
				end = true;
				break;
			}
		}
		catch (const runtime_error& error)
		{
			cout << "----------------------------------------" << endl;
			cout << "Error : " << error.what() << endl;
			cout << "----------------------------------------" << endl;
		}
	}
	return 0;
}
