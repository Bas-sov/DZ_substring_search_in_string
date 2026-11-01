#pragma once
#include <string>

using SearchFunction = int (*)(const std::string& text, const std::string& pattern);

int std_find_search(const std::string& text, const std::string& pattern);
int perebor_search_Belova(const std::string& text, const std::string& pattern);
int aho_corasick_search_Valeev(const std::string& text, const std::string& pattern);
int boyer_moore_search_Bakulin(const std::string& text, const std::string& pattern);
int kmp_search_Starikov(const std::string& text, const std::string& pattern);

struct Algorithm {
	std::string name;
	SearchFunction function;
};
