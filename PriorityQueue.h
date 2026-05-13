#pragma once
class PriorityQueue {
private:
    unsigned int* heap;       // Stores the vertex indices
    unsigned int* priorities; // Stores the priorities
    unsigned int  size;

    void swap(const unsigned int& i, const unsigned int& j) {
        unsigned int temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;

        unsigned int tempPriority = priorities[i];
        priorities[i] = priorities[j];
        priorities[j] = tempPriority;
    }

    void heapifyUp(unsigned int idx) {
        while (idx > 0 && priorities[idx] < priorities[(idx - 1) / 2]) {
            swap(idx, (idx - 1) / 2);
            idx = (idx - 1) / 2;
        }
    }

    void heapifyDown(unsigned int idx) {
        unsigned int left = 2 * idx + 1;
        unsigned int right = 2 * idx + 2;
        unsigned int smallest = idx;

        if (left < size && priorities[left] < priorities[smallest])
            smallest = left;
        if (right < size && priorities[right] < priorities[smallest])
            smallest = right;
        if (smallest != idx) {
            swap(idx, smallest);
            heapifyDown(smallest);
        }
    }

public:
    PriorityQueue(const long long int& edge_numbers) : size(0) {
        heap = new unsigned int[edge_numbers];
        priorities = new unsigned int[edge_numbers];
    }

    void insert(int vertex, int priority) {
        heap[size] = vertex;
        priorities[size] = priority;
        heapifyUp(size);
        size++;
    }

    int extractMin() {
        int minVertex = heap[0];
        heap[0] = heap[size - 1];
        priorities[0] = priorities[size - 1];
        size--;
        heapifyDown(0);
        return minVertex;
    }

    bool isEmpty() {
        return size == 0;
    }

    void decreasePriority(int vertex, int newPriority) {
        for (int i = 0; i < size; i++) {
            if (heap[i] == vertex) {
                priorities[i] = newPriority;
                heapifyUp(i);
                break;
            }
        }
    }
    ~PriorityQueue() {
        delete[] heap;
        delete[] priorities;
    }
};
