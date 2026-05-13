#include "List.h"
#include <iostream>
using namespace std;

List::List() {

}

void List::push_back(const unsigned int& t) {
	if (first == nullptr) {
		first = new Node(t);
		last = first;
		first->next = nullptr;
		first->prev = nullptr;
		return;
	}

	last->next = new Node(t);
	last->next->prev = last;
	last = last->next;
	++size;
}

void List::deleteFirst() {
	if (first == nullptr) {
		return;
	}
	--size;
	Node* tmp = first;
	first = first->next;

	if (first != nullptr) {
		first->prev = nullptr;
	}
	else {
		// Jesli lista jest teraz pusta, ustawiam last na nullptr
		last = nullptr;
	}

	delete tmp;
}

unsigned int List::getSize() const {
	return size;
}
Node* List::begin() {
	return first;
}

List::~List() {
	Node* cur = first;
	while (cur != nullptr) {
		Node* next = cur->next;
		delete cur;
		cur = next;
	}
	first = nullptr;
	last = nullptr;
	size = 0;
}