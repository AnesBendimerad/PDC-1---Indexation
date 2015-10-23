#include "stdafx.h"
#include "IndexBM25.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;

IndexBM25::IndexBM25(IDictionary * dictionary, DocumentTable * documentTable, ICompressor * compressor, string invertedFilePath)
{
	IndexBM25::dictionary = dictionary;
	IndexBM25::documentTable = documentTable;
	IndexBM25::compressor = compressor;
	IndexBM25::invertedFilePath = invertedFilePath;
}

IndexBM25::~IndexBM25()
{
}


DocumentTerm * IndexBM25::getTermPostingList(string token)
{
	ifstream inputStream(invertedFilePath, ios::in | ios::binary);
	Term * term = dictionary->getTerm(token);
	if (term != nullptr) {
		DocumentTerm* documentTermTable;
		inputStream.seekg((unsigned int)term->postingList);
		compressor->readAndDecompress(&inputStream, &documentTermTable, term->documentNumber);
		inputStream.close();
		return documentTermTable;
	}
	inputStream.close();
	return nullptr;
}


vector<pair<DocumentMetaData, double>> IndexBM25::search(int topK, string query)
{
	vector<pair<DocumentMetaData, double>> topKDocuments;
	map<int, double> result;
	map<int, double>::iterator it;

	istringstream stream(query);
	string token;
	DocumentTerm *documentTermTable;
	Term *term;

	double avgdl = calculateAVGDL();
	double score;

	int tf, fd, nbwords, currentDocumentIndex;

	while (stream >> token)
	{
		documentTermTable = getTermPostingList(token);

		if (documentTermTable != nullptr)
		{
			term = dictionary->getTerm(token);

			for (int i = 0; i < term->documentNumber; i++)
			{
				tf = documentTermTable[i].ftd;
				currentDocumentIndex = documentTermTable[i].documentIndex;
				fd = term->documentNumber;
				nbwords = documentTable->getDocument(currentDocumentIndex)->wordsNumber;
				score = calculateScore(tf, fd, nbwords, avgdl);

				it = result.find(currentDocumentIndex);
				if (it != result.end())
				{
					it->second = it->second + score;
				}
				else
				{
					result.insert(pair<int, double>(currentDocumentIndex, score));

				}
			}
		}
	}

	for (auto element : result)
	{
		topKDocuments.push_back(pair<DocumentMetaData, double>(*documentTable->getDocument(element.first), element.second));
	}

	sort(topKDocuments.begin(), topKDocuments.end(), sort_by_score);

	if (topKDocuments.size() > topK)
		topKDocuments.resize(topK);

		return  topKDocuments;
}


//this function calculates the average document length over all documents of the collection
double IndexBM25::calculateAVGDL()
{
	int totalDocumentsLength = 0;
	DocumentMetaData *allDocuments = documentTable->getFinalizedDocumentTable();

	for (int i = 0; i < documentTable->getDocumentNumber(); i++)
	{
		totalDocumentsLength += allDocuments[i].wordsNumber;
	}

	return (double)totalDocumentsLength / (double)documentTable->getDocumentNumber();
}

//this function calculates the score using BM25 function
 double IndexBM25::calculateScore(int tf, int df,int nbwords,int avgdl)
{
	//free paramaters used by BM25 
	double k1 = 2.0;
	double b = 0.75;

	double score = 0;
	double idf = 0;
	double w = 0;
	int N = documentTable->getDocumentNumber();

	idf = log10((N - df + 0.5) / (df + 0.5));

	w = ((tf*(k1 + 1)) / (tf + (k1*(1 - b + (b*nbwords / avgdl)))));

	score = idf * w;
	
	return score;
}

 bool IndexBM25::sort_by_score(const pair<DocumentMetaData, double>& left, const pair<DocumentMetaData, double>& right)
 {
	 return left.second > right.second;
 }
