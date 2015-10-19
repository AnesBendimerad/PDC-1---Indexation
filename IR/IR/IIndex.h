#pragma once
#include <string>
#include <vector>
#include "DocumentMetaData.h"
#define DEFAULT_OUTPUT_FILE "myIndex2"
#define FAGIN_INDEX_TYPE 1
using namespace std;
class IIndex {
public:
	virtual vector<pair<DocumentMetaData, double>>  search(int topK,string query) = 0;
};
