#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>


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
//struct Start first; //will contain the unique starting characters for the graph.. NO NEED FOR THIS
int queryHelperChildFlag=0;

struct Start *createFirst(struct Start *first, char data) {
	first= malloc(sizeof(struct Start));
	first->data= data;
	first->next=malloc(sizeof(struct Start));
	return first;
}
struct Start *appendFirst(struct Start *first, char data) {
	if(first->data ==0) {
		first->data=data;
		first->next=malloc(sizeof(struct start));
		return first;
	}
	else
		return appendFirst(first->next, data);
}
int isTraversed(struct Start *first, char data) {
	while(first != NULL) {
		if (first->data == data)
			return 1;
		else
			first=first->next;
	}
	return 0;
}
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
	if(root == NULL) {
		node= malloc(sizeof(struct Node));
		node->data = nodeData;
		node->pindex=0;
		node->cindex=0;
		node->next= malloc(sizeof(struct Node));
		root=node;
		return node;
	}
	else if (node->data == 0) {
		node->data = nodeData;
		node->pindex=0;
		node->cindex=0;
		node->next= malloc(sizeof(struct Node));
		return node;
	}
	else
		return createNode(node->next, nodeData);
}

void queryCatcher(char a, char b) {
	int result=query(a,b);
	switch(result) {
	case 0:
		printf("%c concurrent to %c", a,b);
		break;
	case 1:
		printf("%c happened before %c", a,b);
		break;
	case 2:
		printf("%c happened before %c", b,a);
		break;
	case 3:
		printf("Event not found: %c",a);
		break;
	case 4:
		printf("Event not found: %c",b);
		break;
	}
}

//lookup for a element in a particular subgraph to assist QUERY and create a void querycatcher
int query(char a, char b) {
	if(!hasNode(root, a))
		return 3;
	if(!hasNode(root,b))
		return 4;
	else if (qhc(a,b))
		return 1;
	else if (qhc(b,a))
		return 2;
	else
		return 0;
}

int qhc(char a, char b) {
	queryHelperChildFlag=0;
	struct Start traverse= createFirst(traverse,a);//create traversednodes
	queryHelperChild(a,b);
	return queryHelperChildFlag;
}

//search if a has a child as b down the graph somewhere
void queryHelperChild(struct Start *traverse,char a, char b) {
	if(isTraversed(traverse,a))
		return;
	else
		traverse= appendFirst(traverse,a);
	//do the following when this node is not already traversed once
	int i=0;
	struct Node aNode = getNode(root,a);
	while(aNode->child[i] != 0) {
		if(aNode->child[i] == b)
			queryHelperChildFlag= 1;
		else
			queryHelper(aNode.child[i++],b);
	}
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
	while(root->data != 0) {
		printf("\n Node data= %c", root->data);
		printf("\n Node pindex= %d , cindex= %d", root->pindex, root->cindex);
		printf("\n parent Nodes: ");
		while(root->parent[count] !=0) {
			printf("%c,", root->parent[count++]);
		}
		count=0;
		printf("\n Child Nodes: ");
		while(root->child[count] !=0) {
			printf("%c,", root->child[count++]);
			}
		count=0;
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
	 insert('A','C');
	 printGraph(root);
	 return 0;
 }
