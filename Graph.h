#pragma once

#include <iostream>
#include <string> 
#include <map>
#include <forward_list>
#include <queue>

using namespace std;

template<class VertexType, class WeightType>
class Graph {
private:
	class Arc { // Ребро
	public:
		VertexType to; // Конечная вершина
		WeightType weight; // Вес ребра
		bool isWeightSet = false; // Был ли установлен вес

		Arc(VertexType v) : to(v) {}
	};

	size_t e = 0; // Количество ребер

	map<VertexType, forward_list<Arc>> Lgraph;

	Arc* findArc(VertexType v1, VertexType v2) { // Поиск ребра
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

	map<WeightType, queue<VertexType>> exstrisitetsForGraph() { // Поиск эксцентриситетов взвешенного графа
		map<WeightType, queue<VertexType>> exstrisitetsForGraph;
		for (const pair<VertexType, forward_list<Arc>>& vertex : Lgraph) { // Проходимся по всем вершинам
			const VertexType& from = vertex.first; // рассматриваемая вершина
			map<VertexType, queue<VertexType>> paths; // максимальный путь до вершин
			queue<VertexType> q;
			map<VertexType, WeightType> distance; // максимальное расстояние до вершин
			paths[from].push(from);
			q.push(from);
			while (!q.empty()) { // Пока не пройдемся по всем вершинам
				VertexType v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!distance.count(arc.to) || distance[arc.to] < distance[v] + arc.weight) { // если вершину еще не посещали или посещали более коротким путем
						if (!distance.count(arc.to)) {
							distance[arc.to] = arc.weight;
						}
						else {
							distance[arc.to] = distance[v] + arc.weight;
						}
						paths[arc.to] = paths[v];
						paths[arc.to].push(arc.to); // Добавляем в пройденный путь
						q.push(arc.to);
					}
				}
			}
			if (!distance.empty()) { // Когда из данной вершины можно посетить хотя бы еще одну
				VertexType max_vertex = max_element(distance.begin(), distance.end(), [](const pair<VertexType, WeightType>& lhs, const pair<VertexType, WeightType>& rhs) {return lhs.second < rhs.second; })->first;
				exstrisitetsForGraph.insert(make_pair(distance[max_vertex], paths[max_vertex])); // максимальное расстояние
			}
		}
		return exstrisitetsForGraph;
	}
public:
	Graph() {}

	size_t V(); // опрос числа вершин в графе,
	size_t E();// опрос числа ребер в графе,
	void Insert(VertexType v1, VertexType v2);//	вставка ребра, соединяющего вершины v1, v2,
	void Delete(VertexType v1, VertexType v2);//	удаление ребра, соединяющего вершины v1, v2,
	bool Edge(VertexType v1, VertexType  v2);//	опрос наличия ребра, соединяющего вершины v1, v2,
	void SetEdge(VertexType v1, VertexType v2, WeightType weight);//	задание параметров ребра,
	void Task(); //	решение задачи по варианту
	void Show(); // вывод графа на экран
};

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Insert(VertexType v1, VertexType v2) {
	if (v1 == v2) {
		throw exception("Не допускается вставка петель");
	}
	if (Edge(v1, v2)) {
		throw exception("Не допускается вставка параллельных ребер");
	}
	Lgraph[v1].push_front(Arc(v2));
	e++;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Delete(VertexType v1, VertexType v2) {
	if (!Edge(v1, v2)) {
		throw exception("Нет такого ребра");
	}
	forward_list<Arc>& vertex = Lgraph[v1];
	vertex.remove_if([&v2](Arc arc) { return arc.to == v2; });
	e--;
	if (vertex.empty()) {
		Lgraph.erase(v1);
	}
}

template<class VertexType, class WeightType>
bool Graph<VertexType, WeightType>::Edge(VertexType v1, VertexType v2) {
	return findArc(v1, v2) != nullptr;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::SetEdge(VertexType v1, VertexType v2, WeightType weight) {
	Arc* arc = findArc(v1, v2);
	if (arc == nullptr) {
		throw exception("Нет такого ребра");
	}
	arc->weight = weight;
	arc->isWeightSet = true;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Task() {
	map<WeightType, queue<VertexType>> exstr = exstrisitetsForGraph();
	if (exstr.empty()) {
		throw exception("Пустой граф");
	}
	pair<WeightType, queue<VertexType>> radius = *exstr.begin();
	cout << "Pадиус: " << radius.first << endl;
	cout << "Путь: ";
	while (!radius.second.empty()) {
		cout << radius.second.front() << " ";
		radius.second.pop();
	}
	cout << endl;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Show() {
	for (const pair<VertexType, forward_list<Arc>>& vertex : Lgraph) {
		cout << vertex.first;
		for (const Arc& arc : vertex.second) {
			cout << " " << arc.to;
			if (arc.isWeightSet) {
				cout << "," << arc.weight;
			}
		}
		cout << endl;
	}
}

template<class VertexType, class WeightType>
size_t Graph<VertexType, WeightType>::V() {
	return Lgraph.size();
}

template<class VertexType, class WeightType>
size_t Graph<VertexType, WeightType>::E() {
	return e;
}
