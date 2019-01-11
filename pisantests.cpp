/**
 * Testing ass3 graph functions
 */

#include <iostream>
#include <sstream>
#include "graph.h"

using namespace std;

// global variable, used to make tests easier to write
stringstream SS;

/**
 * print OK or ERR, comparing the two given parameters
 * optional message parameter also printed to identify test
 * This version works when data types are the same
 * @param valueReturnedByAFunction
 * @param expectedValue
 * @param message optional message
 */
template<typename T>
void isOK(const T &got, const T &expected, string msg = "") {
  if (got == expected) {
    if (msg == "")
      cout << "OK: got expected value: " << got << endl;
    else
      cout << "OK: " << msg << endl;
  } else {
    if (msg == "")
      cout << "ERR: " << msg << endl;
    else
      cout << "ERR: Test Failed" << endl;
    cout << "    Got   " << got << "\n expected " << expected << endl;
  }
}

// print OK or ERR, works for string and char*
// anything in quotes, such as "abc", defaults to char*
// and has to be explicitly converted
void isOK(const string &str, const char *cstr, string msg = "") {
  isOK(str, string(cstr), msg);
}

// print OK or ERR, works for stringstream and anything else
// convenience function
// ALSO resets global variable simplestream SS
// not the best programming practice, but useful for tests
template<typename T>
void isOK(const stringstream &ss, const T &expected, string msg = "") {
  isOK(ss.str(), string(expected), msg);
  SS.str("");
}

// convert a map to a string so we can compare it
template<typename T>
string map2string(const map<string, T> mp) {
  stringstream ss;
  for (auto &p : mp) {
    ss << "[" << p.first << ":" << p.second << "]";
  }
  return ss.str();
}

void testGraphBasic() {
  Graph g;
  isOK(g.AddVertex("a"), true, "add vertex a");
  isOK(g.AddVertex("b"), true, "add vertex b");
  isOK(g.AddVertex("c"), true, "add vertex c");
  isOK(g.AddVertex("d"), true, "add vertex d");
  isOK(g.AddVertex("e"), true, "add vertex e");
  isOK(g.AddVertex("b"), false, "b added twice");
  isOK(g.Connect("a", "b", 10), true, "connect a b");
  isOK(g.Connect("a", "b", 50), false, "duplicate connect a b");
  isOK(g.Connect("a", "a", 1), false, "connect a to itself");
  g.Connect("a", "d", 40);
  g.Connect("a", "c", 20);
  isOK(g.NumberOfVertices(), 5, "graph number of vertices");
  isOK(g.NumberOfEdges(), 3, "graph number of edges");
  isOK(g.NumberOfEdges("a"), 3, "vertex number of edges");
  isOK(g.NumberOfEdges("c"), 0, "no outgoing edges c");
  isOK(g.NumberOfEdges("xxx"), -1, "no edges for nonexistent vertex");
  isOK(g.HasVertex("xxxx"), false, "xxx not in graph");
  isOK(g.HasVertex("a"), true, "a in graph");

// check that they are sorted based on edge end label
  isOK(g.GetEdges("a"), "b(10),c(20),d(40)");
// disconnect non-existent edge/vertex
  isOK(g.Disconnect("a", "e"), false, "disconnecting non-existent vertex");
  isOK(g.NumberOfEdges(), 3, "number of edges after failed disconnect");
  isOK(g.Disconnect("a", "c"), true, "disconnecting existent vertex");
  isOK(g.NumberOfEdges(), 2, "graph number of edges after disconnect");
  isOK(g.NumberOfEdges("a"), 2, "vertex number of edges after disconnect");
  isOK(g.GetEdges("a"), "b(10),d(40)", "removing middle edge");

}

// visitor function used for DFS and BFS
void labelVisitor(const string &label) {
  SS << label;
}

void testGraph0DFS() {
  cout << "testGraph0DFS" << endl;
  Graph g;
  if (!g.ReadFile("graph0.txt"))
    return;
  isOK(g.HasVertex("A"), true, "a in graph");
  isOK(g.HasVertex("B"), true, "a in graph");
  isOK(g.HasVertex("C"), true, "a in graph");
  isOK(g.GetEdges("A"), "B(1),C(8)");
  isOK(g.GetEdges("B"), "C(3)");
  isOK(g.GetEdges("C"), "");
  g.DFS("A", labelVisitor);
  // cout << "DFS(A) is " << SS.str() << endl;
  isOK(SS, "ABC", "starting from A");

  g.DFS("B", labelVisitor);
  // cout << "DFS(B) is " << SS.str() << endl;
  isOK(SS, "BC", "starting from B");

  g.DFS("C", labelVisitor);
  // cout << "DFS(C) is " << SS.str() << endl;
  isOK(SS, "C", "starting from C");

  g.DFS("X", labelVisitor);
  // cout << "BFS(X) is " << SS.str() << endl;
  isOK(SS, "", "starting from X");
}

void testGraph0BFS() {
  cout << "testGraph0BFS" << endl;
  Graph g;
  if (!g.ReadFile("graph0.txt"))
    return;
  g.BFS("A", labelVisitor);
  // cout << "BFS(A) is " << SS.str() << endl;
  isOK(SS, "ABC", "starting from A");

  g.DFS("B", labelVisitor);
  // cout << "BFS(B) is " << SS.str() << endl;
  isOK(SS, "BC", "starting from B");

  g.DFS("C", labelVisitor);
  // cout << "BFS(C) is " << SS.str() << endl;
  isOK(SS, "C", "starting from C");

  g.DFS("X", labelVisitor);
  // cout << "BFS(X) is " << SS.str() << endl;
  isOK(SS, "", "starting from X");
}

void testGraph0Dijkstra() {
  cout << "testGraph0Dijkstra" << endl;
  Graph g;
  if (!g.ReadFile("graph0.txt"))
    return;
  map<string, int> weights;
  map<string, string> previous;
  g.Dijkstra("A", weights, previous);
  // cout << "Dijkstra(A) weights is " << map2string(weights) << endl;
  isOK(map2string(weights), "[B:1][C:4]", "Dijkstra(A) weights");
  // cout << "Dijkstra(A) previous is " << map2string(previous) << endl;
  isOK(map2string(previous), "[B:A][C:B]", "Dijkstra(A) previous");

  weights.clear();
  previous.clear();
  g.Dijkstra("B", weights, previous);
  // cout << "Dijkstra(B) weights is " << map2string(weights) << endl;
  isOK(map2string(weights), "[C:3]", "Dijkstra(B) weights");
  // cout << "Dijkstra(B) previous is " << map2string(previous) << endl;
  isOK(map2string(previous), "[C:B]", "Dijkstra(B) previous");

  weights.clear();
  previous.clear();
  // cout << "Dijkstra(C) weights is " << map2string(weights) << endl;
  isOK(map2string(weights), "", "Dijkstra(C) weights");
  // cout << "Dijkstra(C) previous is " << map2string(previous) << endl;
  isOK(map2string(previous), "", "Dijkstra(C) previous");
}

void testGraph1() {
  cout << "testGraph1" << endl;
  Graph g;
  if (!g.ReadFile("graph1.txt"))
    return;
  g.DFS("A", labelVisitor);
  isOK(SS, "ABCDEFGH", "dfs starting from A");
  g.BFS("A", labelVisitor);
  isOK(SS, "ABHCGDEF", "bfs starting from A");
  g.DFS("B", labelVisitor);
  isOK(SS, "BCDEFG", "dfs starting from B");
  g.BFS("B", labelVisitor);
  isOK(SS, "BCDEFG", "bfs starting from B");

  map<string, int> weights;
  map<string, string> previous;
  g.Dijkstra("A", weights, previous);
  // cout << "Dijkstra(A) weights is " << map2string(weights) << endl;
  isOK(map2string(weights), "[B:1][C:2][D:3][E:4][F:5][G:4][H:3]",
       "Dijkstra(B) weights");
  // cout << "Dijkstra(B) previous is " << map2string(previous) << endl;
  isOK(map2string(previous), "[B:A][C:B][D:C][E:D][F:E][G:H][H:A]",
       "Dijkstra(B) previous");
}

void testGraph2() {
  cout << "testGraph2" << endl;
  Graph g;
  if (!g.ReadFile("graph2.txt"))
    return;
  g.DFS("O", labelVisitor);
  isOK(SS, "OPRSTUQ", "dfs starting from O");
  g.BFS("O", labelVisitor);
  isOK(SS, "OPQRSTU", "bfs starting from O");

  map<string, int> weights;
  map<string, string> previous;
  g.Dijkstra("O", weights, previous);
  // cout << "Dijkstra(O) weights is " << map2string(weights) << endl;
  isOK(map2string(weights), "[P:5][Q:2][R:3][S:6][T:8][U:9]",
       "Dijkstra(B) weights");
  // cout << "Dijkstra(O) previous is " << map2string(previous) << endl;
  isOK(map2string(previous), "[P:O][Q:O][R:Q][S:R][T:S][U:S]",
       "Dijkstra(O) previous");
}

//int main() {
//  testGraphBasic();
//  testGraph0DFS();
//  testGraph0BFS();
//  testGraph0Dijkstra();
//  testGraph1();
//  testGraph2();
//  return 0;
//}
