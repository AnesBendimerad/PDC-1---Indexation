#pragma once
class IIterator {
public:
	virtual void* getNext() = 0;
};