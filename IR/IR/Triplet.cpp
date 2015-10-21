#include "stdafx.h"
#include "Triplet.h"

bool operator==(const Triplet& triplet1, const Triplet& triplet2)
{
	return (triplet1.termId == triplet2.termId && triplet1.documentId == triplet2.documentId);
}

bool operator!=(const Triplet& triplet1, const Triplet& triplet2)
{
	return !(triplet1 == triplet2);
}

bool operator<(const Triplet& triplet1, const Triplet& triplet2)
{
	return (triplet1.termId < triplet2.termId || (triplet1.termId == triplet2.termId && triplet1.documentId < triplet2.documentId));
}

bool operator>(const Triplet& triplet1, const Triplet& triplet2)
{
	return triplet2 < triplet1;
}

bool operator<=(const Triplet& triplet1, const Triplet& triplet2)
{
	return !(triplet2 < triplet1);
}
bool operator>=(const Triplet& triplet1, const Triplet& triplet2)
{
	return !(triplet1 < triplet2);
}

ofstream & operator << (std::ofstream & os, Triplet * rec) {
	os.write((char*) rec, sizeof(Triplet));
	return os;
}

ifstream & operator >> (std::ifstream & is, Triplet * rec) {
	is.read((char *)rec, sizeof(Triplet));
	return is;
}