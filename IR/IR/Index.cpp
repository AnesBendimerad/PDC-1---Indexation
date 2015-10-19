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
using namespace std;


Index::Index(IDictionary * dictionary, DocumentTable * documentTable, string invertedFilePath)
{
	Index::dictionary = dictionary;
	Index::documentTable = documentTable;
	Index::invertedFilePath = invertedFilePath;
}

DocumentTerm * Index::getTermPostingList(string token)
{
	ifstream inputStream(invertedFilePath, ios::in | ios::binary);
	Term * term = dictionary->getTerm(token);
	if (term != nullptr) {
		DocumentTerm* documentTermTable= (DocumentTerm*)malloc(sizeof(DocumentTerm)*term->documentNumber);
		inputStream.seekg((unsigned int) term->postingList);
		inputStream.read((char * )documentTermTable, sizeof(DocumentTerm)*term->documentNumber);
		inputStream.close();
		return documentTermTable;
	}
	inputStream.close();
	return nullptr;
}




// This function implements fagin's algorithm and return the topK documents which matchs the query
list<int> Index::search(int topK, string query)
{
	vector<vector<pair<int, double>>> list_sorted_by_tf_idf = calculateTF_IDF(query);
	vector<pair<int, double>> topKDocuments;
	list<int> topDocuments;

	double thresHold;
	double weight=0, globalWeight =0;
	int documentIndex;
	int result;

	do
	{
		thresHold = 0;
		for (int i = 0; i < list_sorted_by_tf_idf.size(); i++)
		{
			if (list_sorted_by_tf_idf.at(i).size() != 0)
			{
				//Retrieve the document d(i) containing term ti that has the next largest tf-idf then remove it
				documentIndex = list_sorted_by_tf_idf.at(i).begin()->first;
				weight = list_sorted_by_tf_idf.at(i).begin()->second;
				list_sorted_by_tf_idf.at(i).erase(list_sorted_by_tf_idf.at(i).begin());
			}
			else
			{
				list_sorted_by_tf_idf.erase(list_sorted_by_tf_idf.begin()+i);
			}

			//Compute its global score by retrieving all s(tj, d(i)) with j?i
			for (int j = 0; j < list_sorted_by_tf_idf.size(); j++)
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
				if (topKDocuments.at(topKDocuments.size()-1).second < globalWeight && documentIndex != -1)
				{
					topKDocuments.erase(topKDocuments.end());
					topKDocuments.push_back(pair<int, double>(documentIndex, globalWeight));
				}
			}

			globalWeight = 0;
			documentIndex = -1;
			//We always sort the result by tf_idf 
			sort(topKDocuments.begin(), topKDocuments.end(), sort_by_tf_idf);
		}
	} while (topKDocuments.size() < topK  && topKDocuments.at(topKDocuments.size()-1).second< thresHold && list_sorted_by_tf_idf.size() !=0);

	for (int i = 0; i < topKDocuments.size(); i++)
	{
		cout << "documentindex:" << topKDocuments.at(i).first << endl;
		cout << "rank:" << topKDocuments.at(i).second << endl;
		cout << endl;
		topDocuments.push_back(topKDocuments.at(i).first);
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
	unsigned long long termsNumber = 0;
	unsigned int documentsNumber = 0;

	//Retrieve Number of terms in the dictionary and Number of Documents in the corpus
	inputStream.read((char*)&termsNumber, sizeof(unsigned long long));
	inputStream.read((char*)&documentsNumber, sizeof(unsigned int));

	//Retrieve Documents MetaData
	DocumentMetaData *documentTable = (DocumentMetaData*)malloc(sizeof(DocumentMetaData)*documentsNumber);
	inputStream.read((char *)documentTable, sizeof(DocumentMetaData)*documentsNumber);

	//Calculate tf-idf in the loop
	while (iss >> token)
	{
		Term * term = dictionary->getTerm(token);
		if (term != nullptr)
		{
			DocumentTerm * documentTermTable = getTermPostingList(token);
			for (int i = 0; i < term->documentNumber; i++)
			{
				tf = ((double)documentTermTable[i].ftd / (double)documentTable[documentTermTable[i].documentIndex].wordsNumber);
				idf = (double)documentsNumber / (double)term->documentNumber;

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
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector.at(i).first == documentIndex)
		{
			return i;
		}
	}
	return -1;
}

bool sort_by_tf_idf(const pair<int, double>& left, const pair<int, double>& right)
{
	return left.second > right.second;
}


Index::~Index()
{
	delete dictionary;
	delete documentTable;
}