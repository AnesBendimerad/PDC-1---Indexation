#pragma once
#include <string>
#include <list>
#define DEFAULT_OUTPUT_FILE "myIndex2"
#define FAGIN_INDEX_TYPE 1
using namespace std;
class IIndex {
public:
	virtual list<int> search(int topK,string query) = 0;
};