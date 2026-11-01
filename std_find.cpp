#include "algorithms.h"

int std_find_search(const std::string& text, const std::string& pattern) {
	const std::size_t pos = text.find(pattern);
	return pos == std::string::npos ? -1 : static_cast<int>(pos);
}
