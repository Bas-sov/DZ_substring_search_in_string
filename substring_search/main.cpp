// Основная программа
// Результат пишется в консоль и в result.txt
// Запускать только в Release!

#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include "dataset.h"
#include "Participants.h"

using namespace std;

// perebor_by_Belova -> "perebor\tbelova" (метод и автор — колонки таблицы)
string ParseName(string& method_name)
{
	transform(method_name.begin(), method_name.end(), method_name.begin(),
		[](unsigned char c) { return tolower(c); });
	auto res = regex_replace(method_name, regex("_by_"), "\t");
	if (res.length() == method_name.length())
		res = regex_replace(res, regex("_"), "\t");
	return res;
}

#define RUN(x) {                             \
	Run(#x, x, ds);                          \
}

static string dataset = "";
static vector<string> logs;

void Run(string method_name, Participant p, const DataSet& ds)
{
	auto start = chrono::steady_clock::now();
	int res = p(ds);
	auto stop = chrono::steady_clock::now();
	auto time = chrono::duration_cast<chrono::microseconds>(stop - start).count();

	size_t pos = ds.text.find(ds.pattern);
	int expected = (pos == string::npos) ? -1 : (int)pos;

	ostringstream oss{};
	oss << "\"" << dataset << "\"\t"
		<< ds.text.size() << "\t"
		<< ds.pattern.size() << "\t"
		<< ParseName(method_name) << "\t"
		<< res << "\t"
		<< ((res == expected) ? to_string(time) : "-1");
	logs.push_back(oss.str());
	cout << oss.str() << endl;
}

int main()
{
	string header = "dataset\tn\tm\tmethod\tauthor\tanswer\ttime_us";
	cout << header << endl;
	logs.push_back(header);

	auto dataSets = all_datasets();
	int index = 0;
	for (auto& ds : dataSets)
	{
		string s = to_string(++index);
		dataset = string("test_") + (s.size() < 2 ? "0" : "") + s;

		RUN(find_by_std); // Образец запуска
	}

	ofstream fout("result.txt");
	for (auto& s : logs)
		fout << s << endl;
}
