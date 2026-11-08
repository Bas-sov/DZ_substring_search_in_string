#pragma once
// Участник проекта:
//   1) создаёт свой файл <алгоритм>_by_<Фамилия>.cpp;
//   2) добавляет сюда объявление своей функции;
//   3) капитан напишет строку RUN в main.cpp.
//
// Контракт: вернуть индекс первого вхождения pattern в text,
// если вхождения нет — вернуть -1, если пустой pattern -> 0.

#include <string>
#include "dataset.h"
using namespace std;

using Participant = int(*)(const DataSet& dataset);

// Пример капитана (файл find_by_std.cpp)
int find_by_std(const DataSet& dataset);

// Сюда участники добавляют объявления своих функций
int bm_by_Bakulin(const DataSet& dataset);