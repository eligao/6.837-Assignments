#pragma once
//#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
void split(std::string line, std::vector<std::string> &parts, std::string pattern)
{
	int n = line.size() + 1;
	char *buffer = new char[n];
// 	strcpy(buffer, line.c_str());
	strcpy_s(buffer, n, line.c_str());
	char *p = (char *)buffer, *q = 0;

	const char *seg = "\r \n\t";
	if (pattern.size() > 0)
		seg = pattern.c_str();

	parts.clear();

	while (1 == 1)
	{
		// trim left
		while (*p && strchr(seg, *p))p++;
		if (*p == 0) break;

		// get info
		q = p;
		while (*q && !strchr(seg, *q))q++;
		char ch = *q; *q = 0;
		parts.push_back(p);
		*q = ch; p = q;
	}

	delete[] buffer;
}