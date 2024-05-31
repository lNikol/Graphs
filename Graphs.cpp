#include <iostream>
#include "List.h"
using namespace std;
using ln = long long int;
using ui = unsigned int;
using Graph = ui**;

bool colorDFS(const Graph& gr, char* color, const ui& start, ui* degrees) {
    List s;
    s.push_back(start);
    color[start] = 0;  // Start coloring with 0

    while (s.begin() != nullptr) {
        Node* node = s.begin();

        for (ui x = 0; x < degrees[node->val]; ++x) {
            ui temp = gr[node->val][x];
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

bool isBipartite(const Graph& gr, const ln& size, ui* degrees) {
    char* color = new char[size];  // -1 means uncolored, 0 and 1 are the two colors
    for (ui x = 0; x < size; ++x) {
        color[x] = -1;
    }
    for (ui start = 0; start < size; ++start) {
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

void DFS(Graph& gr, bool* visited, const ln& start, ui* degrees, ui& component_size, ui* component) {
    List stack;
    stack.push_back(start);

    while (stack.begin() != nullptr) {
        Node* node = stack.begin();
        if (!visited[node->val]) {
            visited[node->val] = true;
            component[node->val] = start;  // oznaczam wierzchołek jako należący do składowej start
            ++component_size;
            for (ui x = 0; x < degrees[node->val]; ++x) {
                ui temp = gr[node->val][x];
                if (!visited[temp]) {
                    stack.push_back(temp);
                }
            }
        }
        stack.deleteFirst();
    }
}

ui countComponents(Graph& gr, const ln& size, ui* degrees, ui* component_sizes, ui* component) {
    ui components = 0;
    bool* visited = new bool[size]();

    for (ui i = 0; i < size; ++i) {
        if (!visited[i]) {
            ui component_size = 0;
            DFS(gr, visited, i, degrees, component_size, component);
            component_sizes[i] = component_size;
            ++components;
        }
    }

    delete[] visited;
    return components;
}

void merge(ui* degrees, ui* order, const ln& left, const ln& mid, const ln& right) {
    ln n1 = mid - left + 1;
    ln n2 = right - mid;

    ui* L = new ui[n1];
    ui* R = new ui[n2];
    ui* LOrder = new ui[n1];
    ui* ROrder = new ui[n2];

    for (ui i = 0; i < n1; ++i) {
        L[i] = degrees[left + i];
        LOrder[i] = order[left + i];
    }
    for (ui j = 0; j < n2; ++j) {
        R[j] = degrees[mid + 1 + j];
        ROrder[j] = order[mid + 1 + j];
    }

    ui i = 0, j = 0;
    ln k = left;
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

void mergeSort(ui* degrees, ui* order, const ln& left, const ln& right) {
    if (left < right) {
        ln mid = left + (right - left) / 2;
        mergeSort(degrees, order, left, mid);
        mergeSort(degrees, order, mid + 1, right);
        merge(degrees, order, left, mid, right);
    }
}

ln bfs(const Graph& gr, const ui* degrees, const ln& start, const ln& size, const ln& component_size) {
    unsigned short* distance = new unsigned short[size + 1]();
    ui* queue = new ui[size + 1];
    ui front = 0, back = 0;

    queue[back++] = start;
    distance[start] = 1;
    ln max_distance = 0;
    // back już = 1 : Startowy wierzchołek jest już w komponencie
    // to samo co i
    // ln nodes_in_component = 1; // Startowy wierzchołek jest już w komponencie

    while (front != back) {
        ui node = queue[front++];
        for (ui i = 0; i < degrees[node]; ++i) {
            ui neighbor = gr[node][i];
            if (distance[neighbor] == 0) {
                distance[neighbor] = distance[node] + 1;
                queue[back++] = neighbor;
                if (distance[neighbor] > max_distance) {
                    max_distance = distance[neighbor];
                }
                // jeśli dodałem wszystkie wierzchołki składowej spójności, mogę przerwać
                if (back == component_size) {
                    delete[] queue;
                    delete[] distance;
                    return --max_distance;
                }
            }
        }
    }
    delete[] queue;
    delete[] distance;
    return --max_distance;
}

void eccentricities(const Graph& gr, const ui* degrees, const ln& size, const ui* component, const ui* component_sizes) {
    for (ui i = 0; i < size; ++i) {
        if (component_sizes[component[i]] == 1) {
            printf("0 ");
            continue;
        }
        printf("%lld ", bfs(gr, degrees, i, size, component_sizes[component[i]]));
    }
    printf("\n");
}

void greedyColoring(Graph& gr, ui* order, ui* result, const char& method, const ln& V, ui* degrees) {
    for (ui i = 0; i < V; ++i) {
        result[i] = 0;
    }

    if (method != 2) {  // Default method
        result[order[0]] = 1;  // Assign the first color to the first vertex

        // A temporary array to store the available colors. False value means color is available.
        bool* available = new bool[V];
        for (ui x = 0; x < V; ++x) {
            available[x] = true;
        }

        // Assign colors to remaining V-1 vertices
        for (ui i = 1; i < V; ++i) {
            ui u = order[i];
            // Process all adjacent vertices and mark their colors as unavailable
            for (ui x = 0; x < degrees[u]; ++x) {
                ui temp = gr[u][x];
                if (result[temp] != 0) {
                    available[result[temp]] = false;
                }
            }

            // Find the first available color
            ui cr;
            for (cr = 1; cr < V; ++cr) {
                if (available[cr]) {
                    break;
                }
            }

            result[u] = cr;  // Assign the found color to vertex u

            // Reset the values back to true for the next iteration

            for (ui x = 0; x < degrees[u]; ++x) {
                ui temp = gr[u][x];
                if (result[temp] != 0) {
                    available[result[temp]] = true;
                }
            }
        }
        delete[] available;
    }
}

// dodać konetarze w C4 i innych fragmentach kodu, usunąć stare i zbędne komentarze
ln countC4Subgraphs(const Graph& gr, const ui* degrees, const ln& V) {
    ln count = 0;

    // Iteracja po wszystkich wierzchołkach
    for (ui u = 0; u < V; ++u) {
        ui tempCount = 0;
        if (degrees[u] <= 1) {
            continue;
        }

        // Iteracja po sąsiadach wierzchołka u
        for (ui i = 0; i < degrees[u]; ++i) {
            // v - pierwszy sąsiad u
            ui v = gr[u][i];
            if (degrees[v] <= 1) {
                continue;
            }

            for (ui j = i + 1; j < degrees[u]; ++j) {
                // w - drugi sąsiad u
                ui w = gr[u][j];
                if (degrees[w] <= 1) {
                    continue;
                }
                ui t1 = 0, t2 = 0;
                while (t1 < degrees[v] && t2 < degrees[w]) {
                    if (gr[v][t1] == gr[w][t2]) {
                        ++tempCount;
                        ++t1;
                        ++t2;
                    }
                    else if (gr[v][t1] < gr[w][t2]) {
                        ++t1;
                    }
                    else {
                        ++t2;
                    }
                }
            }
        }
        count += tempCount;
    }

    return count / 2; // Każdy cykl C4 jest liczony 4 razy
}

int main()
{
    unsigned short graph_numbers;
    scanf_s("%hu", &graph_numbers);
    for (ui i = 0; i < graph_numbers; ++i) {
        ln edge_numbers;
        ln edges = 0;
        scanf_s("%lld", &edge_numbers);
        Graph gr = new ui * [edge_numbers];

        ui* degrees = new ui[edge_numbers];
        ui* degrees_for_sort = new ui[edge_numbers];
        ui* order = new ui[edge_numbers];
        ui* order_for_sort = new ui[edge_numbers];
        for (ui j = 0; j < edge_numbers; ++j) {
            order[j] = j;
            order_for_sort[j] = j;
            ui neighbors;
            scanf_s("%u", &neighbors);
            edges += neighbors;
            gr[j] = new ui[neighbors];
            degrees[j] = neighbors;
            degrees_for_sort[j] = neighbors;
            for (ui k = 0; k < neighbors; ++k) {
                ui n;
                scanf_s("%u", &n);
                gr[j][k] = n - 1 >= 0 ? n - 1 : n;
            }
        }

        // 1. DEGREE
        mergeSort(degrees_for_sort, order_for_sort, 0, edge_numbers - 1);
        for (ui k = 0; k < edge_numbers; ++k) {
            printf("%u ", degrees_for_sort[k]);
        }
        printf("\n");

        // Tablica przechowująca rozmiary składowych spójności
        ui* component_sizes = new ui[edge_numbers + 1];
        ui* component = new ui[edge_numbers + 1];

        // 2. COMPONENTS (Liczba składowych spójności)
        // Wyznaczam składowe spójności i ich rozmiary
        printf("%u\n", countComponents(gr, edge_numbers, degrees, component_sizes, component));
        // 3.
        printf("%s\n", isBipartite(gr, edge_numbers, degrees) ? "T" : "F");

        // 4. Acentryczność wierzchołków
        eccentricities(gr, degrees, edge_numbers, component, component_sizes);
        // 5.
        printf("?\n");
        // 6a.
        ui* result = new ui[edge_numbers];
        greedyColoring(gr, order, result, 0, edge_numbers, degrees);
        // PRINT COLORING RESULT
        for (ui u = 0; u < edge_numbers; ++u) {
            printf("%u ", result[u]);
        }
        printf("\n");

        // 6b.
        greedyColoring(gr, order_for_sort, result, 0, edge_numbers, degrees);
        
        // print colloring result
        for (ui u = 0; u < edge_numbers; ++u) {
            printf("%u ", result[u]);
        }
        printf("\n");

        // 6c.
        printf("?\n");
        // 7.
        printf("%lld\n", countC4Subgraphs(gr, degrees, edge_numbers));
        // 8. liczba krawędzi dopełnienia grafu 
        printf("%lld\n", (edge_numbers * (edge_numbers - 1) / 2) - edges / 2);

        for (ui x = 0; x < edge_numbers; ++x) {
            delete[] gr[x];
        }
        delete[] gr;
        delete[] result;
        delete[] order;
        delete[] order_for_sort;
        delete[] degrees;
        delete[] degrees_for_sort;
        delete[] component;
        delete[] component_sizes;
    }
}