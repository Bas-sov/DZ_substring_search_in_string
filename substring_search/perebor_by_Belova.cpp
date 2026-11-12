#include "Participants.h"

int perebor_by_Belova(const DataSet& dataset)
{
    const std::string& text = dataset.text;
    const std::string& pattern = dataset.pattern;

   
    if (pattern.empty()) {
        return 0;
    }

    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    
    if (m > n) {
        return -1;
    }

    
    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            return i;   
        }
    }

    return -1;       
}
