/*
* ass3.cpp
*
* Unit tests for Graph class
*
* @author Juan Arias
*
*/

#include <cassert>
#include "graph.h"

/* Constant array for testing */
static const Graph::Label arr[10] {"A", "B", "C", "D", "E",
								   "F", "G", "H", "I", "J" };

/*
* Unit test for empty constructor
*/
void constructor() {

	Graph g;
}

/*
* Unit test for hasVertex
*/
void hasVertex() {

	Graph g;

	for (const Graph::Label lbl : arr) {

		assert(!g.HasVertex(lbl));
	}
}

/*
* Unit test for addVertex
*/
void addVertex() {
	
	Graph g;

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && !g.AddVertex(lbl) && g.HasVertex(lbl));
	}
}

/*
* Unit test for numberOfVertices
*/
void numOfVertices() {

	Graph g;

	assert(g.NumberOfEdges() == 0 && g.NumberOfVertices() == 0);

	int count(0);

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && g.NumberOfVertices() == ++count);
	}
}

/*
* Helper function to add Egdes in Graph
* @param g The Graph to add Edges to
*/
void addEdges(Graph* g) {

	assert(g->Connect("A", "C", 14) && g->Connect("C", "F", 4) &&
		   g->Connect("H", "I", 8)  && g->Connect("A", "G")    &&
		   g->Connect("A", "J", 6)  && g->Connect("C", "B", 7) &&
		   g->Connect("B", "F", 10) && g->Connect("D", "A", 3) &&
		   g->Connect("I", "E", 1)
		&& !g->Connect("A", "C") && !g->Connect("C", "F")
		&& !g->Connect("H", "I") && !g->Connect("A", "G")
		&& !g->Connect("A", "J") && !g->Connect("C", "B")
		&& !g->Connect("B", "F") && !g->Connect("D", "A")
		&& !g->Connect("I", "E")
	);
}

/*
* Unit test for connect
*/
void connect() {
	
	Graph g;

	int count(0);

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && !g.Connect(lbl, lbl) &&
			   g.NumberOfVertices() == ++count);
	}

	addEdges(&g);
}

/*
* Helper function to remove Edges in Graph
* @param g The Graph to remove Edges from
*/
void removeEdges(Graph* g) {
	
	assert(g->Disconnect("A", "C") && g->Disconnect("C", "F")  &&
		   g->Disconnect("H", "I") && g->Disconnect("A", "G")  &&
		   g->Disconnect("A", "J") && g->Disconnect("C", "B")  &&
		   g->Disconnect("B", "F") && g->Disconnect("D", "A")  &&
		   g->Disconnect("I", "E") && !g->Disconnect("A", "C") &&
		  !g->Disconnect("C", "F") && !g->Disconnect("H", "I") &&
		  !g->Disconnect("A", "G") && !g->Disconnect("A", "J") &&
		  !g->Disconnect("C", "B") && !g->Disconnect("B", "F") &&
		  !g->Disconnect("D", "A") && !g->Disconnect("I", "E"));
}

/*
* Unit test for disconnect
*/
void disconnect() {

	Graph g;

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && !g.Disconnect(lbl, lbl));
	}

	addEdges(&g);
	removeEdges(&g);
}

/*
* Unit test for numberOfEdges
*/
void numOfEdges() {

	Graph g;

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && g.NumberOfEdges() == 0);
	}

	addEdges(&g);
	
	assert(g.NumberOfEdges() == 9 && g.NumberOfEdges("A") == 3 &&
		g.NumberOfEdges("B") == 1 && g.NumberOfEdges("C") == 2 &&
		g.NumberOfEdges("D") == 1 && g.NumberOfEdges("E") == 0 &&
		g.NumberOfEdges("F") == 0 && g.NumberOfEdges("G") == 0 &&
		g.NumberOfEdges("H") == 1 && g.NumberOfEdges("I") == 1 &&
		g.NumberOfEdges("J") == 0);

	removeEdges(&g);
	
	for (const Graph::Label& lbl : arr) {
	
		assert(g.NumberOfEdges(lbl) == 0);
	}
}

/*
* Unit test for getEdges
*/
void getEdges() {

	Graph g;

	for (const Graph::Label& lbl : arr) {

		assert(g.AddVertex(lbl) && g.GetEdges(lbl) == "");
	}

	addEdges(&g);

	assert(g.GetEdges("A") == "C(14),G(0),J(6)" && g.GetEdges("I") == "E(1)" &&
		   g.GetEdges("C") == "B(7),F(4)"       && g.GetEdges("H") == "I(8)" &&
		   g.GetEdges("B") == "F(10)"           && g.GetEdges("D") == "A(3)"
	);

	removeEdges(&g);

	for (const Graph::Label& lbl : arr) {
		
		assert(g.GetEdges(lbl) == "");
	}
}

/*
* Test readFile for graph0.txt
*/
void file0() {

	Graph g;

	assert(g.ReadFile("graph0.txt"));

	assert(g.NumberOfEdges() == 3    && g.NumberOfVertices() == 3            &&
		   g.HasVertex("A")          && g.HasVertex("B") && g.HasVertex("C") &&
		   g.NumberOfEdges("A") == 2 && g.NumberOfEdges("B") == 1            &&
		   g.NumberOfEdges("C") == 0 && g.GetEdges("A") == "B(1),C(8)"       &&
		   g.GetEdges("B") == "C(3)" && g.GetEdges("C") == "");
}

/*
* Test readFile for graph1.txt
*/
void file1() {

	Graph g;

	assert(g.ReadFile("graph1.txt"));

	assert(g.NumberOfEdges() == 9 && g.NumberOfVertices() == 10);

	for (char c('A'); c <= 'H'; ++c) {

		assert(g.HasVertex(Graph::Label(1, c)));
	}

	assert(g.HasVertex("X") && g.HasVertex("Y") && g.NumberOfEdges("A") == 2);

	int edges(1);

	for (char c('B'); c <= 'H'; ++c) {
		
		edges = (c == 'G') ? 0 : 1;

		assert(g.NumberOfEdges(Graph::Label(1, c)) == edges);
	}

	assert(g.NumberOfEdges("X") == 1      && g.NumberOfEdges("Y") == 0 &&
		   g.GetEdges("A") == "B(1),H(3)" && g.GetEdges("B") == "C(1)" &&
		   g.GetEdges("C") == "D(1)"      && g.GetEdges("D") == "E(1)" &&
		   g.GetEdges("E") == "F(1)"      && g.GetEdges("F") == "G(1)" &&
		   g.GetEdges("H") == "G(1)"      && g.GetEdges("X") == "Y(10)"
	);
}

/*
* Test readFile for graph2.txt
*/
void file2() {
	
	Graph g;

	assert(g.ReadFile("graph2.txt"));

	assert(g.NumberOfEdges() == 24 && g.NumberOfVertices() == 21);

	for (char c('A'); c <= 'U'; ++c) {

		assert(g.HasVertex(Graph::Label(1, c)));
	}

	assert(g.NumberOfEdges("A") == 3 && g.NumberOfEdges("B") == 2 &&
		   g.NumberOfEdges("C") == 1 && g.NumberOfEdges("D") == 2 &&
		   g.NumberOfEdges("E") == 0 && g.NumberOfEdges("F") == 1 &&
		   g.NumberOfEdges("G") == 2 && g.NumberOfEdges("H") == 1 &&
		   g.NumberOfEdges("I") == 2 && g.NumberOfEdges("J") == 0 &&
		   g.NumberOfEdges("K") == 0 && g.NumberOfEdges("L") == 0 &&
		   g.NumberOfEdges("M") == 0 && g.NumberOfEdges("N") == 0 &&
		   g.NumberOfEdges("O") == 2 && g.NumberOfEdges("P") == 1 &&
		   g.NumberOfEdges("Q") == 1 && g.NumberOfEdges("R") == 2 &&
		   g.NumberOfEdges("S") == 3 && g.NumberOfEdges("T") == 1 &&
		   g.GetEdges("A") == "B(0),C(0),D(0)" &&
		   g.GetEdges("B") == "E(0),F(0)" && g.GetEdges("C") == "G(0)" &&
		   g.GetEdges("D") == "H(0),I(0)" && g.GetEdges("E") == "" &&
		   g.GetEdges("F") == "J(0)"      && g.GetEdges("G") == "K(0),L(0)" &&
		   g.GetEdges("H") == "M(0)"      && g.GetEdges("I") == "M(0),N(0)" &&
		   g.GetEdges("J") == ""          && g.GetEdges("K") == "" &&
		   g.GetEdges("L") == ""          && g.GetEdges("M") == "" &&
		   g.GetEdges("N") == ""          && g.GetEdges("O") == "P(5),Q(2)" &&
		   g.GetEdges("P") == "R(2)"      && g.GetEdges("Q") == "R(1)" &&
		   g.GetEdges("R") == "O(1),S(3)" &&
		   g.GetEdges("S") == "R(1),T(2),U(3)" && g.GetEdges("T") == "O(8)");
}

/*
* Unit test for readFile
*/
void readFile() {

	file0();
	file1();
	file2();
}

/*
* Visit function to use in DFS & BFS
* @param lbl The label of each visited Vertex
*/
void printVisit(const Graph::Label& lbl) {
	
	std::cout << lbl;
}

/*
* Helper for DepthFirstSearch & BreadthFirstSearch unit tests
* @param g The Graph to search on
* @param arr The array of Vertex Labels to search from
* @param size The size of the array
* @param dfs True if DepthFirstSearch, else false (BreadthFirstSearch)
*/
void searchTest(Graph* g, const Graph::Label arr[], int size, bool dfs) {

	for (int i(0); i < size; ++i) {

		std::cout << "From " << arr[i] << ": ";

		if (dfs) {

			g->DFS(arr[i], printVisit);

		} else {
		
			g->BFS(arr[i], printVisit);
		}
	
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

/*
* Unit test for DepthFirstSearch
*/
void DFS() {
	
	std::cout << "DFS" << std::endl << std::endl;

	Graph g;

	assert(g.ReadFile("graph0.txt"));
	Graph::Label arr0[3] {"A", "B", "C"};
	std::cout << "graph0" << std::endl;
	searchTest(&g, arr0, 3, true);


	assert(g.ReadFile("graph1.txt"));
	Graph::Label arr1[5] {"A", "B", "C", "H", "X"};
	std::cout << "graph1" << std::endl;
	searchTest(&g, arr1, 5, true);

	assert(g.ReadFile("graph2.txt"));
	Graph::Label arr2[6] {"A", "D", "O", "T", "R", "S"};
	std::cout << "graph2" << std::endl;
	searchTest(&g, arr2, 6, true);
}

/*
* Unit test for BreadthFirstSearch
*/
void BFS() {
	
	std::cout << "BFS" << std::endl << std::endl;

	Graph g;

	assert(g.ReadFile("graph0.txt"));
	Graph::Label arr0[3]{ "A", "B", "C" };
	std::cout << "graph0" << std::endl;
	searchTest(&g, arr0, 3, false);

	std::cout << std::endl;

	assert(g.ReadFile("graph1.txt"));
	Graph::Label arr1[5]{ "A", "B", "C", "H", "X" };
	std::cout << "graph1" << std::endl;
	searchTest(&g, arr1, 5, false);

	std::cout << std::endl;

	assert(g.ReadFile("graph2.txt"));
	Graph::Label arr2[6]{ "A", "D", "O", "T", "R", "S" };
	std::cout << "graph2" << std::endl;
	searchTest(&g, arr2, 6, false);
}

/*
* Unit test for Dijkstra shortest path
*/
void Dijkstra() {

	Graph g;
	Graph::WeightMap wMap;
	Graph::PrevMap pMap;

	assert(g.ReadFile("graph0.txt"));

	g.Dijkstra("A", wMap, pMap);
	assert(wMap["B"] == 1 && wMap["C"] == 4 && pMap["B"] == "A" &&
		   pMap["C"] == "B");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("B", wMap, pMap);
	assert(wMap["C"] == 3 && pMap["C"] == "B");

	wMap.clear();
	pMap.clear();

	assert(g.ReadFile("graph1.txt"));

	g.Dijkstra("A", wMap, pMap);
	assert(wMap["B"] == 1 && wMap["C"] == 2 && wMap["D"] == 3 && wMap["E"] == 4
		&& wMap["F"] == 5 && wMap["G"] == 4 && wMap["H"] == 3
		&& pMap["B"] == "A" && pMap["C"] == "B" && pMap["D"] == "C" &&
		   pMap["E"] == "D" && pMap["F"] == "E" && pMap["G"] == "H" &&
		   pMap["H"] == "A");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("B", wMap, pMap);
	assert(wMap["C"] == 1 && wMap["D"] == 2 && wMap["E"] == 3 && wMap["F"] == 4
		&& wMap["G"] == 5 && pMap["C"] == "B" && pMap["D"] == "C" &&
		   pMap["E"] == "D" && pMap["F"] == "E" && pMap["G"] == "F");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("X", wMap, pMap);
	assert(wMap["Y"] == 10 && pMap["Y"] == "X");

	wMap.clear();
	pMap.clear();

	assert(g.ReadFile("graph2.txt"));

	g.Dijkstra("A", wMap, pMap);
	assert(wMap["B"] == 0 && wMap["C"] == 0 && wMap["D"] == 0 && wMap["E"] == 0
		&& wMap["F"] == 0 && wMap["G"] == 0 && wMap["H"] == 0 && wMap["I"] == 0
		&& wMap["J"] == 0 && wMap["K"] == 0 && wMap["L"] == 0 && wMap["M"] == 0
		&& wMap["N"] == 0 && pMap["B"] == "A" && pMap["C"] == "A" &&
		   pMap["D"] == "A" && pMap["E"] == "B" && pMap["F"] == "B" &&
		   pMap["G"] == "C" && pMap["H"] == "D" && pMap["I"] == "D" &&
		   pMap["J"] == "F" && pMap["K"] == "G" && pMap["L"] == "G" &&
		   pMap["M"] == "H" && pMap["N"] == "I");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("O", wMap, pMap);
	assert(wMap["P"] == 5 && wMap["Q"] == 2 && wMap["R"] == 3 && wMap["S"] == 6
		&& wMap["T"] == 8 && wMap["U"] == 9 && pMap["P"] == "O" &&
		   pMap["Q"] == "O" && pMap["R"] == "Q" && pMap["S"] == "R" &&
		   pMap["T"] == "S" && pMap["U"] == "S");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("T", wMap, pMap);
	assert(wMap["O"] == 8 && wMap["P"] == 13 && wMap["Q"] == 10 &&
	       wMap["R"] == 11 && wMap["S"] == 14 && wMap["U"] == 17 &&
		   pMap["P"] == "O" && pMap["Q"] == "O" && pMap["R"] == "Q" &&
		   pMap["S"] == "R" && pMap["U"] == "S");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("S", wMap, pMap);
	assert(wMap["O"] == 2 && wMap["P"] == 7 && wMap["Q"] == 4 &&
		   wMap["R"] == 1 && wMap["T"] == 2 && wMap["U"] == 3 &&
		   pMap["P"] == "O" && pMap["Q"] == "O" && pMap["R"] == "S" &&
		   pMap["U"] == "S" &&pMap["T"] == "S" && pMap["O"] == "R");

	wMap.clear();
	pMap.clear();

	g.Dijkstra("R", wMap, pMap);
	assert(wMap["O"] == 1 && wMap["P"] == 6 && wMap["Q"] == 3 && wMap["S"] == 3
		&& wMap["T"] == 5 && wMap["U"] == 6 && pMap["P"] == "O" &&
		   pMap["Q"] == "O" && pMap["S"] == "R" && pMap["U"] == "S" &&
		   pMap["T"] == "S" && pMap["O"] == "R");
}

/*
* Unit test for SumOfEdges
*/
void sumOfEdges() {
	
	Graph g;

	assert(g.ReadFile("graph0.txt"));
	assert(g.SumOfEdges() == 12);
	assert(g.Disconnect("A", "C"));
	assert(g.SumOfEdges() == 4);

	assert(g.ReadFile("graph1.txt"));
	assert(g.SumOfEdges() == 20);
	assert(g.Disconnect("X", "Y"));
	assert(g.SumOfEdges() == 10);

	assert(g.ReadFile("graph2.txt"));
	assert(g.SumOfEdges() == 28);
	assert(g.Disconnect("T", "O"));
	assert(g.SumOfEdges() == 20);
}

/*
* Unit test for MinSpanningTree
*/
void MinSpanTree() {
	
	Graph g;

	assert(g.ReadFile("graph0.txt"));

	Graph* mst = g.MinSpanningTree("A");
	assert(mst->SumOfEdges() == 4);
	delete mst;

	assert(g.ReadFile("graph1.txt"));

	mst = g.MinSpanningTree("A");
	assert(mst->SumOfEdges() == 9);
	delete mst;

	mst = g.MinSpanningTree("B");
	assert(mst->SumOfEdges() == 5);
	delete mst;

	assert(g.ReadFile("graph2.txt"));

	mst = g.MinSpanningTree("A");
	assert(mst->SumOfEdges() == 0);
	delete mst;

	mst = g.MinSpanningTree("R");
	assert(mst->SumOfEdges() == 16);
	delete mst;

	mst = g.MinSpanningTree("P");
	assert(mst->SumOfEdges() == 13);
	delete mst;

	mst = g.MinSpanningTree("S");
	assert(mst->SumOfEdges() == 14);
	delete mst;
}

/*
* Runs all unit tests
*/
void runTests() {

	constructor();
	hasVertex();
	addVertex();
	numOfVertices();
	connect();
	disconnect();
	numOfEdges();
	getEdges();
	readFile();
	DFS();
	BFS();
	Dijkstra();
	sumOfEdges();
	MinSpanTree();
}

/*
* Start unit testing
*/
int main() {

	runTests();

	std::cout << "Success!" << std::endl;

	return 0;
}
