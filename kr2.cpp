#include <iostream>
#include "graph.h"

using namespace std;

//Вариант 8
//Реализация АТД «Взвешенный орграф». Граф представлен в виде списков смежности(L - граф).
//Определение радиуса и определения списка вершин для соответствующего радиусу пути на основе алгоритма Дейкстры. 
//(радиус – минимальный эксцентриситет в графе, путь - последовательность вершин, лежащих на пути с суммарным весом ребер, равным радиусу).

int main() {
	setlocale(LC_ALL, "Russian");
	Graph<int, int> g = Graph<int, int>();
	int n;
	while (1) {
		cout << endl << "Меню. Выберите действие:" << endl << endl;
		cout << "1. Вставка" << endl;
		cout << "2. Удаление" << endl;
		cout << "3. Число вершин" << endl;
		cout << "4. Число ребер" << endl;
		cout << "5. Опрос наличия ребра" << endl;
		cout << "6. Задание параметров ребра" << endl;
		cout << "7. Вывод структуры графа на экран" << endl;
		cout << "8. Определение радиуса и определения списка вершин для соответствующего радиусу пути на основе алгоритма Дейкстры" << endl;
		cout << "9. Сгенерировать Взвешенный орграф" << endl;

		cout << endl;
		cin >> n;
		cout << endl;
		try {
			switch (n) {
			case 1: {
				int v1, v2;
				cin >> v1 >> v2;
				g.Insert(v1, v2);
				break;
			}
			case 2: {
				int v1, v2;
				cin >> v1 >> v2;
				g.Delete(v1, v2);
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
				g.SetEdge(v1, v2, weight);
				break;
			}
			case 7: {
				g.Show();
				break;
			}
			case 8: {
				g.Task();
				break;
			}
			case 9: {
				g.Insert(1, 2);
				g.SetEdge(1, 2, 2);

				g.Insert(1, 4);
				g.SetEdge(1, 4, 1);

				g.Insert(2, 5);
				g.SetEdge(2, 5, 3);

				g.Insert(3, 6);
				g.SetEdge(3, 6, 1);

				g.Insert(3, 5);
				g.SetEdge(3, 5, 4);

				g.Insert(4, 2);
				g.SetEdge(4, 2, 10);

				g.Insert(4, 5);
				g.SetEdge(4, 5, 2);

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
