#pragma once
#include "Node.h"
class List {
	Node* first = nullptr;
	Node* last = nullptr;
	unsigned int size = 0;
public:
	unsigned int getSize() const;
	List();
	Node* begin();
	void push_back(const unsigned int& t);
	void deleteFirst();
	~List();
};