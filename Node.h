#pragma once
struct Node {
	long long int val;
	Node* prev = nullptr;
	Node* next = nullptr;
	Node(const long long int& v) {
		val = v;
	}
	Node(Node* n) {
		val = n->val;
		next = n->next;
		prev = n->prev;
	}
};