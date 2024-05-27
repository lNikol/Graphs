#include "List.h"
#include <iostream>
using namespace std;

List::List() {

}

void List::push_back(const long long int& t) {
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
}

void List::pop_back() {
	if (first == nullptr) {
		return;
	}

	// usuwam pierwszy (on jest ostatnim) element z listy
	if (first->next == nullptr) {
		delete first;
		first = nullptr;
		last = nullptr;
		return;
	}

	Node* tmp = last->prev;
	delete last;
	last = tmp;
	if (last != nullptr) {
		last->next = nullptr;
	}
}

void List::deleteFirst() {
	if (first == nullptr) {
		return;
	}

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


Node* List::begin() {
	return first;
}

Node* List::end() {
	if (last == nullptr) {
		return nullptr;
	}
	else {
		return last;
	}
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
}