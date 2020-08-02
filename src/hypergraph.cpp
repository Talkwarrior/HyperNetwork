#include "hypergraph.h"
#include "ansi_escapes.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

int createHyperGraph(HyperGraph** graph, const char* title) {
	if (*graph != NULL) {
		return 1;
	}

	*graph = (HyperGraph*)malloc(sizeof(HyperGraph));
	if (*graph == NULL) { NULL_Malloc_Exception(); return 1; }
	else {
		(*graph)->edgeCnt = 0;
		(*graph)->nodeCnt = 0;
		strcpy(const_cast<char*>((*graph)->title), title);

		// dummy node
		(*graph)->edge = (struct edgeList*)malloc(sizeof(struct edgeList));
		if ((*graph)->edge == NULL) { NULL_Malloc_Exception(); }

		(*graph)->edge->data = NULL;
		(*graph)->edge->next = NULL;

		(*graph)->node = (struct nodeList*)malloc(sizeof(struct nodeList));
		if ((*graph)->node == NULL) { NULL_Malloc_Exception(); }

		(*graph)->node->data = NULL;
		(*graph)->node->next = NULL;
		return 0;
	}
}

//search
struct nodeList* searchPreHyperNode(struct nodeList* head, const char* nodeName)
{
	struct nodeList* n = head;
	while (n->next)
	{
		switch (strcmp(n->next->data->name, nodeName))
		{
		case -1:
			n = n->next;
			break;
		case 0:
			return n;
		case 1:
			return NULL;
		}
	}
	return NULL;
}

struct edgeList* searchPreHyperEdge(struct edgeList* head, const char* edgeName)
{
	struct edgeList* e = head;
	while (e->next)
	{
		switch (strcmp(e->next->data->name, edgeName))
		{
		case -1:
			e = e->next;
			break;
		case 0:
			return e;
		case 1:
			return NULL;
		}
	}
	return NULL;
}

struct nodeList* searchHyperNode(struct nodeList* head, const char* nodeName) {
	struct nodeList* n = head;
	while (n->next != NULL) {
		switch (strcmp(n->next->data->name, nodeName)) {
		case -1:
			n = n->next;
			break;
		case 0:
			return n->next;
		case 1:
			return NULL;
		}
	}
	return NULL;
}

struct edgeList* searchHyperEdge(struct edgeList* head, const char* edgeName) {
	struct edgeList* e = head;
	while (e->next != NULL) {
		switch (strcmp(e->next->data->name, edgeName)) {
		case -1:
			e = e->next;
			break;
		case 0:
			return e->next;
		case 1:
			return NULL;
		}
	}
	return NULL;
}

int InsertHyperNodeByName(HyperGraph* graph, const char* NodeName) {
	if (graph == NULL) {
		return 1;
	}

	// create node at graph
	struct nodeList* parent = graph->node;
	struct nodeList* child = graph->node;
	char flag = 1;
	while (flag == 1 && parent->next != NULL) {
		switch (strcmp(parent->next->data->name, NodeName)) {
		case -1:
			parent = parent->next;
			break;
		case 0:		// node already exists
			return 1;
		case 1:
			flag = 0;
		}
	}

	// insert newEdge between par and n
	struct nodeList* newNode = (struct nodeList*)malloc(sizeof(struct nodeList));
	if (newNode == NULL) { NULL_Malloc_Exception(); return 1; }
	else {
		newNode->data = (HyperNode*)malloc(sizeof(HyperNode));
		newNode->data->edgeCnt = 0;
		newNode->data->edges = (struct edgeList*)malloc(sizeof(struct edgeList));
		newNode->data->edges->next = NULL;
		strcpy(const_cast<char*>(newNode->data->name), NodeName);

		newNode->next = parent->next;
		parent->next = newNode;

		graph->nodeCnt++;

		return 0;
	}
}

int InsertHyperEdgeByName(HyperGraph* graph, const char *EdgeName, int nodecnt, const char* node2connect[]) {
	if (graph == NULL) { return 1; }

	// insert edge in graph->edge
	struct edgeList* e = graph->edge;
	char flag = 1;
	while (flag == 1 && e->next != NULL) {
		switch (strcmp(e->next->data->name, EdgeName)) {
		case -1:
			e = e->next;
			break;
		case 0:
			return 1;
		case 1:
			flag = 0;
		}
	}

	struct edgeList* newEdge = (struct edgeList*)malloc(sizeof(struct edgeList));
	if (newEdge == NULL) { NULL_Malloc_Exception(); return 1; }
	else {
		newEdge->data = (HyperEdge*)malloc(sizeof(HyperEdge));
		if (newEdge->data == NULL) { NULL_Malloc_Exception(); }

		newEdge->data->nodeCnt = 0;
		newEdge->data->nodes = (struct nodeList*)malloc(sizeof(struct nodeList));
		if (newEdge->data->nodes == NULL) { NULL_Malloc_Exception(); }
		newEdge->data->nodes->next = NULL;
		strcpy(const_cast<char*>(newEdge->data->name), EdgeName);

		newEdge->next = e->next;
		e->next = newEdge;

		printf(CYAN_TXT "%s " WHITE_TXT "has successfully inserted\n", EdgeName);

		graph->edgeCnt++;

		printf(WHITE_BKG BLUE_TXT "connecting %d node...." BLACK_BKG"\n" WHITE_TXT, nodecnt);
		for (int i = 0; i < nodecnt; i++)
		{
			printf("\tConnect " MAGENTA_TXT "%s" WHITE_TXT " at " CYAN_TXT "%s: " WHITE_TXT, node2connect[i], EdgeName);
			connectEdge2Node(graph, EdgeName, node2connect[i]);
		}
		return 0;
	}	
}

// show
void showHyperGraph(HyperGraph* graph) {
	if (graph == NULL) { return; }
	printf("=============  Summarizing Data of <%s> =============\n", graph->title);
	struct nodeList* NodeCursor = graph->node;
	if (NodeCursor->next == NULL) {
		puts("Empty Graph");
		return;
	}
	printf("NodeCnt: %d, edgeCnt: %d\n", graph->nodeCnt, graph->edgeCnt);

	for (unsigned int iter = 0; iter < graph->nodeCnt; iter++)
	{
		NodeCursor = NodeCursor->next;
		printf("%s: ", NodeCursor->data->name);

		if (NodeCursor->data->edgeCnt == 0) {
			puts("None");
			continue;
		}
		puts("");

		struct edgeList* EdgeCursor = NodeCursor->data->edges;
		for (unsigned int jter = 0; jter < NodeCursor->data->edgeCnt; jter++)
		{
			EdgeCursor = EdgeCursor->next;
			printf("\t" CYAN_TXT" %s " WHITE_TXT "[ " MAGENTA_TXT, EdgeCursor->data->name);
			struct nodeList* nd = EdgeCursor->data->nodes;

			for (unsigned int zter = 0; zter < EdgeCursor->data->nodeCnt; zter++)
			{
				nd = nd->next;
				if (strcmp(nd->data->name, NodeCursor->data->name)) { printf("%s ", nd->data->name); }
			}
			fputs(WHITE_TXT "] \n", stdout);
		}
		puts("");
	}
}

void showHyperNode(HyperNode* node)
{
	if (node == NULL) { puts("Node not exist"); return; }
	printf("Node Name: " MAGENTA_TXT "%s\n" WHITE_TXT, node->name);

	struct edgeList* eg = node->edges;
	fputs("connected edges: [" CYAN_TXT, stdout);
	while (eg->next != NULL)
	{
		eg = eg->next;
		printf("%s ", eg->data->name);
	}
	fputs(WHITE_TXT "] ", stdout);
}

void showHyperEdge(HyperEdge* edge)
{
	if (edge == NULL) { puts("Edge not exist"); return; }
	printf("Edge Name: " CYAN_TXT" %s\n" WHITE_TXT, edge->name);

	struct nodeList* nd = edge->nodes;
	fputs("connected nodes: [ " MAGENTA_TXT, stdout);
	while (nd->next != NULL)
	{
		nd = nd->next;
		printf("%s ", nd->data->name);
	}
	fputs(WHITE_TXT "] ", stdout);
}

// clean
int cleanHyperGraph_Edges(HyperGraph* graph) {
	if (graph == NULL) { return -1; }

	int cnt = 0;
	int times = graph->edgeCnt;
	struct edgeList* e = graph->edge;

	while (e->next != NULL && times > 0) {
		if (e->next->data->nodeCnt == 0) {
			DeleteHyperEdgeByReference(graph, e->next->data);
			cnt++;
		}
		times--;
	}
	printf("%d edges are deleted\n", cnt);
	return cnt;
}

int cleanHyperGraph_Nodes(HyperGraph* graph) {
	if (graph == NULL) { return -1; }

	int cnt = 0;
	int times = graph->nodeCnt;
	struct nodeList* e = graph->node;
	struct nodeList* temp = NULL;
	while (e->next != NULL && times > 0) {
		if (e->next->data->edgeCnt == 0) {
			DeleteHyperNodeByReference(graph, e->next->data);
			cnt++;
		}
		times--;
	}
	printf("%d nodes are deleted", cnt);
	return cnt;
}

// delete
void DeleteHyperNodeByName(HyperGraph* graph, const char* NodeName)
{
	if (graph == NULL) { return; }
	printf("Delete HyperNode %s\n", NodeName);

	struct nodeList* n = searchPreHyperNode(graph->node, NodeName);

	if (n == NULL) { puts("Node Not Found"); return; }

	// deleted in graph
	struct nodeList* dnode = n->next;

	struct edgeList* e = dnode->data->edges;
	while (e->next != NULL)
	{
		printf("Disconnect node from " CYAN_TXT "%s\n" WHITE_TXT, e->next->data->name);
		disconnectNodeFromEdgeByName(graph, NodeName, e->next->data->name);
	}
	printf("Remove node " MAGENTA_TXT "%s" WHITE_TXT " from graph\n", NodeName);
	n->next = dnode->next;
	graph->nodeCnt--;
	free(dnode->data->edges);
	free(dnode->data);
	free(dnode);
}

void DeleteHyperNodeByReference(HyperGraph* graph, HyperNode* node)
{
	if (graph == NULL) { return; }
	printf("Delete HyperNode " MAGENTA_TXT "%s\n" WHITE_TXT, node->name);
	struct nodeList* n = searchPreHyperNode(graph->node, node->name);
	if (n == NULL) { puts("Invalid Node Reference."); return; }

	// deleted in graph
	struct nodeList* dnode = n->next;

	struct edgeList* e = dnode->data->edges;
	while (e->next != NULL)
	{
		printf("Disconnect node from " CYAN_TXT "%s\n" WHITE_TXT, e->next->data->name);
		disconnectNodeFromEdgeByName(graph, node->name, e->next->data->name);
	}
	printf("Remove node " MAGENTA_TXT "%s" WHITE_TXT " from graph\n", node->name);
	n->next = dnode->next;
	graph->nodeCnt--;
	free(dnode->data->edges);
	free(dnode->data);
	free(dnode);
}

void DeleteHyperEdgeByName(HyperGraph* graph, const char* EdgeName)
{
	if (graph == NULL) { return; }
	printf("Delete HyperEdge " CYAN_TXT "%s\n" WHITE_TXT, EdgeName);
	struct edgeList* preEdge = searchPreHyperEdge(graph->edge, EdgeName);
	if (preEdge == NULL) { printf("Can't find edge " CYAN_TXT "%s\n" WHITE_TXT, EdgeName); return; }

	// delete edge data from connected nodes
	struct nodeList* n = preEdge->next->data->nodes;
	while (n->next)
	{
		printf("Disconnect node " MAGENTA_TXT "%s\n" WHITE_TXT, n->next->data->name);
		disconnectNodeFromEdgeByReference(preEdge->next->data, n->next->data);
	}
	struct edgeList* dedge = preEdge->next;
	preEdge->next = dedge->next;
	graph->edgeCnt--;

	free(dedge->data->nodes);
	free(dedge->data);
	free(dedge);
}

void DeleteHyperEdgeByReference(HyperGraph* graph, HyperEdge* edge)
{
	if (graph == NULL) { return; }
	printf("Delete HyperEdge " CYAN_TXT "%s\n" WHITE_TXT, edge->name);
	struct edgeList* e = searchPreHyperEdge(graph->edge, edge->name);
	if (e == NULL) { printf("Can't find edge " CYAN_TXT "%s\n" WHITE_TXT, edge->name); return; }

	// delete edge data from connected nodes
	struct nodeList* n = e->next->data->nodes;
	while (n->next)
	{
		printf("Disconnect node " MAGENTA_TXT "%s\n" WHITE_TXT, n->next->data->name);
		disconnectNodeFromEdgeByReference(e->data, n->next->data);
	}
	struct edgeList* dedge = e->next;
	e->next = dedge->next;
	graph->edgeCnt--;

	free(edge->nodes);
	free(edge);
	free(dedge);
}

void freeHyperGraph(HyperGraph* graph)
{
	// free hypergraph.
	if (graph == NULL) { return; }

	// delete node data
	struct nodeList* tempnode = NULL; 
	for (struct nodeList* n = graph->node->next; n != NULL; free(tempnode) )
	{
		struct edgeList* temp = n->data->edges;
		while(n->data->edgeCnt--)
		{
			struct edgeList* e = temp->next;
			temp->next = temp->next->next;
			free(e);
		}
		free(n->data->edges);
		free(n->data);
		tempnode = n;
		n = n->next;
	}
	free(graph->node);

	// delete edge data;
	struct edgeList* tempedge = NULL;
	for (struct edgeList* e = graph->edge->next; e != NULL; free(tempedge))
	{
		struct nodeList* temp = e->data->nodes;
		while (e->data->nodeCnt--)
		{
			struct nodeList* n = temp->next;
			temp->next = temp->next->next;
			free(n);
		}
		free(e->data->nodes);
		free(e->data);
		tempedge = e;
		e = e->next;
	}
	free(graph->edge);
	free(graph);
}

void renameObject(HyperGraph* graph, const char* name, const char* newName, int type)
{
	// type(0: graph, 1: node, 2: edge)
	switch (type) 
	{
	case 0:
		printf("Rename Graph");
		strcpy(const_cast<char*>(graph->title), newName);
		return;
	case 1:
	{
		struct nodeList* n = searchHyperNode(graph->node, name);
		if (n == NULL) { puts("Node not found"); return; }
		printf("Rename Node " MAGENTA_TXT "%s => %s\n" WHITE_TXT, name, newName);
		strcpy(const_cast<char*>(n->data->name), newName);
	}
	break;
	case 2:
	{
		struct edgeList* e = searchHyperEdge(graph->edge, name);
		if (e == NULL) { puts("Edge not found"); return; }
		printf("Rename Edge " CYAN_TXT "%s => %s\n" WHITE_TXT, name, newName);
		strcpy(const_cast<char*>(e->data->name), newName);
	}
	}
}