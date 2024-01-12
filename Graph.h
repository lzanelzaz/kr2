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
	class Arc { // �����
	public:
		VertexType to; // �������� �������
		WeightType weight; // ��� �����
		bool isWeightSet = false; // ��� �� ���������� ���

		Arc(VertexType v) : to(v) {}
	};

	size_t e = 0; // ���������� �����

	map<VertexType, forward_list<Arc>> Lgraph;

	Arc* findArc(VertexType v1, VertexType v2) { // ����� �����
		if (!Lgraph.count(v1)) { // ����� ������� ���
			return nullptr;
		}
		forward_list<Arc>& vertex = Lgraph[v1];
		auto iter = find_if(vertex.begin(), vertex.end(), [&v2](Arc arc) { return arc.to == v2; });
		if (iter == vertex.end()) { // ����� �� �������
			return nullptr;
		}
		return &(*iter);
	}

	map<WeightType, queue<VertexType>> exstrisitetsForGraph() { // ����� ���������������� ����������� �����
		map<WeightType, queue<VertexType>> exstrisitetsForGraph;
		for (const pair<VertexType, forward_list<Arc>>& vertex : Lgraph) { // ���������� �� ���� ��������
			const VertexType& from = vertex.first; // ��������������� �������
			map<VertexType, queue<VertexType>> paths; // ������������ ���� �� ������
			queue<VertexType> q;
			map<VertexType, WeightType> distance; // ������������ ���������� �� ������
			paths[from].push(from);
			q.push(from);
			while (!q.empty()) { // ���� �� ��������� �� ���� ��������
				VertexType v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!distance.count(arc.to) || distance[arc.to] < distance[v] + arc.weight) { // ���� ������� ��� �� �������� ��� �������� ����� �������� �����
						if (!distance.count(arc.to)) {
							distance[arc.to] = arc.weight;
						}
						else {
							distance[arc.to] = distance[v] + arc.weight;
						}
						paths[arc.to] = paths[v];
						paths[arc.to].push(arc.to); // ��������� � ���������� ����
						q.push(arc.to);
					}
				}
			}
			if (!distance.empty()) { // ����� �� ������ ������� ����� �������� ���� �� ��� ����
				VertexType max_vertex = max_element(distance.begin(), distance.end(), [](const pair<VertexType, WeightType>& lhs, const pair<VertexType, WeightType>& rhs) {return lhs.second < rhs.second; })->first;
				exstrisitetsForGraph.insert(make_pair(distance[max_vertex], paths[max_vertex])); // ������������ ����������
			}
		}
		return exstrisitetsForGraph;
	}
public:
	Graph() {}

	size_t V(); // ����� ����� ������ � �����,
	size_t E();// ����� ����� ����� � �����,
	void Insert(VertexType v1, VertexType v2);//	������� �����, ������������ ������� v1, v2,
	void Delete(VertexType v1, VertexType v2);//	�������� �����, ������������ ������� v1, v2,
	bool Edge(VertexType v1, VertexType  v2);//	����� ������� �����, ������������ ������� v1, v2,
	void SetEdge(VertexType v1, VertexType v2, WeightType weight);//	������� ���������� �����,
	void Task(); //	������� ������ �� ��������
	void Show(); // ����� ����� �� �����
};

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Insert(VertexType v1, VertexType v2) {
	if (v1 == v2) {
		throw exception("�� ����������� ������� ������");
	}
	if (Edge(v1, v2)) {
		throw exception("�� ����������� ������� ������������ �����");
	}
	Lgraph[v1].push_front(Arc(v2));
	e++;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Delete(VertexType v1, VertexType v2) {
	if (!Edge(v1, v2)) {
		throw exception("��� ������ �����");
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
		throw exception("��� ������ �����");
	}
	arc->weight = weight;
	arc->isWeightSet = true;
}

template<class VertexType, class WeightType>
void Graph<VertexType, WeightType>::Task() {
	map<WeightType, queue<VertexType>> exstr = exstrisitetsForGraph();
	if (exstr.empty()) {
		throw exception("������ ����");
	}
	pair<WeightType, queue<VertexType>> radius = *exstr.begin();
	cout << "P�����: " << radius.first << endl;
	cout << "����: ";
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
