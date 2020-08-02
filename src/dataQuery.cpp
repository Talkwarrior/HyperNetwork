#include "hypergraph.h"
#include "ansi_escapes.h"
#include <iostream>
#include <set>
#include <cstdio>


void NULL_Malloc_Exception(void) {
	puts(RED_TXT "Malloc Failed." WHITE_TXT);
	exit(1);
}


double get_affinity(HyperGraph* graph, const char* node1, const char* node2)
{
	void* n1 = searchHyperNode(graph->node, node1);
	void* n2 = searchHyperNode(graph->node, node2);
	if (n1 == NULL || n2 == NULL) { return -1; }
	n1 = ((struct nodeList*)n1)->data;
	n2 = ((struct nodeList*)n2)->data;

	struct edgeList* e1 = ((HyperNode*)n1)->edges;
	struct edgeList* e2 = ((HyperNode*)n2)->edges;

	unsigned int connection = 0;
	while(e1->next && e2->next)
	{
		switch (strcmp((const char*)e1->next->data->name, e2->next->data->name))
		{
		case -1:
			e1 = e1->next;
			break;
		case 0:
			connection++;
			e1 = e1->next;
			e2 = e2->next;
			break;
		case 1:
			e2 = e2->next;
		}
	}
	if (connection == 0) { return 0; } // excepts divide by zero
	double af1 = connection / ((HyperNode*)n1)->edgeCnt;
	double af2 = connection / ((HyperNode*)n2)->edgeCnt;
	return (af1 + af2) / 2;
}

unsigned int get_total_connection(HyperGraph* graph)
{
	unsigned int result = 0;
	for (struct edgeList* e = graph->edge; e->next != NULL; e = e->next)
	{
		result += (e->next->data->nodeCnt+1)*e->next->data->nodeCnt / 2;
	}
	return result;
}

// maximum_connection - total_connection
double get_density(HyperGraph* graph)
{
	unsigned int tot_con = get_total_connection(graph);
	unsigned int max_con = graph->edgeCnt*(graph->nodeCnt + 1)*(graph->nodeCnt) / 2;
	printf("%u/%u\n", tot_con, max_con);
	return (double)tot_con / max_con;
}

// nodecnt - UnConnectNodeCnt
int get_inclusiveness(HyperGraph* graph)
{
	int result = graph->nodeCnt;
	for (struct nodeList* n = graph->node; n ->next != NULL; n = n->next)
	{
		if (n->next->data->edgeCnt == 0) { result--; }
	}

	return result;
}

// connected_node / tot_node_cnt
double get_centrality(HyperGraph* graph, const char* nodeName)
{
	void* node = searchHyperNode(graph->node, nodeName);
	if (node == NULL) { return -1; }
	node = ((struct nodeList*)node)->data;

	struct edgeList* edgeL = ((HyperNode*)node)->edges;
	std::set<const char*> connected_node;

	while (edgeL->next)
	{
		for (struct nodeList* n = edgeL->next->data->nodes; n->next; n = n->next) 
		{
			connected_node.insert(n->next->data->name);
		}
		edgeL = edgeL->next;
	}

	return static_cast<double>(connected_node.size()) / graph->nodeCnt;
}