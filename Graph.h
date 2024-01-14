#pragma once

#include <iostream>
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
	size_t v = 0;

	vector<forward_list<Arc>> Lgraph;

	Arc* findArc(int v1, int v2) { // Поиск ребра
		if (Lgraph.size() <= v1) { // Такой вершины нет
			return nullptr;
		}
		forward_list<Arc>& vertex = Lgraph[v1];
		auto iter = find_if(vertex.begin(), vertex.end(), [&v2](Arc arc) { return arc.to == v2; });
		if (iter == vertex.end()) { // Ребро не найдено
			return nullptr;
		}
		return &(*iter);
	}

	vector<pair<WeightType, vector<int>>> exstrisitetsForGraph() { // Поиск эксцентриситетов взвешенного графа
		vector<pair<WeightType, vector<int>>> exstrisitetsForGraph(v);
		for (int from = 0; from < Lgraph.size(); from++) { // Проходимся по всем вершинам
			queue<int> q;
			vector<WeightType> distance(v, numeric_limits<WeightType>::max()); // максимальное расстояние до вершин
			vector<vector<int>> paths(v); // путь с максимальным расстоянием до вершин
			vector<bool> visited(v, false);
			visited[from] = true;
			distance[from] = {};
			paths[from].push_back(from);
			q.push(from);
			while (!q.empty()) { // Пока не пройдемся по всем вершинам
				int v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!visited[arc.to] || distance[arc.to] > distance[v] + arc.weight) { // если вершину еще не посещали или посещали более долгим путем
						distance[arc.to] = distance[v] + arc.weight;
						paths[arc.to] = paths[v];
						paths[arc.to].push_back(arc.to);
						visited[arc.to] = true;
						q.push(arc.to);
					}
				}
			}
			for (int i = 0; i < Lgraph.size(); i++) {
				if (exstrisitetsForGraph[i].first < distance[i]) { //  максимальное значение в каждом столбце
					exstrisitetsForGraph[i] = make_pair(distance[i], paths[i]);
				}
			}
		}
		return exstrisitetsForGraph;
	}
public:
	Graph() {}
	Graph(size_t v);

	size_t V(); // опрос числа вершин в графе
	size_t E(); // опрос числа ребер в графе
	bool Insert(int v1, int v2);//	вставка ребра, соединяющего вершины v1, v2
	bool Delete(int v1, int v2); //	удаление ребра, соединяющего вершины v1, v2
	bool Edge(int v1, int v2); //	опрос наличия ребра, соединяющего вершины v1, v2
	bool SetEdge(int v1, int v2, WeightType weight); //	задание параметров ребра
	vector<int> Task(); //	решение задачи по варианту
	void Show(); // вывод графа на экран
};

template<class WeightType>
bool Graph<WeightType>::Insert(int v1, int v2) {
	if (Lgraph.size() <= v1 || v1 == v2 || Edge(v1, v2)) {
		return false;
	}
	Lgraph[v1].push_front(Arc(v2));
	e++;
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
vector<int> Graph<WeightType>::Task() {
	vector<pair<WeightType, vector<int>>> exstr = exstrisitetsForGraph();
	if (exstr.empty()) {
		throw exception("Пустой граф");
	}
	//for (int from = 0; from < exstr.size(); from++) {
	//	cout << from;
	//	cout << " " << exstr[from].first << ": " << exstr[from].second;
	//	cout << endl;
	//}
	auto radius = min_element( // радиус – минимальный эксцентриситет в графе
		exstr.begin(), exstr.end(),
		[](const pair<WeightType, vector<int>>& p1, const pair<WeightType, vector<int>>& p2) {return p1.first < p2.first; });
	return radius->second;
}

template<class WeightType>
void Graph<WeightType>::Show() {
	for (int i = 0; i < Lgraph.size(); i++) {
		cout << i;
		for (const Arc& arc : Lgraph[i]) {
			cout << " " << arc.to << "," << arc.weight;
		}
		cout << endl;
	}
}

template<class WeightType>
Graph<WeightType>::Graph(size_t v){
	this->v = v;
	Lgraph.clear();
	Lgraph.resize(v, {});
}

template<class WeightType>
size_t Graph<WeightType>::V() {
	return v;
}

template<class WeightType>
size_t Graph<WeightType>::E() {
	return e;
}
