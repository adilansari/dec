#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<netinet/in.h>
#include	<inttypes.h>

//suppose i am getting strings over here
struct Node {
	char data;
	struct List *listhead;
};

struct List {
	struct Node node;
	struct List *listnext;
};

//struct create Node
struct Node *createNode(struct Node *node, char nodeData) {
	node->data =nodeData;
	node->listhead = createList(NULL);
	return node;
}

struct List *createList(struct Node *node) {
	struct List list;
	list->node= node;
	list->listnext= NULL;
	return list;
}
//
struct Node *getfreeNode(struct List *list) {
	while(list->node != NULL)
		return getfreeNode(list->listnext);
	return list->node;
}

//insertion of  a node is in to the attached linkedlist
struct Node *insertNode(struct Node *parent, char nodeData) {
	if(parent== NULL)
		return createNode(parent, nodeData);
	else
		return insertNode(getfreeNode(parent->listhead), nodeData);

}

// this one is to find a particular node in the current STARTING NODE to assist in finding happened before relationship
int Nodelookup(struct Node *StartingNode, char nodeData) {
	if(StartingNode != NULL) {
		if(StartingNode->data == nodeData)
			return 1;
		else
			//pass the listhead node
	}
	else
		return 0;
}

// maintain a linkedlist struct Node startingNodes;
