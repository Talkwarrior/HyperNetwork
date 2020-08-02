#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cctype>

#include "parse.h"
#include "ansi_escapes.h"

// global variable to control graph
HyperGraph* graph;

typedef enum {
	CREATE = 0,
	DELETE,
	SEARCH,
	CONNECT,
	DISCONNECT,
	RENAME,
	GET,
	SAVE,
	LOAD,
	CLEAN,
	DISPLAY,
	HELP,
	EXIT,
	OTHER
}CommandKind;


int query(const char *question)
{
	printf("%s (Y/N) " GREEN_TXT, question);
	char c = getchar();
	getchar();		// read '\n'
	printf(WHITE_TXT);
	return c == 'Y' || c == 'y';
}

static void lower(const char* string) {
	// change string to lowercase
	long len = strlen(string);
	for (int i = 0; i < len; i++)
	{
		const_cast<char*>(string)[i] = tolower(string[i]);
	}
}

static CommandKind getCommandKind(const char* command)
{
	lower(command);

	// compare keyword
	if (!strcmp(command, "create") || !strcmp(command, "new")) { return CREATE; }
	else if (!strcmp(command, "delete")) { return DELETE; }
	else if (!strcmp(command, "search")) { return SEARCH; }
	else if (!strcmp(command, "connect")) { return CONNECT; }
	else if (!strcmp(command, "disconnect")) { return DISCONNECT; }
	else if (!strcmp(command, "rename")) { return RENAME; }
	else if (!strcmp(command, "get")) { return GET; }
	else if (!strcmp(command, "save")) { return SAVE; }
	else if (!strcmp(command, "load")) { return LOAD; }
	else if (!strcmp(command, "clean")) { return CLEAN; }
	else if (!strcmp(command, "display") || !strcmp(command, "show")) { return DISPLAY; }
	else if (!strcmp(command, "help")) { return HELP; }
	else if (!strcmp(command, "exit")) { return EXIT; }
	else { return OTHER; }
}

static void showHelpMessage(CommandKind kind) {
	switch (kind)
	{
	case CREATE:
		puts("-----------------------------------------------");
		puts("	Examples: ");
		puts("		create node [names]");
		puts("		create edge [edgeName] [node2connect]");
		puts("-----------------------------------------------");
		break;
	case DELETE:
		puts("-----------------------------------------------");
		puts("	Examples: ");
		puts("		delete node [name]");
		puts("		delete edge [name]");
		puts("-----------------------------------------------");
		break;
	case SEARCH:
		puts("-----------------------------------------------");
		puts("	Search Function Examples: ");
		puts("		search node [name]");
		puts("		search edge [name]");
		puts("-----------------------------------------------");
		break;
	case CONNECT:
		puts("-----------------------------------------------");
		puts("	Connect Function Examples: ");
		puts("		connect [edgename] [nodename]");
		puts("               -> connects edge to node");
		puts("-----------------------------------------------");
		break;
	case DISCONNECT:
		puts("-----------------------------------------------");
		puts("	Disconnect Function Examples: ");
		puts("		disconnect [nodename] [edgename]");
		puts("               -> disconnects node from edge");
		puts("-----------------------------------------------");
		break;
	case RENAME:
		puts("-----------------------------------------------");
		puts("	Rename Function Examples: ");
		puts("		rename graph [newname]");
		puts("		rename node [nodename] [newname]");
		puts("		rename edge [edgename] [newname]");
		puts("               -> renames selected object");
		puts("-----------------------------------------------");
		break;
	case GET:
		puts("-----------------------------------------------");
		puts("	Get Function Examples: ");
		puts("		get affinity [nodename1] [nodename2]    => shows affinity between two nodes");
		puts("		get density                             => shows density of this graph.");
		puts("		get inclusiv                            => shows number of nodes that are connected at least one edge.");
		puts("		get centrality [nodename]			    => shows the node's centrality.");
		puts("-----------------------------------------------");
		break;
	case SAVE:
		puts("-----------------------------------------------");
		puts("	Save Function Examples: ");
		puts("		save [filename]");
		puts("               -> saves current graph");
		puts("-----------------------------------------------");
		break;
	case LOAD:
		puts("-----------------------------------------------");
		puts("	Load Function Examples: ");
		puts("		load [filename]");
		puts("               -> saves current graph and load new graph");
		puts("-----------------------------------------------");
		break;
	case CLEAN:
		puts("-----------------------------------------------");
		puts("	Clean Function Examples: ");
		puts("		clean node ");
		puts("		clean edge ");
		puts("             -> deletes node/edges that are not connected.");
		puts("-----------------------------------------------");
		break;
	case DISPLAY:
		puts("shows graph");
		break;
	case EXIT:
		puts("exit program");
		break;
	case HELP:
	default:
		puts("------------------------------------------------------------------------------------------------");
		puts("	HyperGraph database command help:");
		puts("		create|new:					create node or edge");
		puts("		delete:						delete node or edge");
		puts("		connect:					connect node and edge");
		puts("		disconnect:					disconnect node and edge");
		puts("		rename:						renames node|edge|graph");
		puts("		get:");
		puts("			affinity [nodename1] [nodename2]	shows affinity between two nodes");
		puts("			density					shows density of this graph.");
		puts("			inclusiv				shows number of nodes that are connected");
		puts("			centrality [nodename]			shows the node's centrality");
		puts("		save:						save graph to file");
		puts("		load:						load graph from file");
		puts("		clean:						remove empty edge or nodes");
		puts("		display | show:					show graph");
		puts("		help:						show this message");
		puts("------------------------------------------------------------------------------------------------");
	}
}

// create node [names]
// create edge [edgeName] [node2connect]
static void command_Create(const char* zip[], int cnt)
{
	if (cnt <= 1)
	{
		showHelpMessage(CREATE);
		return;
	}

	lower(zip[1]);
	if (!strcmp(zip[1], "node"))
	{
		for (int i = 2; i <= cnt; i++)
		{
			printf("Create Node " MAGENTA_TXT "%s\n" WHITE_TXT, zip[i]);
			InsertHyperNodeByName(graph, zip[i]);
		}
		puts("done");
	}
	else if (!strcmp(zip[1], "edge"))
	{
		fputs("create edge ", stdout);
		printf("%s [", zip[2]);
		for (int i = 3; i <= cnt; i++) { printf("%s ", zip[i]); }
		puts("]");
		InsertHyperEdgeByName(graph, zip[2], cnt - 2, &zip[3]);
		puts("done");
	}
	else
	{
		printf("invalid action: %s\n", zip[1]);
		return;
	}
}

static void command_Delete(const char* zip[], int cnt)
{
	if (cnt <= 1)
	{
		showHelpMessage(DELETE);
		return;
	}

	lower(zip[1]);
	if (!strcmp(zip[1], "node"))
	{
		DeleteHyperNodeByName(graph, zip[2]);
	}
	else if (!strcmp(zip[1], "edge"))
	{
		DeleteHyperEdgeByName(graph, zip[2]);
	}
}

static void command_Search(const char* zip[], int cnt)
{
	if (cnt <= 1)
	{
		showHelpMessage(SEARCH);
		return;
	}

	lower(zip[1]);
	if (!strcmp(zip[1], "node"))
	{
		struct nodeList* n = searchHyperNode(graph->node, zip[2]);
		if (n == NULL) { puts("Node Not Found"); return; }
		showHyperNode(n->data);
	}
	else if (!strcmp(zip[1], "edge"))
	{
		struct edgeList* e = searchHyperEdge(graph->edge, zip[2]);
		if (e == NULL) { puts("Edge Not Found"); return; }
		showHyperEdge(e->data);
	}
	else {
		showHelpMessage(SEARCH);
	}
}

// connect [edgename] [nodename]
static void command_Connect(const char* zip[], int cnt)
{
	if (cnt != 2)
	{
		showHelpMessage(CONNECT);
		return;
	}
	connectEdge2Node(graph, zip[1], zip[2]);
}

// disconnect [nodename] [edgename]
static void command_Disconnect(const char* zip[], int cnt)
{
	if (cnt != 2) {
		showHelpMessage(DISCONNECT);
		return;
	}
	printf("Disconnect " MAGENTA_TXT "%s" WHITE_TXT " from " CYAN_TXT "%s\n" WHITE_TXT, zip[2], zip[1]);
	disconnectNodeFromEdgeByName(graph, zip[1], zip[2]);
}

static void command_Save(const char* zip[], int cnt)
{
	char *fName = NULL;
	if (cnt < 1)
	{
		fName = (char*)malloc(sizeof(char)*(GRAPH_TITLE_CAPACITY + 4));
		if (fName == NULL) { NULL_Malloc_Exception(); }
		sprintf(fName, "%s.jon", graph->title);
	}
	else
	{
		fName = const_cast<char*>(zip[1]);
	}
	puts("Save Graph...");
	if (SaveGraph2File(graph, fName) == 0)
	{
		printf("Successfully Saved File %s", fName);
	}
	else
	{
		puts("Failed to save graph.");
	}
}

static void command_Load(const char* zip[], int cnt)
{
	if (cnt < 1)
	{
		showHelpMessage(LOAD);
		return;
	}
	if (query("Do you want to save current graph? "))
	{
		printf("File Name : " GREEN_TXT);
		char* saveFname = (char*)malloc(sizeof(const char) * 255);
		if (saveFname == NULL) { NULL_Malloc_Exception(); }

		getchar();
		fgets(saveFname, 255, stdin);
		saveFname[strlen(saveFname) - 1] = '\0';
		printf(WHITE_TXT);
		SaveGraph2File(graph, saveFname);
	}

	printf("Loading Graph From %s...\n", zip[1]);
	HyperGraph* newGraph = NULL;
	if (LoadGraphFromFile(&newGraph, zip[1]) == 0)
	{
		freeHyperGraph(graph);
		puts("Whole Graph Memory Freed");
		graph = newGraph;
		puts("Successfully Loaded Graph");
	}
	else
	{
		puts("Failed To Load Graph");
	}
}
static void command_Clean(const char* zip[], int cnt) {
	if (cnt == 0) {
		showHelpMessage(CLEAN);
		return;
	}
	lower(zip[1]);
	if (!strcmp(zip[1], "node")) { cleanHyperGraph_Nodes(graph); }
	else if (!strcmp(zip[1], "edge")) { cleanHyperGraph_Edges(graph); }
}

enum getKind {
	AFFINITY = 0,
	DENSITY,
	INCLUSIVENESS,  // 포괄성
	CENTRALITY
};

static int get_GetCommandKind(const char* function)
{
	if (!strcmp(function, "affinity")) { return AFFINITY; }
	else if (!strcmp(function, "density")) { return DENSITY; }
	else if (!strcmp(function, "inclusiv")) { return INCLUSIVENESS; }
	else if (!strcmp(function, "centrality")) { return CENTRALITY; }
	else { return -1; }
}


static void command_Get(const char* zip[], int cnt)
{
	if (cnt == 0)
	{
		// show help message
		showHelpMessage(GET);
		return;
	}
	lower(zip[1]);
	switch (get_GetCommandKind(zip[1]))
	{
	case AFFINITY:
	{
		// 친밀도
		double affinity = get_affinity(graph, zip[2], zip[3]);
		if (affinity == -1) { puts("Failed to find nodes"); return; }
		printf("Affinity Between" MAGENTA_TXT " %s " WHITE_TXT "&"  MAGENTA_TXT " %s: " WHITE_TXT "%.2lf%%\n",
			zip[2], zip[3], 100 * affinity);
	}
	break;
	case DENSITY:
	{
		// 밀도
		double dense = get_density(graph);
		printf("Density of %s: %lf\n", graph->title, dense);
	}
	break;
	case INCLUSIVENESS:
	{
		// 포괄성
		int inc = get_inclusiveness(graph);
		printf("inclusiveness of %s: (%d/%d)\n", graph->title, inc, graph->nodeCnt);
	}
	break;
	case CENTRALITY:
	{
		double cent = get_centrality(graph, zip[2]);
		if (cent == -1) { puts(RED_TXT "Failed to find Node" WHITE_TXT); return; }
		printf("Centrality of " MAGENTA_TXT "%s" WHITE_TXT": " BLUE_TXT "%.2lf%%\n" WHITE_TXT, zip[2], 100 * cent);
	}
	break;
	default:
		showHelpMessage(GET);
		return;
	}
}

static void command_Rename(const char* zip[], int cnt)
{
	if (cnt < 1) { showHelpMessage(RENAME); return; }

	lower(zip[1]);
	if (!strcmp(zip[1], "graph") && cnt > 1) { renameObject(graph, NULL, zip[2], 0); }
	else if (!strcmp(zip[1], "node") && cnt > 2) { renameObject(graph, zip[2], zip[3], 1); }
	else if (!strcmp(zip[1], "edge") && cnt > 2) { renameObject(graph, zip[2], zip[3], 2); }
	else { showHelpMessage(RENAME); }
}

char execute(char command[100]) {
	const char* commandZip[20];
	char top = -1;
	const char* token = strtok(command, " ,\n\"\'");

	while (token != NULL) {
		commandZip[++top] = token;
		//printf("token [%d] == %s\n", top, token);
		token = strtok(NULL, " ,\n\"\'");
	}

	if (top == -1) { return 1; }
	switch (getCommandKind(commandZip[0])) {
	case CREATE:
		command_Create(commandZip, top);
		break;
	case DELETE:
		command_Delete(commandZip, top);
		break;
	case SEARCH:
		command_Search(commandZip, top);
		break;
	case CONNECT:
		command_Connect(commandZip, top);
		break;
	case DISCONNECT:
		command_Disconnect(commandZip, top);
		break;
	case RENAME:
		command_Rename(commandZip, top);
		break;
	case GET:
		command_Get(commandZip, top);
		break;
	case SAVE:
		command_Save(commandZip, top);
		break;
	case LOAD:
		command_Load(commandZip, top);
		break;
	case CLEAN:
		command_Clean(commandZip, top);
		break;
	case DISPLAY:
		showHyperGraph(graph);
		break;
	case EXIT:
	{
		if (query("Do you really want to exit? ")) {
			if (query("Do you want to save? ")) { command_Save(NULL, 0); }
			freeHyperGraph(graph); return 0;
		}
	}
	break;
	case HELP:
	default:
		if (top < 1) { showHelpMessage(HELP); }
		else { showHelpMessage(getCommandKind(commandZip[1])); }
	}
	return 1;
}