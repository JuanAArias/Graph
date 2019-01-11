/*
* graph.h
*
* Implementations for Graph class
*
* @author Juan Arias
*
*/

#include "graph.h"

/* Constant definition*/
const Graph::Label  Graph::NO_LABEL("");
const Graph::Weight Graph::NO_WEIGHT(0);
const int           Graph::EMPTY(0);
const int           Graph::COUNT(1);
const char          Graph::COMMA(',');
const char          Graph::LEFT_P('(');
const char          Graph::RIGHT_P(')');

/*
* Friend function to print Graph to ostream
* @param out The ostream to print to Graph
* @param g The Graph to be printed
* @return out by reference
*/
std::ostream & operator<<(std::ostream& out, const Graph& g) {

	for (const Graph::LabelVert& pair : g.map) {
	
		out << pair.first << ": " << pair.second->getEdges() << std::endl;
	}

	return out;
}


/*
* Constructs empty graph
*/
Graph::Graph() :edges(Graph::EMPTY) {}

/*
* Copy constructor overload
* @param other The other Graph to copy
*/
Graph::Graph(const Graph& other) {
	
	(*this) = other;
}

/*
* Destroys Graph, all Vertices, all Edges and deallocates dynamic memory
*/
Graph::~Graph() {

	this->clear();
}

/*
* Assignment operator overload
* @param other The other Graph to copy
*/
Graph& Graph::operator=(const Graph& other) {
	
	if (this != &other) {
		
		this->clear();
		this->edges = other.edges;
		this->clone(&other.map);
	}

	return (*this);
}

/*
* Get the total number of Vertices in Graph
* @return total number of Vertices in Graph
*/
int Graph::NumberOfVertices() const {
	
	return static_cast<int>(this->map.size());
}

/*
* Get the total number of Edges in Graph
* @return total number of Edges in Graph
*/
int Graph::NumberOfEdges() const {
	
	return this->edges;
}

/*
* Get the number of Edges connected to a given Vertex
* @param label The label of the Vertex
* @return number of edges from Vertex, -1 if Vertex not found
*/
int Graph::NumberOfEdges(const Label& label) const {

	int edges(Graph::EMPTY - Graph::COUNT);

	if (this->map.count(label)) {

		edges = this->map.at(label)->numOfEdges();
	}
		
	return edges;
}

/*
* Add a Vertex to Graph, no duplicates allowed
* @param label The label of the Vertex
* @return true if vertex added, false if it already is in Graph
*/
bool Graph::AddVertex(const Label& label) {
	
	bool added(false);

	if (!this->map.count(label)) {

		this->map[label] = new Vertex();

		added = true;
	}

	return added;
}

/*
* Check is Vertex exists in Graph
* @param label The label of the vertex to find
* @return true if Vertex in Graph, else false
*/
bool Graph::HasVertex(const Label& label) const {

	return this->map.count(label);
}

/*
* String representation of Edges from a given Vertex
* @param label The label of the Vertex to get Edges from
* @return string representing Edges and weights, "" if Vertex not found
*/
std::string Graph::GetEdges(const Label& label) const {

	std::string edges;

	if (this->map.count(label)) {
	
		edges = this->map.at(label)->getEdges();
	}

	return edges;
}

/*
* Add a new Edge between start and end Vertex
* If the Vertices do not exist, create them
* A Vertex cant connect to itself or have multiple Edges to another Vertex
* @param label1 The starting Vertex label
* @param label2 The ending Vertex label
* @param weight An optional weight for new Edge, defaults to 0
* @return true if successfully connected, else false
*/
bool Graph::Connect(const Label& label1, const Label& label2,
	                const Weight& weight) {

	bool connected(false);

	if (label1 != label2) {

		this->AddVertex(label1);
		this->AddVertex(label2);

		connected = this->map[label1]->connect(label2, weight);

		this->edges += (connected) ? Graph::COUNT : Graph::EMPTY;
	}

	return connected;
}

/*
* Remove Edge from Graph
* @param label1 The starting Vertex label
* @param label2 The ending Vertex label
* @return true if Edge successfully deleted, else false
*/
bool Graph::Disconnect(const Label& label1, const Label& label2) {
	
	bool disconnected(false);

	if (label1 != label2 && this->map.count(label1) &&
		                this->map.count(label2)) {

		disconnected = this->map[label1]->disconnect(label2);

		this->edges -= (disconnected) ? Graph::COUNT : Graph::EMPTY;
	}

	return disconnected;
}

/*
* Read Edges from file, first line of file is an integer indicating
* number of Edges, each line represents an Edge in the form of
* "string string int", Vertex labels cannot contain spaces
* Clears previous contents
* @param fileName The name of the file
* @return true if file successfully read, else false
*/
bool Graph::ReadFile(const std::string& fileName) {
	
	bool fileRead(false);

	std::ifstream file(fileName);

	if (file.is_open()) {
		
		this->clear();
		this->extractFile(&file);

		fileRead = true;
	}

	return fileRead;
}

/*
* Depth-first traversal originating from Vertex with given label
* @param label The origin Vertex label
* @param func The function to call on each Vertex label
*/
void Graph::DFS(const Label& label, Visit func) {

	if (this->map.count(label)) {

		LabelStack stack;

		stack.push(label);

		this->dfs(&stack, func);
		this->unvisitVertices();
	}
}

/*
* Breadth-first traversal originating from Vertex with given label
* @param label The origin Vertex label
* @param func The function to call on each Vertex label
*/
void Graph::BFS(const Label& label, Visit func) {
	
	if (this->map.count(label)) {

		LabelQ q;

		q.push(label);

		this->map[label]->setVisited();

		this->bfs(&q, func);

		this->unvisitVertices();
	}
}

/**
* Dijkstra's algorithm to find shortest distance to all other Vertices
* and the path to all other Vertices
* Path cost is recorded in the map passed in, e.g. weights["F"] = 10
* How to get to vertex is recorded in map passed in, prevs["F" = "C"
* @param label The label of the origin Vertex
* @param wMap The map to record shortest path
* @param pMap The map to record previous Vertex
*/
void Graph::Dijkstra(const Label& label, WeightMap& wMap, PrevMap& pMap) {

	Vertex* curr = this->map[label];

	wMap[label] = Graph::NO_WEIGHT;

	this->initMaps(curr->getNext(), label, &wMap, &pMap);

	VertexSet set;

	set.insert(curr);

	dijkstra(&set, &wMap, &pMap);

	this->cleanWeights(&wMap);
}

/*
* Creates a MinSpanningTree using Prim's algorithm
* @param label The label of the origin Vertex
* @return a Graph of the MinSpanningTree
*/
Graph* Graph::MinSpanningTree(const Label& label) {

	Graph* mst = new Graph;

	Vertex* curr = this->map[label],
		  * next = nullptr;

	Label currLabel;

	mst->AddVertex(label);

	curr->setVisited();

	bool done(false);

	while (!done) {
		
		Graph::nextMinEdge(curr, next, &currLabel);

		if (curr != nullptr) {
			
			mst->Connect(currLabel, next->getLabel(), next->getWeight());

		} else {
			
			done = true;

			this->unvisitVertices();
		}
	}

	return mst;
}

/*
* Gets the sum cost of all Edges in Graph
* @return sum cost of all Edges in Graph
*/
Graph::Weight Graph::SumOfEdges() const {

	Weight sum = Graph::NO_WEIGHT;

	for (const LabelVert& pair : this->map) {
	
		Vertex* curr = pair.second->getNext();

		while (curr != nullptr) {
			
			sum += curr->getWeight();

			curr = curr->getNext();
		}
	}

	return sum;
}

/*
* Deallocate all dynamic memory
*/
void Graph::clear() {

	for (const LabelVert& pair : this->map) {
		
		pair.second->clear();
		
		delete pair.second;
	}

	this->map.clear();
	this->edges = Graph::EMPTY;
}

/*
* Helper for copy constructor and assignment operator overload
* Copies contents of AdjacencyMap in other Graph
* @param otherMap The map in the other Graph to clone
*/
void Graph::clone(const AdjacencyMap* otherMap) {
	
	for (const LabelVert& pair : *otherMap) {

		*this->map[pair.first] = *pair.second;
	}
}

/*
* Extracts contents from file to create Vertices & Edges
* @param file The file to extract contents from
*/
void Graph::extractFile(std::ifstream* file) {

	int edges;

	(*file) >> edges;

	for (int i(0); i < edges; ++i) {
		
		char c1, c2;
		Weight w;

		(*file) >> c1 >> c2 >> w;

		this->Connect(Label(1, c1), Label(1, c2), w);
	}
}

/*
* Helper for depthFirstTraversal
* @param stack The stack to search depth first with
* @param func The function to call on each Vertex
*/
void Graph::dfs(LabelStack* stack, Visit func) {

	while (!stack->empty()) {

		Label next = this->nextLabel(stack->top());

		if (next != Graph::NO_LABEL) {

			func(next);

			stack->push(next);

		} else {

			stack->pop();
		}
	}
}

/*
* Gets the next unvisited Vertex label adjacent to Vertex with given label
* @param label The label of the Vertex to get next from
* @return next unvisited Vertex label adjacent to Vertex
*/
Graph::Label Graph::nextLabel(const Label& label) {

	Vertex* curr = this->map[label];

	Label nextLabel = label;

	curr = (curr->isVisited()) ? this->nextVertex(curr->getNext(), &nextLabel):
		                         curr;

	nextLabel = (curr == nullptr) ? Graph::NO_LABEL : nextLabel;

	if (curr != nullptr) {

		curr->setVisited();
	}

	return nextLabel;
}

/*
* Helper for nextLabel
* Gets the next unvisited Vertex starting from curr
* @param curr The current Vertex
* @param nextLabel Pointer to store label of next unvisited Vertex
* @return next unvisited Vertexnext unvisited Vertex
*/
Graph::Vertex* Graph::nextVertex(Vertex* curr, Label* nextLabel) const {

	while (curr != nullptr) {

		Vertex* temp = this->map.at(curr->getLabel());

		if (!temp->isVisited()) {

			*nextLabel = curr->getLabel();

			curr = temp;

			break;
		}

		curr = curr->getNext();
	}

	return curr;
}

/*
* Helper for breadthFirstTraversal
* @param q The queue to search breadth first with
* @param func The function to call on each Vertex
*/
void Graph::bfs(LabelQ* q, Visit func) {

	while (!q->empty()) {

		Vertex* next = this->map[q->front()];

		func(q->front());

		q->pop();

		Graph::addUnvisited(q, next->getNext());
	}
}

/*
* Helper for bfsHelper, adds all unvisited adjacent Vertices to q
* @param q The queue to search breadth first with
* @param curr The Vertex to get unvisited adjacent Vertices from
*/
void Graph::addUnvisited(LabelQ* q, Vertex* curr) {

	while (curr != nullptr) {
		
		Vertex* temp = this->map[curr->getLabel()];

		if (!temp->isVisited()) {
			
			temp->setVisited();

			q->push(curr->getLabel());
		}

		curr = curr->getNext();
	}
}

/*
* Helper for Dijkstra, initializes WeightMap and PrevMap for all Vertices
* adjacent to the origin Vertex
* @param curr The origin Vertex
* @param label The label of the origin Vertex
* @param wMap The WeightMap
* @param pMap The PrevMap
*/
void Graph::initMaps(const Vertex* curr, const Label& label, WeightMap* wMap,
					 PrevMap* pMap) const {

	while (curr != nullptr) {

		(*wMap)[curr->getLabel()] = curr->getWeight();
		(*pMap)[curr->getLabel()] = label;

		curr = curr->getNext();
	}

	for (const LabelVert& pair : this->map) {

		if (!wMap->count(pair.first)) {

			(*wMap)[pair.first] = INT_MAX;
		}
	}
}

/*
* Helper for Dijkstra, iteratively gets the next smallest Vertex from
* WeightMap and updates any possible shorter paths from the origin
* @param set The VertexSet of visited Vertices
* @param wMap The WeightMap
* @param pMap The PrevMap
*/
void Graph::dijkstra(VertexSet* set, WeightMap* wMap, PrevMap* pMap) const {

	Label currLabel;

	Vertex* curr = nextSmallest(set, wMap, &currLabel);

	while (curr != nullptr) {

		for (const LabelWeight& pair : *wMap) {

			if (!set->count(this->map.at(pair.first))) {

				this->update(curr, pair, currLabel, wMap, pMap);
			}
		}

		set->insert(curr);

		curr = nextSmallest(set, wMap, &currLabel);
	}
}

/*
* Helper for dijkstra, gets the label for the next smallest Vertex from
* WeightMap thats not in VertexSet
* @param set The VertexSet
* @param wMap The WeightMap
* @param label The pointer to assign as the label of the next smallest Vertex
* @return pointer to the next smallest Vertex
*/
Graph::Vertex* Graph::nextSmallest(const VertexSet* set, const WeightMap* wMap,
	                               Label* label) const {

	Vertex* next = nullptr;

	Weight smallest = INT_MAX;

	for (const LabelWeight& pair : *wMap) {

		Vertex* temp = this->map.at(pair.first);

		if (!set->count(temp) && pair.second < smallest) {

			smallest = pair.second;

			next = temp;

			*label = pair.first;
		}
	}

	return next;
}

/*
* Helper for dijkstra, updates an entry in the WeightMap if there is a shorter
* path
* @param curr The current Vertex
* @param pair The entry in the WeightMap to update
* @param label The label of the current Vertex
* @param wMap The WeightMap
* @param pMap The PrevMap
*/
void Graph::update(const Vertex* curr, const LabelWeight& pair,
	               const Label& label, WeightMap* wMap, PrevMap* pMap) const {

	Vertex* adjacent = curr->getAdjacent(pair.first);

	if (adjacent != nullptr && (*wMap)[label] + adjacent->getWeight() <
		                                                 pair.second) {
	
		(*wMap)[pair.first] = (*wMap)[label] + adjacent->getWeight();
		(*pMap)[pair.first] = label;
	}
}


void Graph::cleanWeights(WeightMap* wMap) {

	for (const LabelVert& pair : this->map) {
		
		if ((*wMap)[pair.first] == INT_MAX ||
			(*wMap)[pair.first] == Graph::NO_WEIGHT) {
			
			wMap->erase(pair.first);
		}
	}
}

/*
* Helper for MinSpanningTree, gets the Vertices connected to the next
* minimum cost Edge
* @param curr The starting Vertex on the next minimum cost Edge
* @param next The ending Vertex on the next minimum cost Edge
* @param label The label of the starting Vertex on the next minimum cost Edge
*/
void Graph::nextMinEdge(Vertex*& curr, Vertex*& next, Label* label) {
	
	Weight minWeight = INT_MAX;

	for (const LabelVert& pair : this->map) {
		
		Vertex* temp = pair.second,
			  * tempNext = temp->getNext();
		
		while (temp->isVisited() && tempNext != nullptr) {
			
			if (tempNext->getWeight() < minWeight &&
				!this->map[tempNext->getLabel()]->isVisited()) {
				
				curr = temp;

				next = tempNext;

				minWeight = tempNext->getWeight();

				*label = pair.first;
			}

			tempNext = tempNext->getNext();
		}
	}

	if (minWeight != INT_MAX) {

		curr->setVisited();

		this->map[next->getLabel()]->setVisited();
		
	} else {

		curr = nullptr;
	}
}

/*
* Mark all verticies as unvisited
*/
void Graph::unvisitVertices() {

	for (const LabelVert& pair : this->map) {

		pair.second->setVisited(false);
	}
}

/*
* Constructs Vertex with given Label
* @param label The lable of the Vertex, default to blank
* @param weight The weight of the Vertex, default to -1
* @param next The next Vertex in the list
*/
Graph::Vertex::Vertex(const Label& label, const Weight& weight, Vertex* next,
	                  const int& edges)
	
	:label(label), weight(weight), visited(false), next(next), edges(edges) {
}

/*
* Copy constructor with pointers
* @param other The other Vertex to copy
*/
Graph::Vertex::Vertex(const Vertex& other)
	
	:label(other.label), weight(other.weight), visited(false), next(nullptr),
	edges(other.edges) {

	*this = other;
}

/*
* Assignment operator overload
* @param other The other Vertex to copy
* @return this by reference
*/
Graph::Vertex& Graph::Vertex::operator=(const Vertex& other) {

	Vertex* curr = this,
		  * nextCurr = other.next;

	while (nextCurr != nullptr) {

		curr->next = new Vertex(other.label, other.weight);

		nextCurr = nextCurr->next;
		curr = curr->next;
	}

	return *this;
}

/*
* Destroys Vertex
*/
Graph::Vertex::~Vertex() {}

/*
* Gets Vertex label
* @return Vertex label
*/
Graph::Label Graph::Vertex::getLabel() const {

	return this->label;
}

/*
* Gets Edge Weight (for adjacent Vertices)
* @return Edge Weight
*/
Graph::Weight Graph::Vertex::getWeight() const {

	return this->weight;
}

/*
* Returns true if Vertex visited, else false
* @return true if Vertex visited, else false
*/
bool Graph::Vertex::isVisited() const {
	
	return this->visited;
}

/*
* Sets the Vertex as visited or unvisited
* @param visited The boolean to indicated if visited(true) or
* unvisited(false)
*/
void Graph::Vertex::setVisited(bool visited) {

	this->visited = visited;
}

/*
* Gets the pointer to next Vertex in the adjacency list
* @retrun pointer to next Vertex in the adjacency list
*/
Graph::Vertex* Graph::Vertex::getNext() const {

	return this->next;
}

/*
* Sets next to new Vertex in the adjacency list
* @param pointer to next Vertex in the adjacency list to add
*/
void Graph::Vertex::setNext(Vertex* next) {

	this->next = next;
}

/*
* Counts all Edges for a Vertex
* @return number of Edges connected from Vertex
*/
int Graph::Vertex::numOfEdges() const {

	return this->edges;
}

/*
* Gets string representation of Edges & adjacent Vertices
* @return string representation of Edges & adjacent Vertices
*/
std::string Graph::Vertex::getEdges() const {

	std::string edges;

	Vertex* curr = this->next;

	while (curr != nullptr) {

		edges += curr->toString();

		if (curr->next != nullptr) {

			edges += Graph::COMMA;
		}

		curr = curr->next;
	}

	return edges;
}

/*
* Adds new Vertex with label in adjacent list
* Must maintain sorted list, Vertex cant connect to itself
* No duplicate Edges
* @param label The label to give the adjacent Vertex
* @param weight The weight to label the Edge
* @return true if connected, else false
*/
bool Graph::Vertex::connect(const Label& label, const Weight& weight) {
	
	bool connected(false);

	Vertex* curr = this;

	while (curr->next != nullptr && curr->next->label < label) {

		curr = curr->next;
	}

	if (curr->next == nullptr || curr->next->label != label) {

		Vertex* right = curr->next;

		curr->next = new Vertex(label, weight, right);

		++this->edges;

		connected = true;
	}

	return connected;
}

/*
* Deletes Vertex with label in adjacent list
* @param label The label of the adjacent Vertex to delete
* @return true if disconnected, else false
*/
bool Graph::Vertex::disconnect(const Label& label) {
	
	bool disconnected(false);

	Vertex* curr = this;

	while (curr->next != nullptr && curr->next->label != label) {

		curr = curr->next;
	}

	if (curr->next != nullptr) {

		Vertex* temp = curr->next->next;

		delete curr->next;

		curr->next = temp;

		--this->edges;

		disconnected = true;
	}

	return disconnected;
}

/*
* Gets the adjacent Vertex with given label
* or nullptr if it does not exist
* @param label The label of the adjacent Vertex
* @return pointer to adjacent Vertex or nullptr if it does not exist
*/
Graph::Vertex* Graph::Vertex::getAdjacent(const Label& label) const {

	Vertex* adjacent = this->next;

	while (adjacent != nullptr && adjacent->label != label) {

		adjacent = adjacent->next;
	}

	return adjacent;
}

/*
* Deallocate all adjacent Vertices
*/
void Graph::Vertex::clear() {

	Vertex* curr = this->next;

	while (curr != nullptr) {

		Vertex* temp = curr;

		curr = curr->next;

		delete temp;
	}
}

/*
* Makes string representation Vertex
* @return string Vertex
*/
std::string Graph::Vertex::toString() const {

	return this->label + Graph::LEFT_P + std::to_string(this->weight)
		+ Graph::RIGHT_P;
}
