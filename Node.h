#pragma once
struct Node {
	unsigned int val;
	Node* prev = nullptr;
	Node* next = nullptr;
	Node(const unsigned int& v) {
		val = v;
	}
};