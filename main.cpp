#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "algorithms.h"
#include "dataset.h"

int reference_find(const std::string& text, const std::string& pattern) {
	const std::size_t pos = text.find(pattern);
	return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

std::string dataset_label(std::size_t index) {
	const std::string s = std::to_string(index + 1);
	return std::string("test_") + (s.size() < 2 ? "0" : "") + s;
}

void print_line(std::ofstream& file, const std::string& line) {
	std::cout << line << '\n';
	file << line << '\n';
}

int main() {
	const std::vector<Algorithm> algorithms = {
		{ "std_find",     std_find_search },
		{ "perebor",      perebor_search_Belova },
		{ "aho_corasick", aho_corasick_search_Valeev },
		{ "boyer_moore",  boyer_moore_search_Bakulin },
		{ "kmp",          kmp_search_Starikov }
	};

	const std::vector<DataSet> datasets = all_datasets();

	std::ofstream file("result.txt");
	if (!file) {
		std::cerr << "Не удалось создать result.txt\n";
		return 2;
	}

	print_line(file, "dataset   algorithm       n         m    answer   time_us     status");
	print_line(file, std::string(70, '-'));

	int failures = 0;
	std::vector<double> total_us(algorithms.size(), 0.0);
	std::vector<int>    ok_count(algorithms.size(), 0);

	for (std::size_t d = 0; d < datasets.size(); ++d) {
		const DataSet& ds = datasets[d];
		const std::string label = dataset_label(d);
		const int         expected = reference_find(ds.text, ds.pattern);

		for (std::size_t a = 0; a < algorithms.size(); ++a) {
			const Algorithm& algo = algorithms[a];
			int    answer = -1;
			double us = 0.0;
			bool   crashed = false;

			try {
				const auto t0 = std::chrono::steady_clock::now();
				answer = algo.function(ds.text, ds.pattern);
				const auto t1 = std::chrono::steady_clock::now();
				us = std::chrono::duration<double, std::micro>(t1 - t0).count();
			}
			catch (...) {
				crashed = true;
			}

			const bool ok = !crashed && (answer == expected);
			if (ok) ++ok_count[a]; else ++failures;
			total_us[a] += us;

			std::ostringstream line;
			line << std::left
				<< std::setw(10) << label
				<< std::setw(14) << algo.name
				<< std::setw(10) << ds.text.size()
				<< std::setw(5) << ds.pattern.size()
				<< std::setw(9) << answer
				<< std::setw(12) << std::fixed << std::setprecision(1) << us
				<< (ok ? "OK" : ("FAIL (ожидалось " + std::to_string(expected) + ")"));
			print_line(file, line.str());
		}
	}

	std::cout << std::string(70, '-') << '\n';
	std::cout << std::left
		<< std::setw(15) << "algorithm"
		<< std::setw(14) << "total_us"
		<< "passed\n";
	for (std::size_t a = 0; a < algorithms.size(); ++a) {
		std::cout << std::left
			<< std::setw(15) << algorithms[a].name
			<< std::setw(14) << std::fixed << std::setprecision(1) << total_us[a]
			<< ok_count[a] << '/' << datasets.size() << '\n';
	}

	print_line(file, std::string(70, '-'));
	if (failures == 0) {
		print_line(file, "ВСЕ ТЕСТЫ ПРОЙДЕНЫ");
	}
	else {
		print_line(file, "ОШИБОК: " + std::to_string(failures));
	}
	return failures == 0 ? 0 : 1;
}
