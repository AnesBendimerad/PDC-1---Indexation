#pragma once
#include <string>
#include <list>
using namespace std;
class IIndex {
public:
	virtual list<int> search(string querry) = 0;
};