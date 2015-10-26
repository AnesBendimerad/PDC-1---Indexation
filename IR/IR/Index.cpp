#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath> 
#include <algorithm>
#include "Index.h"
#include "IDictionary.h"
#include "DocumentTable.h"
#include "DocumentTerm.h"
#include "Term.h"
#include "FileManager.h"
using namespace std;


Index::Index(IDictionary * dictionary, DocumentTable * documentTable, ICompressor * compressor, string invertedFilePath)
{
	Index::dictionary = dictionary;
	Index::documentTable = documentTable;
	Index::compressor = compressor;
	Index::invertedFilePath = invertedFilePath;
}

DocumentTerm * Index::getTermPostingList(string token)
{
	ifstream *inputStream = FileManager::openIfstream(invertedFilePath);
	Term * term = dictionary->getTerm(token);
	if (term != nullptr) {
		DocumentTerm* documentTermTable;
		inputStream->seekg((unsigned int) term->postingList);
		compressor->readAndDecompress(inputStream, &documentTermTable, term->documentNumber);
		inputStream->close();
		return documentTermTable;
	}
	inputStream->close();
	return nullptr;
}




// This function implements fagin's algorithm and return the topK documents which matchs the query
vector<pair<DocumentMetaData, double>> Index::search(int topK, string query)
{
	vector<vector<pair<int, double>>> list_sorted_by_tf_idf = calculateTF_IDF(query);
	vector<pair<int, double>> topKDocuments;
	vector<pair<DocumentMetaData, double>>  topDocuments;

	double thresHold;
	double weight=0, globalWeight =0;
	int documentIndex;
	int result;

	do
	{
		thresHold = 0;
		for (unsigned int i = 0; i < list_sorted_by_tf_idf.size(); i++)
		{
			if (list_sorted_by_tf_idf.at(i).size() != 0)
			{
				//Retrieve the document d(i) containing term ti that has the next largest tf-idf then remove it
				documentIndex = list_sorted_by_tf_idf.at(i).begin()->first;
				weight = list_sorted_by_tf_idf.at(i).begin()->second;
				list_sorted_by_tf_idf.at(i).erase(list_sorted_by_tf_idf.at(i).begin());
				//Compute its global score by retrieving all s(tj, d(i)) with j!=i
				for (unsigned int j = 0; j < list_sorted_by_tf_idf.size(); j++)
				{
					if (i != j)
					{
						if ((result = findPositionOfDocument(documentIndex, list_sorted_by_tf_idf.at(j))) != -1)
						{
							globalWeight += list_sorted_by_tf_idf.at(j).at(result).second;
							list_sorted_by_tf_idf.at(j).erase(list_sorted_by_tf_idf.at(j).begin() + result);
						}
					}
				}

				globalWeight += weight;
				thresHold += weight;

				//If R contains less than k documents, add d(i) to the result
				if (topKDocuments.size() < topK && documentIndex != -1)
				{
					topKDocuments.push_back(pair<int, double>(documentIndex, globalWeight));
				}
				//Otherwise, if gd(i) is larger than the minimum of the scores of documents in Result, 
				//replace the document with minimum score in R with d(i).
				else
				{
					if (topKDocuments.at(topKDocuments.size() - 1).second < globalWeight && documentIndex != -1)
					{
						topKDocuments.pop_back();
						topKDocuments.push_back(pair<int, double>(documentIndex, globalWeight));
					}
				}

			}
			globalWeight = 0;
			documentIndex = -1;

			//We always sort the result by global weight 
			sort(topKDocuments.begin(), topKDocuments.end(),sort_by_tf_idf);
		}
	} while (topKDocuments.size() < topK  && topKDocuments.at(topKDocuments.size()-1).second< thresHold && list_sorted_by_tf_idf.size() !=0);

	for (unsigned int i = 0; i < topKDocuments.size(); i++)
	{
		topDocuments.push_back(pair<DocumentMetaData, double>(*documentTable->getDocument(topKDocuments.at(i).first), topKDocuments.at(i).second));
	}

	return topDocuments;
}

// This function calculates tf_idf of each token of the query 
vector<vector<pair<int, double>>> Index::calculateTF_IDF(string query)
{
	vector<vector<pair<int, double>>> tf_idfLists;
	vector<pair<int, double>> vector;
	double tf, idf;
	fstream inputStream(invertedFilePath);
	istringstream iss(query);
	string token;
	unsigned int documentsNumber = documentTable->getDocumentNumber();

	//Calculate tf-idf in the loop
	while (iss >> token)
	{
		Term * term = dictionary->getTerm(token);
		if (term != nullptr)
		{
			DocumentTerm * documentTermTable = getTermPostingList(token);
			for (unsigned int i = 0; i < term->documentNumber; i++)
			{
				tf = ((double)documentTermTable[i].ftd / (double)documentTable->getDocument(documentTermTable[i].documentIndex)->wordsNumber);
				idf = (double)documentTable->getDocumentNumber() / (double)term->documentNumber;

				vector.push_back(pair<int, double>(documentTermTable[i].documentIndex, tf*log10(idf)));
			}

			sort(vector.begin(), vector.end(), sort_by_tf_idf);
			tf_idfLists.push_back(vector);
			vector.clear();
		}
	}
	return tf_idfLists;
}

int Index::findPositionOfDocument(int documentIndex, vector<pair<int, double>> vector)
{
	for (unsigned int i = 0; i < vector.size(); i++)
	{
		if (vector.at(i).first == documentIndex)
		{
			return i;
		}
	}
	return -1;
}

bool Index:: sort_by_tf_idf(const pair<int, double>& left, const pair<int, double>& right)
{
	return left.second > right.second;
}


Index::~Index()
{
	delete dictionary;
	delete documentTable;
}
