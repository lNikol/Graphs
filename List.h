#pragma once
#include "Node.h"
class List {
	Node* first = nullptr;
	Node* last = nullptr;
public:
	List();
	Node* begin();
	Node* end();
	void push_back(const long long int& t);
	void deleteFirst();
	void pop_back();
	~List();
};