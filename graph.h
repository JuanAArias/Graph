/*
* graph.h
*
* Interface/specifications for Graph class
*
* @author Yusuf Pisan & Juan Arias 
*
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <climits>
#include <utility>
#include <string>
#include <stack>
#include <queue>
#include <set>
#include <map>

/*
* A graph is made up of vertices and edges.
* Vertex labels are unique.
* A vertex can be connected to other vertices via weighted, directed edge.
* A vertex cannot connect to itself or have multiple edges to the same vertex
*/
class Graph {

	/*
	* Friend function to print Graph to ostream
	* @param out The ostream to print to Graph
	* @param g The Graph to be printed
	* @return out by reference
	*/
	friend std::ostream& operator<<(std::ostream& out, const Graph& g);

public:

	/* Type definitions*/
	using Label        = std::string;
	using Weight       = int;
	using Visit        = void(const Label&);
	using LabelStack   = std::stack<Label>;
	using LabelQ       = std::queue<Label>;
	using LabelWeight  = std::pair<const Label, const Weight>;
	using PrevLabel    = std::pair<const Label, const Label>;
	using WeightMap    = std::map<Label, Weight>;
	using PrevMap      = std::map<Label, Label>;

	/* Constants for reuse with meaning*/
	static const Label  NO_LABEL;
	static const Weight NO_WEIGHT;
	static const int    EMPTY;
	static const int    COUNT;
	static const char   COMMA;
	static const char   LEFT_P;
	static const char   RIGHT_P;

	/*
	* Constructs empty graph
	*/
	Graph();

	/*
	* Copy constructor overload
	* @param other The other Graph to copy
	*/
	Graph(const Graph& other);

	/*
	* Destroys Graph, all Vertices, all Edges and deallocates dynamic memory
	*/
	virtual ~Graph();

	/*
	* Assignment operator overload
	* @param other The other Graph to copy
	*/
	Graph& operator=(const Graph& other);

	 /*
	* Get the total number of Vertices in Graph
	* @return total number of Vertices in Graph
	*/
	int NumberOfVertices() const;

	/*
	* Get the total number of Edges in Graph
	* @return total number of Edges in Graph
	*/
	int NumberOfEdges() const;

	/*
	* Get the number of Edges connected to a given Vertex
	* @param label The label of the Vertex
	* @return number of edges from Vertex, -1 if Vertex not found
	*/
	int NumberOfEdges(const Label& label) const;

	/*
	* Add a Vertex to Graph, no duplicates allowed
	* @param label The label of the Vertex
	* @return true if vertex added, false if it already is in Graph
	*/
	bool AddVertex(const Label& label);

	/*
	* Check is Vertex exists in Graph
	* @param label The label of the vertex to find
	* @return true if Vertex in Graph, else false
	*/
	bool HasVertex(const Label& label) const;

	/*
	* String representation of Edges from a given Vertex
	* @param label The label of the Vertex to get Edges from
	* @return string representing Edges and weights, "" if Vertex not found
	*/
	std::string GetEdges(const Label& label) const;

	/*
	* Add a new Edge between start and end Vertex
	* If the Vertices do not exist, create them
	* A Vertex cant connect to itself or have multiple Edges to another Vertex
	* @param label1 The starting Vertex label
	* @param label2 The ending Vertex label
	* @param weigt An optional weight for new Edge, defaults to 0
	* @return true if successfully connected, else false
	*/
	bool Connect(const Label& label1, const Label& label2,
		         const Weight& weight = 0);

	/*
	* Remove Edge from Graph
	* @param label1 The starting Vertex label
	* @param label2 The ending Vertex label
	* @return true if Edge successfully deleted, else false
	*/
	bool Disconnect(const Label& label1, const Label& label2);

	/*
	* Read Edges from file, first line of file is an integer indicating
	* number of Edges, each line represents an Edge in the form of
	* "string string int", Vertex labels cannot contain spaces
	* Clears previous contents
	* @param fileName The name of the file
	* @return true if file successfully read, else false
	*/
	bool ReadFile(const std::string& fileName);

	/*
	* Depth-first traversal originating from Vertex with given label
	* @param label The origin Vertex label
	* @param func The function to call on each Vertex label
	*/
	void DFS(const Label& label, Visit func);

	/*
	* Breadth-first traversal originating from Vertex with given label
	* @param label The origin Vertex label
	* @param func The function to call on each Vertex label
	*/
	void BFS(const Label& label, Visit func);

	/**
	* Dijkstra's algorithm to find shortest distance to all other Vertices
	* and the path to all other Vertices
	* Path cost is recorded in the map passed in, e.g. weights["F"] = 10
	* How to get to vertex is recorded in map passed in, prevs["F" = "C"
	* @param label The label of the origin Vertex
	* @param wMap The map to record shortest path
	* @param pMap The map to record previous Vertex
	*/
	void Dijkstra(const Label& label, WeightMap& wMap, PrevMap& pMap);

	/*
	* Creates a MinSpanningTree using Prim's algorithm
	* @param label The label of the origin Vertex
	* @return a Graph of the MinSpanningTree
	*/
	Graph* MinSpanningTree(const Label& label);

	/*
	* Gets the sum cost of all Edges in Graph
	* @return sum cost of all Edges in Graph
	*/
	Weight SumOfEdges() const;

private:

	/*
	* Private Vertex class of Graph
	*/
	class Vertex {

	public:

		/*
		* Constructs Vertex with given Label 
		* @param label The lable of the Vertex, default to blank
		* @param weight The weight of the Vertex, default to -1
		* @param next The next Vertex in the list
		*/
		explicit Vertex(const Label& label   = Graph::NO_LABEL,
			            const Weight& weight = Graph::NO_WEIGHT,
			            Vertex* next         = nullptr,
			            const int& edges     = Graph::EMPTY);

		/*
		* Copy constructor with pointers
		* @param other The other Vertex to copy
		*/
		Vertex(const Vertex& other);

		/*
		* Assignment operator overload
		* @param other The other Vertex to copy
		* @return this by reference
		*/
		Vertex& operator=(const Vertex& other);

		/*
		* Destroys Vertex
		*/
		virtual ~Vertex();

		/*
		* Gets Vertex label
		* @return Vertex label
		*/
		Label getLabel() const;

		/*
		* Gets Edge Weight (for adjacent Vertices)
		* @return Edge Weight
		*/
		Weight getWeight() const;

		/*
		* Returns true if Vertex visited, else false
		* @return true if Vertex visited, else false
		*/
		bool isVisited() const;

		/*
		* Sets the Vertex as visited or unvisited
		* @param visited The boolean to indicated if visited(true) or
		* unvisited(false)
		*/
		void setVisited(bool visited = true);

		/*
		* Gets the pointer to next Vertex in the adjacency list
		* @retrun pointer to next Vertex in the adjacency list
		*/
		Vertex* getNext() const;

		/*
		* Sets next to new Vertex in the adjacency list
		* @param pointer to next Vertex in the adjacency list to add
		*/
		void setNext(Vertex* next);

		/*
		* Counts all Edges for a Vertex
		* @return number of Edges connected from Vertex
		*/
		int numOfEdges() const;

		/*
		* Gets string representation of Edges & adjacent Vertices
		* @return string representation of Edges & adjacent Vertices
		*/
		std::string getEdges() const;

		/*
		* Adds new Vertex with label in adjacent list
		* Must maintain sorted list, Vertex cant connect to itself
		* No duplicate Edges
		* @param label The label to give the adjacent Vertex
		* @param weight The weight to label the Edge
		* @return true if connected, else false
		*/
		bool connect(const Label& label, const Weight& weight);

		/*
		* Deletes Vertex with label in adjacent list
		* @param label The label of the adjacent Vertex to delete
		* @return true if disconnected, else false
		*/
		bool disconnect(const Label& label);

		/*
		* Gets the adjacent Vertex with given label
		* or nullptr if it does not exist
		* @param label The label of the adjacent Vertex
		* @return pointer to adjacent Vertex or nullptr if it does not exist
		*/
		Vertex* getAdjacent(const Label& label) const;

		/*
		* Deallocate all adjacent Vertices
		*/
		void clear();

	private:
		
		/* Label to uniquely ID each Vertex */
		const Label label;

		/* Edge weight */
		const Weight weight;

		/* Visited mark for Graph traversals */
		bool visited;

		/* Pointer to next Vertex in adjacency list*/
		Vertex* next;

		/* Number of Edges from Vertex */
		int edges;

		/*
		* Makes string representation Vertex
		* @return string Vertex
		*/
		std::string toString() const;
	};

	/* Type definitions */
	using VertexSet    = std::set<const Vertex*>;
	using LabelVert    = std::pair<const Label, Vertex*>;
	using AdjacencyMap = std::map<const Label, Vertex*>;

	/* Adjacency Map from label to Vertex */
	AdjacencyMap map;

	/* Number of Edges in graph */
	int edges;

	/*
	* Helper for copy constructor and assignment operator overload
	* Copies contents of AdjacencyMap in other Graph
	* @param otherMap The map in the other Graph to clone
	*/
	void clone(const AdjacencyMap* otherMap);


	/*
	* Deallocate all dynamic memory
	*/
	void clear();

	/*
	* Extracts contents from file to create Vertices & Edges
	* @param file The file to extract contents from
	*/
	void extractFile(std::ifstream* file);

	/*
	* Helper for depthFirstTraversal
	* @param stack The stack to search depth first with
	*/
	void dfs(LabelStack* stack, Visit func);

	/*
	* Helper for search functions
	* Gets the next unvisited Vertex label adjacent to Vertex with given label
	* @param label The label of the Vertex to get next from
	* @return next unvisited Vertex label adjacent to Vertex, or blank if none
	*/
	Label nextLabel(const Label& label);

	/*
	* Helper for nextLabel
	* Gets the next unvisited Vertex starting from curr
	* @param curr The current Vertex
	* @param nextLabel Pointer to store label of next unvisited Vertex
	* @return next unvisited Vertexnext unvisited Vertex
	*/
	Vertex* nextVertex(Vertex* curr, Label* nextLabel) const;

	/*
	* Helper for breadthFirstTraversal
	* @param q The queue to search breadth first with
	* @param func The function to call on each Vertex
	*/
	void bfs(LabelQ* q, Visit func);

	/*
	* Helper for bfsHelper, adds all unvisited adjacent Vertices to q
	* @param q The queue to search breadth first with
	* @param curr The Vertex to get unvisited adjacent Vertices from
	*/
	void addUnvisited(LabelQ* q, Vertex* curr);

	/*
	* Helper for Dijkstra, initializes WeightMap and PrevMap for all Vertices
	* adjacent to the origin Vertex
	* @param curr The origin Vertex
	* @param label The label of the origin Vertex
	* @param wMap The WeightMap
	* @param pMap The PrevMap
	*/
	void initMaps(const Vertex* curr, const Label& label, WeightMap* wMap,
		          PrevMap* pMap) const;

	/*
	* Helper for Dijkstra, iteratively gets the next smallest Vertex from
	* WeightMap thats not in VertexSet and updates any possible shorter paths
	* from the origin
	* @param set The VertexSets
	* @param wMap The WeightMap
	* @param pMap The PrevMap
	*/
	void dijkstra(VertexSet* set, WeightMap* wMap, PrevMap* pMap) const;

	/*
	* Helper for dijkstra, gets the label for the next smallest Vertex from
	* WeightMap thats not in VertexSet
	* @param set The VertexSet
	* @param wMap The WeightMap
	* @param label The pointer to assign as the label of the next smallest Vertex
	* @return pointer to the next smallest Vertex
	*/
	Vertex* nextSmallest(const VertexSet* set, const WeightMap* wMap,
		                 Label* label) const;

	/*
	* Helper for dijkstra, updates an entry in the WeightMap if there is a shorter
	* path
	* @param curr The current Vertex
	* @param pair The entry in the WeightMap to update
	* @param label The label of the current Vertex
	* @param wMap The WeightMap
	* @param pMap The PrevMap
	*/
	void update(const Vertex* curr, const LabelWeight& pair,
		        const Label& label, WeightMap* wMap, PrevMap* pMap) const;

	/*
	* Helper for Dijkstra, removes origin Vertex entry and entries in WeightMap that
	* were not visited during Dijkstra 
	* @param wMap The WeightMap
	*/
	void cleanWeights(WeightMap* wMap);

	/*
	* Helper for MinSpanningTree, gets the Vertices connected to the next
	* minimum cost Edge
	* @param curr The starting Vertex on the next minimum cost Edge
	* @param next The ending Vertex on the next minimum cost Edge
	* @param label The label of the starting Vertex on the next minimum cost Edge
	*/
	void nextMinEdge(Vertex*& curr, Vertex*& next, Label* label);

	/*
	* Mark all verticies as unvisited
	*/
	void unvisitVertices();

};
#endif  // GRAPH_H
