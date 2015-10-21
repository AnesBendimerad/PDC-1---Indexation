#pragma once
using namespace std;
#include <fstream>

typedef struct Triplet {
	unsigned long long termId;
	unsigned int documentId;
	unsigned int ftd = 1;
} Triplet;

bool operator==(const Triplet& triplet1, const Triplet& triplet2);
bool operator!=(const Triplet& triplet1, const Triplet& triplet2);
bool operator<(const Triplet& triplet1, const Triplet& triplet2);
bool operator>(const Triplet& triplet1, const Triplet& triplet2);
bool operator<=(const Triplet& triplet1, const Triplet& triplet2);
bool operator>=(const Triplet& triplet1, const Triplet& triplet2);

ofstream & operator << (std::ofstream & os, Triplet* rec);
ifstream & operator >> (std::ifstream & is, Triplet* rec);