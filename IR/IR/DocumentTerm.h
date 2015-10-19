#pragma once
typedef struct DocumentTerm {
	unsigned long long documentIndex;
	unsigned int ftd=1; // frequency of term in the current document
} DocumentTerm;