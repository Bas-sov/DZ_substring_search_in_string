#include "Participants.h"

#include <string>
#include <vector>

namespace {

	// Префикс-функция (она же пи-функция) для образца
	// pi[i] = длина наибольшего собственного префикса pattern[0..i],
	// который одновременно является суффиксом pattern[0..i]
	std::vector<int> computePrefixFunction(const std::string& pat)
	{
		const int m = static_cast<int>(pat.size());
		std::vector<int> pi(m, 0);
		for (int i = 1; i < m; ++i) {
			int k = pi[i - 1];
			while (k > 0 && pat[i] != pat[k])
				k = pi[k - 1];
			if (pat[i] == pat[k])
				++k;
			pi[i] = k;
		}
		return pi;
	}

}

int kmp_by_Star(const DataSet& dataset)
{
	const std::string& text = dataset.text;
	const std::string& pattern = dataset.pattern;

	const int n = static_cast<int>(text.size());
	const int m = static_cast<int>(pattern.size());

	if (m == 0) return 0;   // пустой образец — вхождение в позиции 0
	if (m > n)  return -1;  // образец длиннее текста

	const std::vector<int> pi = computePrefixFunction(pattern);

	int k = 0; // длина текущего совпавшего префикса образца
	for (int i = 0; i < n; ++i) {
		while (k > 0 && text[i] != pattern[k])
			k = pi[k - 1];
		if (text[i] == pattern[k])
			++k;
		if (k == m) {
			// нашли вхождение, заканчивающееся в позиции i
			return i - m + 1;
		}
	}

	return -1;
}