#pragma once
#include <string>
#include <list>
#define DEFAULT_OUTPUT_FILE "myIndex"
using namespace std;
class IIndex {
public:
	virtual list<int> search(int topK,string query) = 0;
};