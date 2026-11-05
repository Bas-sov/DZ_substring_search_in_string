#pragma once
#include <string>
#include <vector>

struct DataSet {
	std::string text;
	std::string pattern;
};

std::vector<DataSet> all_datasets();
