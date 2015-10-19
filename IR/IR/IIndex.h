#pragma once
#include <string>
#include <list>
#define DEFAULT_OUTPUT_FILE "myIndex2"
using namespace std;
class IIndex {
public:
	virtual list<int> search(string querry) = 0;
};