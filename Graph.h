#pragma once

#include <iostream>
#include <string>
#include <map>
#include <forward_list>
#include <queue>

using namespace std;

template<class WeightType>
class Graph {
private:
	class Arc { // Ребро
	public:
		int to; // Конечная вершина
		WeightType weight{}; // Вес ребра

		Arc(int v) : to(v) {}
	};

	size_t e = 0; // Количество ребер

	map<int, forward_list<Arc>> Lgraph;

	Arc* findArc(int v1, int v2) { // Поиск ребра
		if (!Lgraph.count(v1)) { // Такой вершины нет
			return nullptr;
		}
		forward_list<Arc>& vertex = Lgraph[v1];
		auto iter = find_if(vertex.begin(), vertex.end(), [&v2](Arc arc) { return arc.to == v2; });
		if (iter == vertex.end()) { // Ребро не найдено
			return nullptr;
		}
		return &(*iter);
	}

	map<int, pair<WeightType, string>> exstrisitetsForGraph() { // Поиск эксцентриситетов взвешенного графа
		map<int, pair<WeightType, string>> exstrisitetsForGraph;
		for (const pair<int, forward_list<Arc>>& vertex : Lgraph) { // Проходимся по всем вершинам
			const int& from = vertex.first; // рассматриваемая вершина
			queue<int> q;
			map<int, WeightType> distance; // максимальное расстояние до вершин
			map<int, string> paths; // путь с максимальным расстоянием до вершин
			map<int, bool> visited;
			for (const pair<int, const forward_list<Arc>>& vert : Lgraph) {
				distance[vert.first] = numeric_limits<WeightType>::max();
				visited[vert.first] = false;
			}
			visited[from] = true;
			distance[from] = {};
			paths[from] = to_string(from);
			q.push(from);
			while (!q.empty()) { // Пока не пройдемся по всем вершинам
				int v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!visited[arc.to] || distance[arc.to] > distance[v] + arc.weight) { // если вершину еще не посещали или посещали более долгим путем
						distance[arc.to] = distance[v] + arc.weight;
						paths[arc.to] = paths[v] + " " + to_string(arc.to);
						visited[arc.to] = true;
						q.push(arc.to);
					}
				}
			}
			for (const pair<int, const forward_list<Arc>>& vert : Lgraph) {
				const int& currentVert = vert.first;
				if (!exstrisitetsForGraph.count(currentVert) || exstrisitetsForGraph[currentVert].first < distance[currentVert]) { //  максимальное значение в каждом столбце
					exstrisitetsForGraph[currentVert] = make_pair(distance[currentVert], paths[currentVert]);
				}
			}
		}
		return exstrisitetsForGraph;
	}
public:
	Graph() {}

	size_t V(); // опрос числа вершин в графе
	size_t E(); // опрос числа ребер в графе
	bool Insert(int v1, int v2);//	вставка ребра, соединяющего вершины v1, v2
	bool Delete(int v1, int v2); //	удаление ребра, соединяющего вершины v1, v2
	bool Edge(int v1, int v2); //	опрос наличия ребра, соединяющего вершины v1, v2
	bool SetEdge(int v1, int v2, WeightType weight); //	задание параметров ребра
	void Task(); //	решение задачи по варианту
	void Show(); // вывод графа на экран
};

template<class WeightType>
bool Graph< WeightType>::Insert(int v1, int v2) {
	if (v1 == v2 || Edge(v1, v2)) {
		return false;
	}
	Lgraph[v1].push_front(Arc(v2));
	e++;
	if (!Lgraph.count(v2)) {
		Lgraph[v2] = {};
	}
	return true;
}

template<class WeightType>
bool Graph<WeightType>::Delete(int v1, int v2) {
	if (!Edge(v1, v2)) {
		return false;
	}
	forward_list<Arc>& vertex = Lgraph[v1];
	vertex.remove_if([&v2](Arc arc) { return arc.to == v2; });
	e--;
	if (vertex.empty()) {
		Lgraph.erase(v1);
	}
	return true;
}

template<class WeightType>
bool Graph<WeightType>::Edge(int v1, int v2) {
	return findArc(v1, v2) != nullptr;
}

template<class WeightType>
bool Graph<WeightType>::SetEdge(int v1, int v2, WeightType weight) {
	Arc* arc = findArc(v1, v2);
	if (arc == nullptr) {
		return false;
	}
	arc->weight = weight;
	return true;
}

template<class WeightType>
void Graph<WeightType>::Task() {
	map<int, pair<WeightType, string>> exstr = exstrisitetsForGraph();
	if (exstr.empty()) {
		throw exception("Пустой граф");
	}
	//for (const pair<int, pair<WeightType, string>>& vertex : exstr) {
	//	cout << vertex.first;
	//	cout << " " << vertex.second.first << ": " << vertex.second.second;
	//	cout << endl;
	//}
	pair<WeightType, string> radius = min_element( // радиус – минимальный эксцентриситет в графе
		exstr.begin(), exstr.end(),
		[](const pair<int, pair<WeightType, string>>& p1, const pair<int, pair<WeightType, string>>& p2) {return p1.second.first < p2.second.first; })->second;
	cout << "Pадиус: " << radius.first << endl;
	cout << "Путь: " << radius.second << endl;
}

template<class WeightType>
void Graph<WeightType>::Show() {
	for (const pair<int, const forward_list<Arc>>& vertex : Lgraph) {
		cout << vertex.first;
		for (const Arc& arc : vertex.second) {
			cout << " " << arc.to << "," << arc.weight;
		}
		cout << endl;
	}
}

template<class WeightType>
size_t Graph<WeightType>::V() {
	return Lgraph.size();
}

template<class WeightType>
size_t Graph<WeightType>::E() {
	return e;
}
