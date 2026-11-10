#include "Participants.h"

#include <algorithm>
#include <string>
#include <vector>

namespace {

    constexpr int ASIZE = 256;

    // Эвристика "плохого символа"
    // bmBc[c] = сдвиг, если в тексте встретился символ c, а в образце он правее не встречается
    void computeBadChar(const std::string& pat, std::vector<int>& bmBc)
    {
        const int m = static_cast<int>(pat.size());
        bmBc.assign(ASIZE, m);
        for (int i = 0; i < m - 1; ++i)
            bmBc[static_cast<unsigned char>(pat[i])] = m - 1 - i;
    }

    // Вспомогательный массив суффиксов (Z-функция для образца)
    void computeSuffixes(const std::string& pat, std::vector<int>& suff)
    {
        const int m = static_cast<int>(pat.size());
        suff.assign(m, 0);
        suff[m - 1] = m;
        int g = m - 1;
        int f = 0;
        for (int i = m - 2; i >= 0; --i) {
            if (i > g && suff[i + m - 1 - f] < i - g) {
                suff[i] = suff[i + m - 1 - f];
            }
            else {
                if (i < g) g = i;
                f = i;
                while (g >= 0 && pat[g] == pat[g + m - 1 - f])
                    --g;
                suff[i] = f - g;
            }
        }
    }

    // Эвристика "хорошего суффикса"
    void computeGoodSuffix(const std::string& pat, std::vector<int>& bmGs)
    {
        const int m = static_cast<int>(pat.size());
        std::vector<int> suff;
        computeSuffixes(pat, suff);

        bmGs.assign(m, m);

        int j = 0;
        for (int i = m - 1; i >= 0; --i) {
            if (suff[i] == i + 1) {
                for (; j < m - 1 - i; ++j) {
                    if (bmGs[j] == m)
                        bmGs[j] = m - 1 - i;
                }
            }
        }

        for (int i = 0; i <= m - 2; ++i)
            bmGs[m - 1 - suff[i]] = m - 1 - i;
    }

}

int bm_by_Bakulin(const DataSet& dataset)
{
    const std::string& text = dataset.text;
    const std::string& pattern = dataset.pattern;

    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());

    if (m == 0) return 0;   // пустой образец — вхождение в позиции 0
    if (m > n)  return -1;  // образец длиннее текста

    std::vector<int> bmBc, bmGs;
    computeBadChar(pattern, bmBc);
    computeGoodSuffix(pattern, bmGs);

    int s = 0; // текущий сдвиг образца относительно текста
    while (s <= n - m) {
        int j = m - 1;
        // сравнение справа налево
        while (j >= 0 && pattern[j] == text[s + j])
            --j;

        if (j < 0)
            return s;

        const int bcShift =
            bmBc[static_cast<unsigned char>(text[s + j])] - m + 1 + j;
        const int gsShift = bmGs[j];

        s += std::max(bcShift, gsShift);
    }

    return -1;
}