#include "hypergraph.h"
#include "ansi_escapes.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

// connecting existing node and existing edge.
int connectEdge2Node(HyperGraph* graph, const char* edgeName, const char* nodeName) {
	if (graph == NULL) { return 1; }
	// puts("#1: Find Edge in Graph");
	// find edge in graph
	struct edgeList* e = searchHyperEdge(graph->edge, edgeName);
	if (e == NULL) { puts("Error: invalid edgeName"); return 1; }
	
	// puts("#2: Find Node in Graph->Nodes");
	// find node in graph->nodes
	struct nodeList* gn = searchHyperNode(graph->node, nodeName);
	if (gn == NULL) { puts("Error: invalid nodeName"); return 1; }	// invalid node

	// puts("#3: Insert Gn Data");
	// insert gn data into e->nodes.
	struct nodeList* n = e->data->nodes;
	char flag = 1;
	while (flag == 1 && n->next != NULL) { 
		switch (strcmp(n->next->data->name, nodeName)) {
		case -1:
			n = n->next; 
			break;
		case 0:
			return 1;
		case 1:
			flag = 0;
		}
	}
	
	struct nodeList* newNode = (struct nodeList*)malloc(sizeof(struct nodeList));
	newNode->data = gn->data;
	
	newNode->next = n->next;
	n->next = newNode;

	e->data->nodeCnt++;
	gn->data->edgeCnt++;

	// puts("#4: Add Edge Data into Gn->edges");
	// gn->data->edges에 e->data 추가하기.
	struct edgeList* parent = gn->data->edges;
	flag = 1;
	while (flag && parent->next != NULL) {
		switch (strcmp(parent->next->data->name, edgeName))
		{
		case -1:
			parent = parent->next;
			break;
		case 0:
			return 0;
		case 1:
			flag = 0;
		}
	}

	struct edgeList* newNodeElem = (struct edgeList*)malloc(sizeof(struct edgeList));
	newNodeElem->data = e->data;
	newNodeElem->next = parent->next;
	parent->next = newNodeElem;
	
	puts(YELLOW_TXT "Success" WHITE_TXT);
	return 0;
}

int disconnectNodeFromEdgeByReference(HyperEdge* edge, HyperNode* node)
{
	if (edge == NULL || node == NULL) { return 1; }

	// find node in edge's nodes
	struct nodeList* parent = searchPreHyperNode(edge->nodes, node->name);
	if (parent ==  NULL) { printf("Node %s is not connected to edge %s\n", node->name, edge->name); return 1; }
	
	
	struct edgeList* e = searchPreHyperEdge(parent->next->data->edges, edge->name);
	if (e == NULL) { printf("Node %s is not connected to edge %s\n", node->name, edge->name); return 1; }

	struct edgeList* echild = e->next;
	e->next = echild->next;
	echild->data->nodeCnt--;
	struct nodeList* child = parent->next;
	parent->next = child->next;
	child->data->edgeCnt--;

	free(child);
	free(echild);

	puts(YELLOW_TXT "Success" WHITE_TXT);
	return 0;
}

int disconnectNodeFromEdgeByName(HyperGraph* graph, const char* nodeName, const char* edgeName) {
	if (graph == NULL) { return 1; }
	
	// find edge in graph->edge
	struct edgeList* edge = searchHyperEdge(graph->edge, edgeName);
	if (edge == NULL) { return 1; }

	// find node in edge's nodes
	struct nodeList* preNode = searchPreHyperNode(edge->data->nodes, nodeName);
	if (preNode == NULL) { printf("%s is not connected to %s\n", nodeName, edgeName); return 1; }

	struct nodeList* disconnectNode = preNode->next;
	preNode->next = preNode->next->next;
	edge->data->nodeCnt--;
	
	// delete edge data in node
	struct edgeList* e = searchPreHyperEdge(disconnectNode->data->edges, edgeName);
	
	struct edgeList* disconnectEdge = e->next;
	e->next = disconnectEdge->next;
	disconnectNode->data->edgeCnt--;

	free(disconnectNode);
	free(disconnectEdge);

	// don't care about empty edge.
	return 0;
}