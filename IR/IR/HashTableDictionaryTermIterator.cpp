#include "stdafx.h"
#include "HashTableDictionaryTermIterator.h"

HashTableDictionaryTermIterator::HashTableDictionaryTermIterator(int size, list<Term>  ** hashTable)
{
	HashTableDictionaryTermIterator::size = size;
	HashTableDictionaryTermIterator::hashTable = hashTable;
}

void * HashTableDictionaryTermIterator::getNext()
{
	if (currentElement == -1) {
		processNextTermListIndex();
		if (currentElement < size) {
			it = hashTable[currentElement]->begin();
			return &(*it);
		}
	}
	else {
		it++;
		if (it != hashTable[currentElement]->end()) {
			return &(*it);
		}
		else {
			processNextTermListIndex();
			if (currentElement < size) {
				it = hashTable[currentElement]->begin();
				return &(*it);
			}
		}
	}
	return nullptr;
}
void HashTableDictionaryTermIterator::processNextTermListIndex()
{
	currentElement++;
	while (currentElement < size && hashTable[currentElement] == nullptr) {
		currentElement++;
	}
}
HashTableDictionaryTermIterator::~HashTableDictionaryTermIterator()
{
}
