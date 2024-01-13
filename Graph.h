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
	class Arc { // �����
	public:
		int to; // �������� �������
		WeightType weight{}; // ��� �����

		Arc(int v) : to(v) {}
	};

	size_t e = 0; // ���������� �����

	map<int, forward_list<Arc>> Lgraph;

	Arc* findArc(int v1, int v2) { // ����� �����
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

	map<int, pair<WeightType, string>> exstrisitetsForGraph() { // ����� ���������������� ����������� �����
		map<int, pair<WeightType, string>> exstrisitetsForGraph;
		for (const pair<int, forward_list<Arc>>& vertex : Lgraph) { // ���������� �� ���� ��������
			const int& from = vertex.first; // ��������������� �������
			queue<int> q;
			map<int, WeightType> distance; // ������������ ���������� �� ������
			map<int, string> paths; // ���� � ������������ ����������� �� ������
			map<int, bool> visited;
			for (const pair<int, const forward_list<Arc>>& vert : Lgraph) {
				distance[vert.first] = numeric_limits<WeightType>::max();
				visited[vert.first] = false;
			}
			visited[from] = true;
			distance[from] = {};
			paths[from] = to_string(from);
			q.push(from);
			while (!q.empty()) { // ���� �� ��������� �� ���� ��������
				int v = q.front();
				q.pop();
				for (const Arc& arc : Lgraph[v]) {
					if (!visited[arc.to] || distance[arc.to] > distance[v] + arc.weight) { // ���� ������� ��� �� �������� ��� �������� ����� ������ �����
						distance[arc.to] = distance[v] + arc.weight;
						paths[arc.to] = paths[v] + " " + to_string(arc.to);
						visited[arc.to] = true;
						q.push(arc.to);
					}
				}
			}
			for (const pair<int, const forward_list<Arc>>& vert : Lgraph) {
				const int& currentVert = vert.first;
				if (!exstrisitetsForGraph.count(currentVert) || exstrisitetsForGraph[currentVert].first < distance[currentVert]) { //  ������������ �������� � ������ �������
					exstrisitetsForGraph[currentVert] = make_pair(distance[currentVert], paths[currentVert]);
				}
			}
		}
		return exstrisitetsForGraph;
	}
public:
	Graph() {}

	size_t V(); // ����� ����� ������ � �����
	size_t E(); // ����� ����� ����� � �����
	bool Insert(int v1, int v2);//	������� �����, ������������ ������� v1, v2
	bool Delete(int v1, int v2); //	�������� �����, ������������ ������� v1, v2
	bool Edge(int v1, int v2); //	����� ������� �����, ������������ ������� v1, v2
	bool SetEdge(int v1, int v2, WeightType weight); //	������� ���������� �����
	void Task(); //	������� ������ �� ��������
	void Show(); // ����� ����� �� �����
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
		throw exception("������ ����");
	}
	//for (const pair<int, pair<WeightType, string>>& vertex : exstr) {
	//	cout << vertex.first;
	//	cout << " " << vertex.second.first << ": " << vertex.second.second;
	//	cout << endl;
	//}
	pair<WeightType, string> radius = min_element( // ������ � ����������� �������������� � �����
		exstr.begin(), exstr.end(),
		[](const pair<int, pair<WeightType, string>>& p1, const pair<int, pair<WeightType, string>>& p2) {return p1.second.first < p2.second.first; })->second;
	cout << "P�����: " << radius.first << endl;
	cout << "����: " << radius.second << endl;
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
