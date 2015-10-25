#pragma once
#include <list>
#include "IDictionary.h"
#include "IHasher.h"
#include "Term.h"
#include <vector>
#define DEFAULT_SIZE 10009

class HashTableDictionary : public IDictionary {
private:
	IHasher * hasher;
	list<Term>  ** hashTable;
	unsigned int size;
	unsigned long long termsNumber;


	static bool compare(const void * firstTerm, const void * secondTerm);
	vector<Term *> sortTermsByOccurances();
	unsigned int termsListMemorySize(list<Term>* termsList);
public:
	HashTableDictionary(unsigned int size,IHasher *hasher);
	HashTableDictionary();
	Term* addTerm(string token) ;
	void addTerm(Term* term);
	Term* getTerm(string token) ;
	IIterator * getIterator();
	unsigned long long& getTermsNumber();
	unsigned long long getMemorySize();

	unsigned long long getTokenId(string token);
	Term* getTermById(unsigned long long id);
	void writeCSVFile();

	~HashTableDictionary();
};