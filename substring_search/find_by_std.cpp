// Пример капитана: так выглядит готовый файл участника
// Поиск через std::string::find

#include "Participants.h"

int find_by_std(const DataSet& dataset)
{
	size_t pos = dataset.text.find(dataset.pattern);
	return pos == string::npos ? -1 : (int)pos;
}
