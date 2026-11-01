#include "dataset.h"


std::string repeat(const std::string& s, std::size_t times) {
	std::string r;
	r.reserve(s.size() * times);
	for (std::size_t i = 0; i < times; ++i) r += s;
	return r;
}


std::vector<DataSet> all_datasets() {
	return {
		{ "hello world, this is a simple test", "world" },
		{ "hello world, this is a simple test", "xyz" },
		{ repeat("a", 100000), "aabaa" },
		{ "", "" },
		{ "c", "abababc" },
		{ "aab", "aacaac" },
		{ "anything at all", "" },
		{ repeat("short", 1000), "shortshort" },
		{ "exactly the same", "exactly the same" },
	};
}
