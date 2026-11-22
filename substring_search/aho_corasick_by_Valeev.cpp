#include "Participants.h"
#include <string>
#include <vector>
#include <queue>

using namespace std;

const int ALPHABET = 256;   // все возможные символы (char)

// Одна вершина бора
struct Node {
	int next[ALPHABET];  // переходы по символам (-1 если нет)
	int link;            // суффиксная ссылка
	int termLink;        // ближайшая по ссылкам вершина, где кончается слово (-1 если нет)
	int wordIndex;       // номер слова, которое кончается здесь (-1 если никакое)

	Node() {
		for (int i = 0; i < ALPHABET; i++) next[i] = -1;
		link = 0;
		termLink = -1;
		wordIndex = -1;
	}
};

// Одно найденное вхождение
struct Match {
	int pos;    // с какой позиции в тексте начинается слово
	int word;   // номер слова в словаре
};

class AhoCorasick {
private:
	vector<string> words;   // словарь
	vector<Node> nodes;     // вершины бора (0 - корень)
	bool built;             // построен ли автомат для текущего словаря

	// Строим бор + суффиксные ссылки заново по всему словарю
	void build() {
		nodes.clear();
		nodes.push_back(Node());   // корень

		// 1. Строим бор
		for (int i = 0; i < (int)words.size(); i++) {
			if (words[i].empty()) continue;   // пустые слова пропускаем

			int v = 0;
			for (int j = 0; j < (int)words[i].size(); j++) {
				int c = (unsigned char)words[i][j];
				if (nodes[v].next[c] == -1) {
					nodes[v].next[c] = nodes.size();
					nodes.push_back(Node());
				}
				v = nodes[v].next[c];
			}
			// если такое слово уже было, оставляем первое
			if (nodes[v].wordIndex == -1) nodes[v].wordIndex = i;
		}

		// 2. Обход в ширину: считаем суффиксные ссылки и достраиваем переходы
		queue<int> q;

		// корень обрабатываем отдельно
		for (int c = 0; c < ALPHABET; c++) {
			if (nodes[0].next[c] == -1) {
				nodes[0].next[c] = 0;        // нет перехода - остаёмся в корне
			}
			else {
				int child = nodes[0].next[c];
				nodes[child].link = 0;
				q.push(child);
			}
		}

		while (!q.empty()) {
			int v = q.front();
			q.pop();

			int l = nodes[v].link;

			// ссылка на ближайшую вершину, где заканчивается слово
			if (nodes[l].wordIndex != -1) nodes[v].termLink = l;
			else nodes[v].termLink = nodes[l].termLink;

			for (int c = 0; c < ALPHABET; c++) {
				int u = nodes[v].next[c];
				if (u == -1) {
					// перехода нет - берём переход из суффиксной ссылки
					nodes[v].next[c] = nodes[l].next[c];
				}
				else {
					nodes[u].link = nodes[l].next[c];
					q.push(u);
				}
			}
		}

		built = true;
	}

public:
	AhoCorasick() {
		built = false;
	}

	// Добавить слово в словарь
	void addWord(const string& w) {
		words.push_back(w);
		built = false;   // словарь изменился - надо перестроить
	}

	// Очистить словарь
	void clear() {
		words.clear();
		nodes.clear();
		built = false;
	}

	int wordsCount() const {
		return words.size();
	}

	// Найти все вхождения всех слов словаря в тексте
	vector<Match> search(const string& text) {
		vector<Match> result;
		if (!built) build();

		int v = 0;
		for (int i = 0; i < (int)text.size(); i++) {
			int c = (unsigned char)text[i];
			v = nodes[v].next[c];

			// само слово, которое кончается в вершине v
			if (nodes[v].wordIndex != -1) {
				Match m;
				m.word = nodes[v].wordIndex;
				m.pos = i - (int)words[m.word].size() + 1;
				result.push_back(m);
			}

			// другие слова (суффиксы), которые тоже кончаются здесь
			int u = nodes[v].termLink;
			while (u != -1) {
				Match m;
				m.word = nodes[u].wordIndex;
				m.pos = i - (int)words[m.word].size() + 1;
				result.push_back(m);
				u = nodes[u].termLink;
			}
		}
		return result;
	}

	// Найти первое вхождение (останавливаемся сразу, как нашли).
	// Возвращает позицию начала или -1, если ничего нет.
	// Для одного слова первое найденное вхождение - самое левое.
	int findFirst(const string& text) {
		if (!built) build();

		int v = 0;
		for (int i = 0; i < (int)text.size(); i++) {
			int c = (unsigned char)text[i];
			v = nodes[v].next[c];

			if (nodes[v].wordIndex != -1) {
				return i - (int)words[nodes[v].wordIndex].size() + 1;
			}
			int u = nodes[v].termLink;
			if (u != -1) {
				return i - (int)words[nodes[u].wordIndex].size() + 1;
			}
		}
		return -1;
	}

	string getWord(int i) const {
		return words[i];
	}
};

int aho_by_Val(const DataSet& dataset)
{
	// ищем dataset.pattern в dataset.text
	// вернуть индекс первого вхождения (с 0) или -1, если вхождения нет

	// пустой pattern -> 0
	if (dataset.pattern.empty()) return 0;

	AhoCorasick ac;
	ac.addWord(dataset.pattern);

	return ac.findFirst(dataset.text);
}