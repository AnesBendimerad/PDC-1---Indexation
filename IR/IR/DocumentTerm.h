#pragma once
typedef struct DocumentTerm {
	unsigned int documentIndex;
	unsigned int ftd; // frequency of term in the current document
} DocumentTerm;