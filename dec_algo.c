#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netdb.h>
#include	<netinet/in.h>
#include	<inttypes.h>


//functions declaration:-
void insert(char a, char b);
struct Node *createNode(struct Node *root, char nodeData);
int hasNode(struct Node *root, char data);
struct Node *getNode(struct Node *root, char data);
void printGraph(struct Node *root);

//suppose i am getting strings over here
struct Node {
	char data;
	char parent[50];
	char child[50];
	struct Node *next;
	int pindex;
	int cindex;
};

struct Start {
	char data;
	struct Start *next;
};

struct Node *root; // starting from first element this will have all the elements that have been added to the graph
int rootflag=0;
struct Start first; //will contain the unique starting characters for the graph.. NO NEED FOR THIS
struct Start traversed; //will contain a list of all the elements traversed while looking up for an element

void insert(char a, char b) {
	struct Node *pNode, *cNode;
		if (hasNode(root,a))
			pNode= getNode(root,a);
		else {
			pNode= createNode(root,a);
			//addToFirst(one); No need as we already have list of all elements
		}
		if (hasNode(root, b))
			cNode= getNode(root,b);
		else
			cNode= createNode(root, b);
		printf("%c %c", pNode->data, cNode->data);
	pNode->child[(pNode->cindex)++] = b;
	cNode->parent[(cNode->pindex)++]= a;
}

struct Node *createNode(struct Node *node, char nodeData) {
	printf("createNode");
	if(root==NULL) {
		node= malloc(sizeof(struct Node));
		node->data = nodeData;
		node->next= NULL;
		node->pindex=0;
		node->cindex=0;
		root=node;
		return node;
	}
	else if (node == NULL){
		node->next= malloc(sizeof(struct Node));
		node= node->next;
		node->data = nodeData;
		node->next= NULL;
		node->pindex=0;
		node->cindex=0;
		return node;
	}
	else {
		return createNode(node->next, nodeData);
	}
}

//lookup for a element in a particular subgraph to assist QUERY
int lookup(struct Start *first, char data) {
	char c= first->data;
	//get Node for first.data
	int i=0;
	if(c == data) {
		return 1;
	}
	else {
		for(i=0;i++;i<50) {

		}
	}
	return 1;
}

//look if graph has Node
int hasNode(struct Node *root, char data) {
	while(root != NULL) {
		if(root->data == data)
			return 1;
		else
			root=root->next;
	}
	return 0;
}
//return Node for that particular element to have access to its parent and child elements
struct Node *getNode(struct Node *root, char data) {
	while (root->data != data)
		root= root->next;
	return root;
}

//NO NEED .... if a new parent is created than add it to the list of all unique parents/starting nodes of the graphs

// print the whole graph
void printGraph(struct Node *root) {
	int count=0;
	while(root != NULL) {
		printf("\n Node data= %c", root->data);
		printf("\n Node pindex= %d , cindex= %d", root->pindex, root->cindex);
		printf("\n parent Nodes: ");
		for(count=0;count<50;count++) {
			printf("%c,", root->parent[count]);
		}
		printf("\n Child Nodes: ");
		for(count=0;count<50;count++) {
			printf("%c,", root->child[count]);
		}
		root= root->next;
	}
	printf("\n -------- END ---------");
}
//a function to check if a request is valid
 int main() {
	 insert('A','B');
	 printGraph(root);
	 insert('B','C');
	 printGraph(root);
	 insert('D','E');
	 printGraph(root);
	 return 0;
 }
