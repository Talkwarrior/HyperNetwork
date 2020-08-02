#include "hypergraph.h"
#include "ansi_escapes.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <cstring>

int SaveGraph2File(HyperGraph* graph, const char* fileName)
{
	// check file exist
	/*
		io.h 의 access 함수에 대하여.
				06  읽고 쓰기 허가 검사
				04  읽기 허가 검사
				02  쓰기 허가 검사
				01  실행(ignored)
				00  파일 존재여부를 검사
		출처: http://blog.daum.net/juju2for/37
	*/
	if(access(fileName, 0) != -1)
	{
		// file exists
		std::cout << "Looks Like " << fileName <<  " is already exists. Do you want to overwrite it? (Y/N)";
		char answer = getchar();
		if (answer != 'Y' && answer != 'y') 
		{
			std::cout << "Canceled Save Command.\n";
			return 1;
		}
	}
	FILE* outStream = fopen(fileName, "w");

	// write header
	struct _fhead* header = (struct _fhead*)malloc(sizeof(struct _fhead));
	if (header == NULL) { NULL_Malloc_Exception(); }
	strcpy(header->MWORD, "HG");
	strcpy(header->title, graph->title);
	header->EdgeCnt = graph->edgeCnt;
	header->EmptyNodeCnt = 0;
	for (struct nodeList* n = graph->node; n->next != NULL; n = n->next) {
		if (n->next->data->edgeCnt == 0) {
			header->EmptyNodeCnt++;
		}
	}

	fwrite((void*)header, sizeof(struct _fhead), 1, outStream);
	free(header);

	// init edge
	for (struct edgeList* e = graph->edge; e->next != NULL; e = e->next)
	{
		HyperEdge* edge = e->next->data;
		
		fwrite((void*)edge->name, sizeof(byte), EDGE_NAME_CAPACITY, outStream);
		fwrite((void*)&edge->nodeCnt, sizeof(unsigned int), 1, outStream);
		for (struct nodeList* node = edge->nodes; node->next!=NULL; node=node->next)
		{
			fwrite((void*)node->next->data->name, sizeof(byte), NODE_NAME_CAPACITY, outStream);
		}
	}

	// write empty nodes(edgeCnt==0)
	for (struct nodeList* n = graph->node; n->next != NULL ; n=n->next)
	{
		if (n->next->data->edgeCnt == 0) { 
			HyperNode* node = n->next->data;
			fwrite(node->name, sizeof(byte), NODE_NAME_CAPACITY, outStream);
		}
	}

	fclose(outStream);
	return 0;
}

int LoadGraphFromFile(HyperGraph** graph, const char* fileName)
{
	if (*graph != NULL) { std::cout << "Graph is not NULL!" << std::endl; exit(1); }
	if (access(fileName, 0) == -1)
	{
		// file not exists
		printf("There is no file called: %s\n", fileName);
		return 1;
	}
	FILE* inStream = fopen(fileName, "r");

	// read data from file and save it
	// header
	HyperGraphFile* gf = (HyperGraphFile*)malloc(sizeof(HyperGraphFile));
	if (gf == NULL) { NULL_Malloc_Exception(); }

	gf->header = (struct _fhead*)malloc(sizeof(struct _fhead));
	if (gf->header == NULL) { NULL_Malloc_Exception(); }

	fread((void*)gf->header, sizeof(struct _fhead), 1, inStream);
	if (strncmp(gf->header->MWORD, "HG", 2)) { 
		printf(RED_TXT "Invalid File, MWORD is %s\n" WHITE_TXT, gf->header->MWORD); 
		return 1; 
	}

	// edges
	gf->edges = (struct _fedge*)malloc(sizeof(struct _fedge)*gf->header->EdgeCnt);
	if (gf->edges == NULL) { NULL_Malloc_Exception(); }

	for (unsigned int i = 0; i < gf->header->EdgeCnt; i++)
	{
		fread((void*)gf->edges[i].name, sizeof(byte), EDGE_NAME_CAPACITY, inStream);
		fread((void*)&gf->edges[i].nodeCnt, sizeof(unsigned int), 1, inStream);
		gf->edges[i].node = (byte**)malloc(sizeof(byte*)*gf->edges[i].nodeCnt);
		if (gf->edges[i].node == NULL) { NULL_Malloc_Exception(); }
		for (unsigned int node = 0; node < gf->edges[i].nodeCnt; node++)
		{
			gf->edges[i].node[node] = (byte*)malloc(sizeof(byte)*NODE_NAME_CAPACITY);
			if (gf->edges[i].node[node] == NULL) { NULL_Malloc_Exception(); }
			fread(gf->edges[i].node[node], sizeof(byte), NODE_NAME_CAPACITY, inStream);
		}
	}
	// read empty nodes
	gf->nodes = (struct _fnode*)malloc(sizeof(struct _fnode)*gf->header->EmptyNodeCnt);
	if (gf->nodes == NULL) { NULL_Malloc_Exception(); }
	for (unsigned int i = 0; i < gf->header->EmptyNodeCnt; i++)
	{
		fread(gf->nodes[i].name, sizeof(byte), NODE_NAME_CAPACITY, inStream);
	}
	fclose(inStream);

	// fill graph
	if (createHyperGraph(graph, gf->header->title) != 0) { puts(RED_TXT "Failed to Create graph.." WHITE_TXT); return 1; };
	for (unsigned int edge = 0; edge < gf->header->EdgeCnt; edge++)
	{
		for (unsigned int node = 0; node < gf->edges[edge].nodeCnt; node++)
		{
			InsertHyperNodeByName(*graph, gf->edges[edge].node[node]);
		}
		InsertHyperEdgeByName(*graph, gf->edges[edge].name, gf->edges[edge].nodeCnt, const_cast<const char**>(gf->edges[edge].node));
		
		// free edge data(read from file)
		for (unsigned int  node = 0; node < gf->edges[edge].nodeCnt; node++)
		{
			free(gf->edges[edge].node[node]);
		}
		free(gf->edges[edge].node);
	}
	for (unsigned int node = 0; node < gf->header->EmptyNodeCnt; node++)
	{
		InsertHyperNodeByName(*graph, gf->nodes[node].name);
	}
	std::cout << YELLOW_BKG "Created Graph!!!" << BLACK_BKG;
	// free gf
	free(gf->header);
	free(gf->nodes);

	free(gf->edges);
	return 0;
}