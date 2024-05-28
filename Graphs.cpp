#include <iostream>
#include "List.h"
using namespace std;
using ln = long long int;
using Graph = ln**;

// USUNĄĆ ZBĘDNE METODY W LIST
// 1,2,3,7,8

ln currentNum(ln& n) {
    return (n - 1 >= 0 ? n - 1 : n);
}

void DFS(Graph& gr, bool* visited, const ln& start, ln* degrees) {
    List stack;
    stack.push_back(start);
    while (stack.begin() != nullptr) {
        Node* node = stack.begin();
        if (!visited[node->val]) {
            visited[node->val] = true;
            for (ln x = 0; x < degrees[node->val]; ++x) {
                ln temp = gr[node->val][x];
                if (!visited[temp]) {
                    stack.push_back(temp);
                }
            }
        }
        stack.deleteFirst();
    }
}

bool colorDFS(const Graph& gr, ln* color, const ln& start, ln* degrees) {
    List s;
    s.push_back(start);
    color[start] = 0;  // Start coloring with 0

    while (s.begin() != nullptr) {
        Node* node = s.begin();

        for (ln x = 0; x < degrees[node->val]; ++x) {
            ln temp = gr[node->val][x];
            if (color[temp] == -1) {  // If not colored
                color[temp] = 1 - color[node->val];  // Color with opposite color
                s.push_back(temp);
            }
            else if (color[temp] == color[node->val]) {
                // If the neighbor has the same color, the graph is not bipartite
                return false;
            }
        }
        s.deleteFirst();
    }
    return true;
}

bool isBipartite(const Graph& gr, const ln& size, ln* degrees) {
    ln* color = new ln[size];  // -1 means uncolored, 0 and 1 are the two colors
    for (ln x = 0; x < size; ++x) {
        color[x] = -1;
    }
    for (ln start = 0; start < size; ++start) {
        if (color[start] == -1) {  // Not yet colored
            if (!colorDFS(gr, color, start, degrees)) {
                delete[] color;
                return false;
            }
        }
    }
    delete[] color;
    return true;
}

ln countComponents(Graph& gr, const ln& size, ln* degrees) {
    ln components = 0;
    bool* visited = new bool[size]();

    for (ln i = 0; i < size; ++i) {
        if (!visited[i]) {
            DFS(gr, visited, i, degrees);
            ++components;
        }
    }
    delete[] visited;
    return components;
}

void merge(ln* degrees, ln* order, const ln& left, const ln& mid, const ln& right) {
    ln n1 = mid - left + 1;
    ln n2 = right - mid;

    ln* L = new ln[n1];
    ln* R = new ln[n2];
    ln* LOrder = new ln[n1];
    ln* ROrder = new ln[n2];

    for (ln i = 0; i < n1; ++i) {
        L[i] = degrees[left + i];
        LOrder[i] = order[left + i];
    }
    for (ln j = 0; j < n2; ++j) {
        R[j] = degrees[mid + 1 + j];
        ROrder[j] = order[mid + 1 + j];
    }

    ln i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] >= R[j]) {
            degrees[k] = L[i];
            order[k] = LOrder[i];
            ++i;
        }
        else {
            degrees[k] = R[j];
            order[k] = ROrder[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        degrees[k] = L[i];
        order[k] = LOrder[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        degrees[k] = R[j];
        order[k] = ROrder[j];
        ++j;
        ++k;
    }

    delete[] L;
    delete[] R;
    delete[] LOrder;
    delete[] ROrder;
}

void mergeSort(ln* degrees, ln* order, const ln& left, const ln& right) {
    if (left < right) {
        ln mid = left + (right - left) / 2;
        mergeSort(degrees, order, left, mid);
        mergeSort(degrees, order, mid + 1, right);
        merge(degrees, order, left, mid, right);
    }
}

void greedyColoring(Graph& gr, ln* order, ln* result, const ln& method, const ln& V, ln* degrees) {
    for (ln i = 0; i < V; ++i) {
        result[i] = -1;
    }

    if (method != 2) {  // Default method
        result[order[0]] = 0;  // Assign the first color to the first vertex

        // A temporary array to store the available colors. False value means color is available.
        bool* available = new bool[V];
        for (ln x = 0; x < V; ++x) {
            available[x] = true;
        }

        // Assign colors to remaining V-1 vertices
        for (ln i = 1; i < V; ++i) {
            ln u = order[i];
            // Process all adjacent vertices and mark their colors as unavailable
            for (ln x = 0; x < degrees[u]; ++x) {
                ln temp = gr[u][x];
                if (result[temp] != -1) {
                    available[result[temp]] = false;
                }
            }

            // Find the first available color
            ln cr;
            for (cr = 0; cr < V; ++cr) {
                if (available[cr]) {
                    break;
                }
            }

            result[u] = cr;  // Assign the found color to vertex u

            // Reset the values back to true for the next iteration

            for (ln x = 0; x < degrees[u]; ++x) {
                ln temp = gr[u][x];
                if (result[temp] != -1) {
                    available[result[temp]] = true;
                }
            }
        }
        delete[] available;
    }
}

ln countC4Subgraphs(const Graph& gr, const ln* degrees, const ln& V) {
    ln count = 0;
    for (ln u = 0; u < V; ++u) {
        for (ln i = 0; i < degrees[u]; ++i) {
            // u - wierzchołek
            // v - pierwszy sąsiad wierzchołka u
            ln v = gr[u][i];
            for (ln j = i + 1; j < degrees[u]; ++j) {
                // w - drugi sąsiad wierzchołka u
                ln w = gr[u][j];
                bool* neighbor_exists = new bool[V]();
                for (ln l = 0; l < degrees[w]; ++l) {
                    ln neighbor = gr[w][l];
                    if (neighbor != u) {
                        neighbor_exists[neighbor] = true;
                    }
                }
                for (ln k = 0; k < degrees[v]; ++k) {
                    ln neighbor = gr[v][k];
                    if (neighbor != u && neighbor_exists[neighbor]) {
                        ++count;
                    }
                }
                delete[] neighbor_exists;
            }
        }
    }
    return count / 4; // Each C4 cycle is counted 4 times
}

int main()
{
    ln graph_numbers;
    scanf_s("%lld", &graph_numbers);
    for (ln i = 0; i < graph_numbers; ++i) {
        ln edge_numbers;
        ln edges = 0;
        scanf_s("%lld", &edge_numbers);
        Graph gr = new ln*[edge_numbers];

        ln* degrees = new ln[edge_numbers];
        ln* degrees_for_sort = new ln[edge_numbers];
        ln* order = new ln[edge_numbers];
        ln* order_for_sort = new ln[edge_numbers];
        ln degrees_count = 0;
        ln degrees_sort_count = 0;
        for (ln j = 0; j < edge_numbers; ++j) {
            order[j] = j;
            order_for_sort[j] = j;
            ln neighbors;
            scanf_s("%lld", &neighbors);
            edges += neighbors;
            gr[j] = new ln[neighbors];
            degrees[degrees_count++] = neighbors;
            degrees_for_sort[degrees_sort_count++] = neighbors;
            ln neighbor_count = 0;
            for (ln k = 0; k < neighbors; ++k) {
                ln num;
                scanf_s("%lld", &num);
                gr[j][neighbor_count++] = currentNum(num);
            }
        }

        // 1. DEGREE
        mergeSort(degrees_for_sort, order_for_sort, 0, edge_numbers - 1);
        for (ln k = 0; k < edge_numbers; ++k) {
            printf("%llu ", degrees_for_sort[k]);
        }
        printf("\n");

        // 2. COMPONENTS (Liczba składowych spójności)
        printf("%llu\n", countComponents(gr, edge_numbers, degrees));
        // 3.
        printf("%s\n", isBipartite(gr, edge_numbers, degrees) ? "T" : "F");
        // 4.
        printf("?\n");
        // 5.
        printf("?\n");        // 6a.
        ln* result = new ln[edge_numbers];
        greedyColoring(gr, order, result, 0, edge_numbers, degrees);
        // PRINT COLORING RESULT
        for (ln u = 0; u < edge_numbers; ++u) {
            printf("%llu ", result[u] + 1);
        }
        printf("\n");

        // 6b.
        greedyColoring(gr, order_for_sort, result, 0, edge_numbers, degrees);
        // PRINT COLORING RESULT
        for (ln u = 0; u < edge_numbers; ++u) {
            printf("%llu ", result[u] + 1);
        }
        printf("\n");

        // 6c.
        printf("?\n");
        // 7.
        printf("%llu\n", countC4Subgraphs(gr, degrees, edge_numbers));
        // 8. liczba krawędzi dopełnienia grafu 
        printf("%llu\n", (edge_numbers * (edge_numbers - 1) / 2) - edges / 2);

        cout << "\n\n";

        delete[] result;
        delete[] order;
        delete[] order_for_sort;
        delete[] degrees;
        delete[] degrees_for_sort;
        for (ln x = 0; x < edge_numbers; ++x) {
            delete [] gr[x];
        }
        delete[] gr;
    }
}
