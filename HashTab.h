#pragma once

#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <fstream>
#include <string>
#include "Contact.h"

// Размер таблицы по умолчанию
#define DEFAULT_SIZE 128

// Идентификаторы для стека отмены действий
enum {INS, DEL};

// Класс узла (контакта)
template <class TKey, typename TData>
struct Node {

// Пара ключ-значение
	TKey key;
	TData data;

// Конструкторы
	Node() = default;
	Node(const TKey& nKey, const TData& nData) : key(nKey), data(nData) {};

// Перегрузка сравнения
	bool operator ==(const Node<TKey, TData> other) const {
		return key == other.key;
	}
};


// Класс хэш-таблицы
template <typename TKey, typename TData>
class HashTab {
public:

// Класс итератора
	class HIterator {
	public:
		HIterator() = default;
		HIterator(const HashTab<TKey, TData>* table, int iVec, int iList);

// Инкремент
		HIterator& operator ++();

// Разыменовывание
		Node<TKey, TData> operator *() { 
			return *curNode; 
		}

// Перегрузка сравнения
		bool operator ==(const HIterator& rhd) const {
			return curNode == rhd.curNode;
		}
		bool operator !=(const HIterator& rhd) const {
			return curNode != rhd.curNode;
		}

// Вывод содержимого итератора на консоль
		void print();
	private:

// Итератор содержит указатель на таблицу,
// указатель на узел, индекс в векторе и
// индекс в списке
		const Node<TKey, TData>* curNode;
		const HashTab<TKey, TData>* curTable;
		int curVectorI = 0,
				curListI   = 0;
	};

	HashTab() { tableStore.resize(DEFAULT_SIZE); }
	HashTab(int size) { tableStore.resize(size); }

// Хэш-функция
	int hash(const std::string&) const;

// Вставка узла
	int insert(const TKey&, const TData&);

// Итераторы: начало, конец, поиск,
// поиск по значению
	HIterator begin() const;
	HIterator end() const;
	HIterator find(const TKey&) const;
	HIterator find_value(const std::string&);

// Удаление элемента
	int erase(const TKey&);

// Отмена действий
	void undo(int m);

// Печать содержимого таблицы
	void print() const;

// Перегрузка вывода в поток (файл)
	template <typename UKey, class UData>
	friend std::ofstream&operator<<(std::ofstream&,const HashTab<UKey,UData>&);


private:

// Хэш-таблица: вектор списков с узлами
// (для решения проблемы коллизии: метод цепочек
	std::vector<std::list<Node<TKey, TData>>> tableStore;

// Хранение произведённых действий: вектор идентификаторов
	std::stack<int> actionsStack;

// Стэк вставок
	std::stack<Node<TKey, TData>> insertHistory;

// Стэк удалений
	std::stack<Node<TKey, TData>> removeHistory;
};


// Хэширование
// Каждый символ умножается на длину строки
// и берётся остаток от деления на размер таблицы
// Хэши символов суммируются и берётся остаток
// от деления на размер таблицы
// Так гарантируется отсутствие проблемы с выходом
// за пределы размера таблицы
template <typename TKey, typename TData>
int HashTab<TKey, TData>::hash(const std::string& key) const {
	int result = 0;
	for (int i = 0; i < key.length(); ++i)
		result += key[i] - '0';
	return abs(int(result % tableStore.size()));
}

// Печать содержимого таблицы
template <typename TKey, typename TData>
void HashTab<TKey, TData>::print() const {
	for (auto it : (*this))
		std::cout << it.data << std::endl;
}

/// Вставка узла
// Ищется узел с заданным ключом
// Если такового нет, он добавляется в позицию вектора
// в конец списка
// Если есть, данные по ключу перезаписываются
// (модификация таблицы)
template <typename TKey, typename TData>
int HashTab<TKey, TData>::insert(const TKey& key, const TData& data) {
	int result;
	int hKey = hash(key);
	Node<TKey, TData> newNode(key, data);
	auto it = std::find(tableStore[hKey].begin(), tableStore[hKey].end(), newNode);
	if (tableStore[hKey].end() == it) {
		tableStore[hKey].push_back(newNode);
		result = 0;
	}
	else {
		(*it).data = data;
		result = 1;
	}
	actionsStack.push(INS);
	insertHistory.push(newNode);
	return result;
}

// Итератор на первый элемент таблицы
template <typename TKey, typename TData>
typename HashTab<TKey, TData>::HIterator HashTab<TKey, TData>::begin() const {
	int i;
	for (i = 0; i != tableStore.size() && tableStore[i].empty(); ++i);
	return HIterator(this, i, 0);
}

// Итератор на последний элемент таблицы
template <typename TKey, typename TData>
typename HashTab<TKey, TData>::HIterator HashTab<TKey, TData>::end() const {
	int iHash = 0,
			iList = 0;
	for (int i = tableStore.size() - 1; i >= 0; --i)
		if (!tableStore[i].empty()) {
			return HashTab<TKey, TData>::HIterator(this, ++i, iList);
		}
	return this->begin();
}

// Конструктор итератора
template <typename TKey, typename TData>
HashTab<TKey, TData>::HIterator::HIterator(
			const HashTab<TKey, TData>* table, int iHash, int iList) 
{
	this->curTable = table;
	this->curNode = nullptr;

	if (iHash >= curTable->tableStore.size())
		return;
	this->curVectorI = iHash;

	if (iList >= curTable->tableStore[iHash].size())
		return;
	this->curListI = iList;

	if (curTable->tableStore[iHash].begin() != curTable->tableStore[iHash].end())
		this->curNode = &(*next(curTable->tableStore[iHash].begin(), iList));
}

// Инкремент итератора
// Если итератор на конце списка, следующий будет
// находиться на следующем элементе вектора,
// на первом элементе списка
template <typename TKey, typename TData>
typename HashTab<TKey, TData>::HIterator& 
				 HashTab<TKey, TData>::HIterator::operator++()
{
	if (this->curTable->tableStore[curVectorI].size() - 1 == this->curListI) {
		int tmpIndex = this->curVectorI;
		for (int i = curVectorI + 1; i < curTable->tableStore.size(); ++i) {
			if (!this->curTable->tableStore[i].empty()) {
				this->curVectorI = i;
				break;
			}
		}
		if (tmpIndex == curVectorI) {
			curNode = nullptr;
			return *this;
		}
		curListI = 0;
	}
	else
		++(this->curListI);
	if (curNode == nullptr)
		return *this;
	this->curNode = &(*next(this->curTable->tableStore[curVectorI].begin(),
							curListI));
	return *this;
}

// Поиск элемента: возвращается итератор
// Если не найден, то на конец таблицы
template <typename TKey, typename TData>
typename HashTab<TKey, TData>::HIterator 
		 HashTab<TKey, TData>::find(const TKey& key) const 
{
	int hKey = hash(key),
		iList = 0;
	for (auto it : tableStore[hKey]) {
		if (key == it.key)
			return HIterator(this, hKey, iList);
		++iList;
	}
	return this->tableStore.end();
}


// Поиск по значению
// Поочерёдный перебор элементов
template <typename TKey, typename TData>
typename HashTab<TKey, TData>::HIterator
	HashTab<TKey, TData>::find_value(const std::string& subStr)
{
	for (int iHash = 0; iHash < tableStore.size(); ++iHash) {
		int iList = 0;
		for (auto jt : tableStore[iHash]) {
			if (jt.data.find_by_param(subStr))
				return HIterator(this, iHash, iList);
			++iList;
		}
	}
	return this->end();
}

// Печать содержимого итератора
template <typename TKey, typename TData>
void HashTab<TKey, TData>::HIterator::print() {
	if (!(nullptr == this->curNode))
		cout << curNode->data;
}

// Удаление элемента
template <typename TKey, typename TData>
int HashTab<TKey, TData>::erase(const TKey& key)
{
	int hKey = hash(key);
	for (auto jt : tableStore[hKey]) {
		if (key == jt.key) {
			removeHistory.push(jt);
			actionsStack.push(DEL);
			tableStore[hKey].remove(jt);
			return 0;
		}
	}
	return 1;
}

// Отмена действий
// Достаются элементы стэка и производятся
// обратные действия
template <typename TKey, typename TData>
void HashTab<TKey, TData>::undo(int m) {
	int arrSize = actionsStack.size();
	for (int i = 0; i < m && i < arrSize ; ++i) {
		if (actionsStack.top() == DEL) {
			Node<TKey, TData> newNode(removeHistory.top().key, removeHistory.top().data);
			tableStore[hash(removeHistory.top().key)].push_back(newNode);
			removeHistory.pop();
			actionsStack.pop();
		}
		else if (actionsStack.top() == INS) {
			int hKey = hash(insertHistory.top().key);
			for (auto it : tableStore[hKey])
				if (insertHistory.top().key == it.key) {
					tableStore[hKey].remove(it);
					insertHistory.pop();
					actionsStack.pop();
					break;
				}
		}
	}
	while (!actionsStack.empty()) actionsStack.pop();
	while (!removeHistory.empty()) removeHistory.pop();
	while (!insertHistory.empty()) insertHistory.pop();
}

// Вывод в файл
template <typename TKey, class TData>
std::ofstream& operator<< (std::ofstream& fout, const HashTab<TKey, TData>& right) {
	for (auto it : right.tableStore)
		for (auto jt : it)
			fout << jt.data;
	return fout;
}
