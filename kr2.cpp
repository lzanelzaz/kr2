#include <iostream>
#include "graph.h"

using namespace std;

//Вариант 8
//Реализация АТД «Взвешенный орграф». Граф представлен в виде списков смежности(L - граф).
//Определение радиуса и определения списка вершин для соответствующего радиусу пути на основе алгоритма Дейкстры. 
//(радиус – минимальный эксцентриситет в графе, путь - последовательность вершин, лежащих на пути с суммарным весом ребер, равным радиусу).

int main() {
	setlocale(LC_ALL, "Russian");
	Graph<int> g = Graph<int>(0);
	int n;
	while (true) {
		cout << endl << "Меню. Выберите действие:" << endl << endl;
		cout << "0. Создать граф" << endl;
		cout << "1. Вставка" << endl;
		cout << "2. Удаление" << endl;
		cout << "3. Число вершин" << endl;
		cout << "4. Число ребер" << endl;
		cout << "5. Опрос наличия ребра" << endl;
		cout << "6. Задание параметров ребра" << endl;
		cout << "7. Вывод структуры графа на экран" << endl;
		cout << "8. Определение центра и списка вершин для соответствующего радиусу пути на основе алгоритма Дейкстры" << endl;
		cout << endl;
		cin >> n;
		cout << endl;
		try {
			switch (n) {
			case 0: {
				int v;
				cin >> v;
				g = Graph<int>(v);
				break;
			}
			case 1: {
				int v1, v2;
				cin >> v1 >> v2;
				cout << g.Insert(v1, v2) << endl;
				break;
			}
			case 2: {
				int v1, v2;
				cin >> v1 >> v2;
				cout << g.Delete(v1, v2) << endl;
				break;
			}
			case 3: {
				cout << g.V() << endl;
				break;
			}
			case 4: {
				cout << g.E() << endl;
				break;
			}
			case 5: {
				int v1, v2;
				cin >> v1 >> v2;
				cout << g.Edge(v1, v2) << endl;
				break;
			}
			case 6: {
				int v1, v2, weight;
				cin >> v1 >> v2 >> weight;
				cout << g.SetEdge(v1, v2, weight) << endl;
				break;
			}
			case 7: {
				g.Show();
				break;
			}
			case 8: {
				vector<int> radius = g.Task();
				cout << "Центр: " << *radius.rbegin() << endl;
				cout << "Путь:";
				for (const int& vertex : radius) {
					cout << " " << vertex;
				}
				break;
			}
			default: {
				cout << "Попробуйте еще раз" << endl;
				break;
			}
			}
		}
		catch (exception e) {
			cout << e.what() << endl;
		}
	}
}
