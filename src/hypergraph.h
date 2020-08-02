#pragma once

#define NODE_NAME_CAPACITY 50
#define EDGE_NAME_CAPACITY 50
#define GRAPH_TITLE_CAPACITY 50

typedef struct _edge HyperEdge;

typedef struct _node {
	const char name[NODE_NAME_CAPACITY];
	struct edgeList* edges;
	unsigned int edgeCnt;
}HyperNode;

struct _edge {
	const char name[EDGE_NAME_CAPACITY];
	struct nodeList* nodes;
	unsigned int nodeCnt;
};

struct nodeList {
	HyperNode* data;
	struct nodeList* next;
};

struct edgeList {
	HyperEdge* data;
	struct edgeList* next;
};

// stores graph.
// node, edge will be the head of linkedlist.
typedef struct _graph{
	const char title[GRAPH_TITLE_CAPACITY];

	unsigned int nodeCnt;
	struct nodeList* node;

	unsigned int edgeCnt;
	struct edgeList* edge;
}HyperGraph;

// hypergraph.c
/*
*	void NULL_Malloc_Exception(void)
*		excepts an error when malloc fails.
*/
void NULL_Malloc_Exception(void);

/*
*	int createHyperGraph(HyperGraph**, const char*)
*	
*	function:
*		create an empty hypergraph.
*		returns 0 when success, returns 1 when fails.
*
*	argument:
*		HyperGraph** : a reference of pointer to point a new graph.
*		const char* : title
*/
int createHyperGraph(HyperGraph**, const char*);

/*
*	struct nodeList* searchPreHyperNode(struct nodeList*, const char*);
*
*	function:
*		returns preNode: right before the target node.
*
*	argument:
*		struct nodeList* : a head node of nodelist.
*		const char* : name of target node
*/
struct nodeList* searchPreHyperNode(struct nodeList*, const char*);

/*
*	struct edgeList* searchPreHyperEdge(struct edgeList*, const char*);
*
*	function:
*		returns preEdge: right before the target edge.
*
*	argument:
*		struct edgeList* : a head node of edgelist.
*		const char* : name of target edge
*/
struct edgeList* searchPreHyperEdge(struct edgeList*, const char*);

/*
*	struct nodeList* searchHyperNode(struct nodeList*, const char*);
*
*	function:
*		search and returns hypernode's list reference.
*
*	argument:
*		struct nodeList* : a head node of nodelist.
*		const char* : name of target node
*/
struct nodeList* searchHyperNode(struct nodeList*, const char*);

/*
*	struct edgeList* searchHyperEdge(struct edgeList*, const char*);
*
*	function:
*		search and returns hyperedge's list reference.
*
*	argument:
*		struct edgeList* : a head node of nodelist.
*		const char* : name of target edge
*/
struct edgeList* searchHyperEdge(struct edgeList*, const char*);

/*
*	int InsertHyperNodeByName(HyperGraph*, const char*)
*
*	function:
*		insert a node into hypergraph.
*		check if the node exists.
*		returns 0 when created newNode, else returns 1.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		const char* : name of new node.
*
*/
int InsertHyperNodeByName(HyperGraph*, const char*);

/*
*	int InsertHyperEdge(HyperGraph*, const char*, int, const char* [])
*
*	function:
*		insert an edge into hypergraph and connects given nodes.
*		check if the node and edge exists.
*		create just edge not nodes.
*		returns 0 when success, returns 1 when fails.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		const char* : name of new edge
*		int			: number of nodes to connect new edge.
*		const char* [] : name of nodes to connect new edge.
*
*/
int InsertHyperEdgeByName(HyperGraph*, const char*, int, const char* []);

/*
*	void DeleteHyperNodeByName(HyperGraph*, const char*)
*
*	function:
*		deletes hypernode with name.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		const char* : name of node to delete
*
*/
void DeleteHyperNodeByName(HyperGraph*, const char*);

/*
*	void DeleteHyperNodeByReference(HyperGraph*, HyperNode*)
*
*	function:
*		deletes hypernode with reference.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		HyperNode*  : reference of node to delete
*
*/
void DeleteHyperNodeByReference(HyperGraph*, HyperNode*);

/*
*	void DeleteHyperEdgeByName(HyperGraph*, const char*)
*
*	function:
*		deletes hyperedge with name.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		const char* : name of edge to delete
*
*/
void DeleteHyperEdgeByName(HyperGraph*, const char*);

/*
*	void DeleteHyperNodeByReference(HyperGraph*, const char*)
*
*	function:
*		deletes hyperedge with reference.
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		HyperEdge*	: reference of edge to delete
*
*/
void DeleteHyperEdgeByReference(HyperGraph*, HyperEdge*);

/*
*	show functions	: displays given objects.
*/
void showHyperGraph(HyperGraph*);
void showHyperNode(HyperNode*);
void showHyperEdge(HyperEdge*);

/*
*	clean functions	: cleans alone node/edges.
*	* alone: nothing connected.
*/
int cleanHyperGraph_Edges(HyperGraph*);
int cleanHyperGraph_Nodes(HyperGraph*);

// just free all data of hypergraph.
void freeHyperGraph(HyperGraph*);

/*
*	void renameObject(HyperGraph*, const char*, const char*, int)
*
*	function:
*		rename seleted object(graph/node/edge).
*
*	argument:
*		HyperGraph* : a pointer to point a graph.
*		const char* : current name of object
*		const char* : new name of object
*		int			: type(0: graph, 1: node, 2: edge)
*
*/
void renameObject(HyperGraph*, const char*, const char*, int);

/*
*	[dataQuery.cpp]
*	functions to query data of hypergraph, for social network analysis.
*/
double get_affinity(HyperGraph*, const char*, const char*);
double get_density(HyperGraph*);
int get_inclusiveness(HyperGraph*);	
double get_centrality(HyperGraph*, const char*);

// hyperEdge.c
/*
*	functions that related to edge-domain connection.
*/
int connectEdge2Node(HyperGraph* graph, const char* edgeName, const char* nodeName);
int disconnectNodeFromEdgeByReference(HyperEdge*, HyperNode*);
int disconnectNodeFromEdgeByName(HyperGraph* graph, const char* nodeName, const char* edgeName);

// hyperFile.c
// file support
typedef char byte;

struct _fhead {
	byte MWORD[2];
	byte title[GRAPH_TITLE_CAPACITY];
	unsigned int EdgeCnt;
	unsigned int EmptyNodeCnt;
};

struct _fedge {
	byte name[EDGE_NAME_CAPACITY];
	unsigned int nodeCnt;
	byte** node;
};

struct _fnode {
	byte name[NODE_NAME_CAPACITY];
};

typedef struct _fgraph {
	// headers
	struct _fhead *header;

	// edge data
	struct _fedge *edges;

	// non-connected node data
	struct _fnode *nodes;
}HyperGraphFile;

/*
*	int SaveGraph2File(HyperGraph* graph, const char* fileName)
*
*	function:
*		save graph to file. the format should be .graph
*		returns 0 when success, returns 1 when fails
*
*	argument:
*		HyperGraph* graph: a pointer to point a graph.
*		const char* fileName : name of file.
*
*/
int SaveGraph2File(HyperGraph* graph, const char* fileName);

/*
*	int LoadGraphFromFile(HyperGraph* graph, const char* fileName)
*
*	function:
*		load graph from file.
*		if load fails, return 1;
*		-> graph should be NULL. or it will fail.
*
*	argument:
*		HyperGraph* graph: a pointer to point a graph.
*		char fileName[50]: name of file.
*
*/
int LoadGraphFromFile(HyperGraph** graph, const char *fileName);
