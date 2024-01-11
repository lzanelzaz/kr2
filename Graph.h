#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

template<class WeightType>
class Graph {
private:

	class Arc {
	public:
		int to;
		WeightType weight;

		Arc(int v) : to(v) {}
	};

	Arc* findArc(int v1, int v2) {
		if (!Lgraph.count(v1)) {
			return nullptr;
		}
		vector<Arc>& vertex = Lgraph[v1];
		auto iter = find_if(vertex.begin(), vertex.end(), [&v2](Arc arc) { return arc.to == v2; });
		if (iter == vertex.end()) {
			return nullptr;
		}
		return &(*iter);
	}

	map<WeightType, vector<int>> exstrisitetsForGraph() {
		map<WeightType, vector<int>> exstrisitetsForGraph;
		for (const pair<int, vector<Arc>>& vertex : Lgraph) {
			const int& from = vertex.first;
			map<int, vector<int>> paths;
			queue<int> q;
			map<int, WeightType> distance;
			paths[from] = { from };
			q.push(from);
			while (!q.empty()) {
				int v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!distance.count(v) || !distance.count(arc.to) || distance[arc.to] < distance[v] + arc.weight) {
						if (!distance.count(arc.to)) {
							distance[arc.to] = arc.weight;
						}
						else {
							distance[arc.to] = distance[v] + arc.weight;
						}
						paths[arc.to] = paths[v];
						paths[arc.to].push_back(arc.to);
						q.push(arc.to);
					}
				}
			}
			if (!distance.empty()) {
				int max_vertex = max_element(distance.begin(), distance.end(), [](const pair<int, WeightType>& lhs, const pair<int, WeightType>& rhs) {return lhs.second < rhs.second; })->first;
				exstrisitetsForGraph.insert(make_pair(distance[max_vertex], paths[max_vertex]));
			}
		}
		return exstrisitetsForGraph;
	}

	map<int, vector<Arc>> Lgraph;

	size_t e = 0;
public:
	Graph() {}

	size_t V(); // опрос числа вершин в графе,
	size_t E();// опрос числа ребер в графе,
	void Insert(int v1, int v2);//	вставка ребра, соединяющего вершины v1, v2,
	void Delete(int v1, int v2);//	удаление ребра, соединяющего вершины v1, v2,
	bool Edge(int v1, int  v2);//	опрос наличия ребра, соединяющего вершины v1, v2,
	void SetEdge(int v1, int v2, WeightType weight);//	задание параметров ребра,
	void Task();//		решение задачи по варианту
	void Show();
};

template<class WeightType>
void Graph<WeightType>::Insert(int v1, int v2) {
	if (v1 == v2) {
		throw exception("Не допускается вставка петель");
	}
	if (Edge(v1, v2)) {
		throw exception("Не допускается вставка параллельных ребер");
	}
	Lgraph[v1].push_back(Arc(v2));
	e++;
}

template<class WeightType>
void Graph<WeightType>::Delete(int v1, int v2) {
	if (!Edge(v1, v2)) {
		throw exception("Нет такого ребра");
	}
	vector<Arc>& vertex = Lgraph[v1];
	auto arcIt = find_if(vertex.begin(), vertex.end(), [&v2](Arc arc) { return arc.to == v2; });

	vertex.erase(arcIt);
	e--;
	if (vertex.empty()) {
		Lgraph.erase(v1);
	}
}

template<class WeightType>
bool Graph<WeightType>::Edge(int v1, int v2) {
	return findArc(v1, v2) != nullptr;
}

template<class WeightType>
void Graph<WeightType>::SetEdge(int v1, int v2, WeightType weight) {
	Arc* arc = findArc(v1, v2);
	if (arc == nullptr) {
		throw exception("Нет такого ребра");
	}
	arc->weight = weight;
}

template<class WeightType>
void Graph<WeightType>::Task() {
	map<WeightType, vector<int>> exstr = exstrisitetsForGraph();
	if (exstr.empty()) {
		throw exception("Пустой граф");
	}
	pair<WeightType, vector<int>> radius = *exstr.begin();
	cout << "Pадиус: " << radius.first << endl;
	cout << "Путь: ";
	for (const int& v : radius.second) {
		cout << v << " ";
	}
	cout << endl;
}

template<class WeightType>
void Graph<WeightType>::Show() {
	for (const pair<int, vector<Arc>>& vertex : Lgraph) {
		cout << vertex.first << " ";
		for (const Arc& arc : vertex.second) {
			cout << arc.to << "," << arc.weight << " ";
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
