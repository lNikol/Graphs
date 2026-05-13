#include <iostream>
#include "List.h"
using namespace std;
using lli = long long int;
using ui = unsigned int;
using Graph = ui**;

bool colorDFS(const Graph& gr, char* color, const ui& start, ui* degrees) {
    List s;
    s.push_back(start);
    color[start] = 0;  // start coloring with 0

    while (s.begin() != nullptr) {
        Node* node = s.begin();

        for (ui x = 0; x < degrees[node->val]; ++x) {
            ui temp = gr[node->val][x];
            if (color[temp] == -1) {  // if not colored
                color[temp] = 1 - color[node->val];  // color with opposite color
                s.push_back(temp);

            }
            else if (color[temp] == color[node->val]) {
                // if the neighbor has the same color, the graph is not bipartite
                return false;

            }

        }
        s.deleteFirst();

    }
    return true;
}

bool isBipartite(const Graph& gr, const lli& size, ui* degrees) {
    char* color = new char[size];  // -1 means uncolored, 0 and 1 are the two colors
    for (ui x = 0; x < size; ++x) {
        color[x] = -1;

    }
    for (ui start = 0; start < size; ++start) {
        if (color[start] == -1) {  // not yet colored
            if (!colorDFS(gr, color, start, degrees)) {
                delete[] color;
                return false;

            }

        }

    }
    delete[] color;
    return true;
}

void DFS(Graph& gr, bool* visited, const lli& start, ui* degrees, ui& component_size, ui* component) {
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

ui countComponents(Graph& gr, const lli& size, ui* degrees, ui* component_sizes, ui* component) {
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

void merge(ui* degrees, ui* order, const lli& left, const lli& mid, const lli& right, const bool& graf) {
    lli n1 = mid - left + 1;
    lli n2 = right - mid;

    ui* L = new ui[n1];
    ui* R = new ui[n2];

    if (!graf) {
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
        lli k = left;
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

        delete[] LOrder;
        delete[] ROrder;

    }
    else {
        // preproccessing
        // degrees tutaj to lista sąsiadów wierzchołka czyli gr[wierzcholek]
        // order to degrees

        // Preprocessing Step: Sort the adjacency list N^+ (v) of each vertex v, such that the neighborhood
        // begins with N^− (v) (in arbitrary order), and then is followed by N^+ (v) (sorted in order of ≺)
        /*
        The vertices of the graph are assumed to have labels in [n] = {1, . . . , n}. We use uv as shorthand for an edge
        {u, v} when convenient. The neighborhood of a vertex v is given by N(v) = {u : uv ∈ E} and d(v) = |N (v)|
        is its degree. We define a total order of V by setting u ≺ v if d(u) < d(v); if d(u) = d(v), we break ties
        arbitrarily (e.g. by vertex ID)
        W kodzie break zrobiono na podstawie ID
        */

        for (ui i = 0; i < n1; ++i) {
            L[i] = degrees[left + i];

        }
        for (ui j = 0; j < n2; ++j) {
            R[j] = degrees[mid + 1 + j];

        }

        ui i = 0, j = 0;
        lli k = left;
        while (i < n1 && j < n2) {
            if (order[L[i]] < order[R[j]] ||
                (order[L[i]] == order[R[j]] && L[i] < R[j])) {
                degrees[k++] = L[i++];

            }
            else {
                degrees[k++] = R[j++];

            }

        }

        while (i < n1) {
            degrees[k++] = L[i++];

        }
        while (j < n2) {
            degrees[k++] = R[j++];

        }

    }

    delete[] L;
    delete[] R;
}

void mergeSort(ui* degrees, ui* order, const lli& left, const lli& right, const bool& graf) {
    if (left < right) {
        lli mid = left + (right - left) / 2;
        mergeSort(degrees, order, left, mid, graf);
        mergeSort(degrees, order, mid + 1, right, graf);
        merge(degrees, order, left, mid, right, graf);

    }
}

lli bfs(const Graph& gr, const ui* degrees, const lli& start, const lli& size, const lli& component_size) {
    unsigned short* distance = new unsigned short[size + 1]();
    ui* queue = new ui[size + 1];
    ui front = 0, back = 0;

    queue[back++] = start;
    distance[start] = 1;
    lli max_distance = 0;
    // back już = 1 : Startowy wierzchołek jest już w komponencie
    // wyżej to samo co i lli nodes_in_component = 1; - Startowy wierzchołek jest już w komponencie

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

void eccentricities(const Graph& gr, const ui* degrees, const lli& size, const ui* component, const ui* component_sizes) {
    for (ui i = 0; i < size; ++i) {
        if (component_sizes[component[i]] == 1) {
            printf("0 ");
            continue;

        }
        printf("%lld ", bfs(gr, degrees, i, size, component_sizes[component[i]]));

    }
    printf("\n");
}

void greedyColoring(Graph& gr, ui* order, ui* result, const char& method, const lli& V, ui* degrees) {
    for (ui i = 0; i < V; ++i) {
        result[i] = 0;

    }

    result[order[0]] = 1;  // Assign the first color to the first vertex

    // a temporary array to store the available colors. True value means color is available.
    bool* available = new bool[V];
    for (ui x = 0; x < V; ++x) {
        available[x] = true;

    }

    // assign colors to remaining V-1 vertices
    for (ui i = 1; i < V; ++i) {
        ui u = order[i];
        // process all adjacent vertices and mark their colors as unavailable
        for (ui x = 0; x < degrees[u]; ++x) {
            ui temp = gr[u][x];
            if (result[temp] != 0) {
                available[result[temp]] = false;

            }

        }

        // find the first available color
        ui cr;
        for (cr = 1; cr < V; ++cr) {
            if (available[cr]) {
                break;

            }

        }

        result[u] = cr;  // assign the found color to vertex u

        // reset the values back to true for the next iteration
        for (ui x = 0; x < degrees[u]; ++x) {
            ui temp = gr[u][x];
            if (result[temp] != 0) {
                available[result[temp]] = true;

            }

        }

    }
    delete[] available;
}

void mySwap(ui& a, ui& b) {
    ui temp = a;
    a = b;
    b = temp;
}

void sortNeighbors(Graph& gr, ui* degrees, const lli& size) {
    // first part of preprocessing
    for (ui i = 0; i < size; ++i) {
        if (degrees[i] > 1) {
            ui numNeighbors = degrees[i];
            int low = 0, high = numNeighbors - 1;
            while (low <= high) {
                if (degrees[gr[i][low]] < numNeighbors) {
                    ++low;

                }
                else if (degrees[gr[i][high]] >= numNeighbors) {
                    --high;

                }
                else {
                    mySwap(gr[i][low], gr[i][high]);
                    ++low;
                    --high;

                }

            }
            // sort the second part (neighbors with degrees >= degree of vertex)
            mergeSort(gr[i], degrees, low, numNeighbors - 1, true);

        }

    }
}

lli countC4Subgraphs(Graph& gr, ui* degrees, const lli& V) {
    // zrobiono algorytm 5 ze strony
    // https://arxiv.org/pdf/2303.06090
    // cały opis działania tego algorytmu jest na tej stronie

    sortNeighbors(gr, degrees, V);

    lli count = 0;
    unsigned short* L = new unsigned short[V]();

    for (ui v = 0; v < V; ++v) {
        ui tempCount = 0;
        for (ui i = 0; i < degrees[v]; ++i) {
            ui u = gr[v][i];
            if (degrees[u] > degrees[v]) {
                break;
            }
            if (degrees[u] == degrees[v] && u >= v) {
                continue;

            }
            ui j = 0;
            ui y = gr[u][j];
            while (y != v) {
                tempCount += L[y]++;
                y = gr[u][++j]; // ustawienie y na kolejnego sąsiada u

            }

        }

        for (ui i = 0; i < degrees[v]; ++i) {
            ui u = gr[v][i];
            if (degrees[u] > degrees[v]) {
                break;

            }
            if (degrees[u] == degrees[v] && u >= v) {
                continue;

            }
            ui j = 0;
            ui y = gr[u][j];
            while (y != v) {
                L[y] = 0;
                y = gr[u][++j]; // ustawienie y na kolejnego sąsiada u

            }
        }
        count += tempCount;

    }

    delete[] L;

    return count;
}

int main()
{
    unsigned short graph_numbers;
    scanf_s("%hu", &graph_numbers);
    for (ui i = 0; i < graph_numbers; ++i) {
        lli edge_numbers;
        lli edges = 0;
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

        // 1. Stopnie
        mergeSort(degrees_for_sort, order_for_sort, 0, edge_numbers - 1, false);
        for (ui k = 0; k < edge_numbers; ++k) {
            printf("%u ", degrees_for_sort[k]);

        }
        printf("\n");

        ui* component_sizes = new ui[edge_numbers + 1];
        ui* component = new ui[edge_numbers + 1];

        // tablica component przechowuje numery komponentów/składowych spójności
        // component[index] da numer komponentu/składowej spójności
        // tablica component_sizes przechowuje rozmiar komponentu/składowej spójności pod indeksem 'index'

        // 2. Liczba składowych spójności
        // Wyznaczam składowe spójności i ich rozmiary
        printf("%u\n", countComponents(gr, edge_numbers, degrees, component_sizes, component));
        // 3. Dwudzielność grafu        
        printf("%s\n", isBipartite(gr, edge_numbers, degrees) ? "T" : "F");

        // 4. Acentryczność wierzchołków
        eccentricities(gr, degrees, edge_numbers, component, component_sizes);
        // 5. Planarność
        printf("?\n");
        // 6a. Kolorowanie greedy
        ui* result = new ui[edge_numbers];
        greedyColoring(gr, order, result, 0, edge_numbers, degrees);

        for (ui u = 0; u < edge_numbers; ++u) {
            printf("%u ", result[u]);

        }
        printf("\n");

        // 6b. Kolorowanie LF
        greedyColoring(gr, order_for_sort, result, 0, edge_numbers, degrees);

        // print colloring result
        for (ui u = 0; u < edge_numbers; ++u) {
            printf("%u ", result[u]);

        }
        printf("\n");

        // 6c. Kolorowanie SLF
        printf("?\n");
        // 7. Cykli c4
        printf("%lld\n", countC4Subgraphs(gr, degrees, edge_numbers));
        // 8. Liczba krawędzi dopełnienia grafu 
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