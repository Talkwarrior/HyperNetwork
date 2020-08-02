/*
*	title: database for hypergraph
*	2020.06.18
*	developed by น้มพวๅ
*/

#include "hypergraph.h"

#include <iostream>
#include <cstdlib>
#include <io.h>

#include "ansi_escapes.h"
#include "parse.h"

extern HyperGraph* graph;

void trimString(const char* data) {
	int i = 0;
	while (i<GRAPH_TITLE_CAPACITY && data[i]!='\n') { i++; }
	if (i >= GRAPH_TITLE_CAPACITY) { std::cout << "Too Long!";  exit(8); }
	const_cast<char*>(data)[i] = '\0';
}

int main(void) {
	setupConsole();
	graph = NULL;

	std::cout << "Enter your Graph Title: " GREEN_TXT;
	char title[GRAPH_TITLE_CAPACITY];
	fgets(title, GRAPH_TITLE_CAPACITY, stdin);
	trimString(title);

	std::cout << WHITE_TXT;
	{
		char* fileName = (char*)malloc(sizeof(byte)*(GRAPH_TITLE_CAPACITY + 4));
		sprintf(fileName, "%s.jon", title);
		if (access(fileName, 0) != -1 && query("Seems The graph already exists. do you want to load it? ")) 
		{ 
			LoadGraphFromFile(&graph, fileName); 
		}
		else {
			if (createHyperGraph(&graph, title)) {
				// failed to create graph
				std::cout << "Failed to create Graph. Sorry.\n";
				return 0;
			}
		}
		free(fileName);
	}

	char command[100];
	char flag = 1;

	do {
		std::cout << "\n(" << graph->title << ")>>>" YELLOW_TXT;
		fgets(command, 100, stdin);
		std::cout << WHITE_TXT;
		flag = execute(command);
	} while (flag);

	restoreConsole();
	return 0;
}