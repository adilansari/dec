#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include "dec_algo.c"

//functions
char * rqstList(char *rqst);
void tokenize(char *str);
char * tokenizer(char *token);
void createrList(char a,char b);
void deallocaterList();
int initialCheck();
int finalcheck();
char * queryCatcherPro(char *s);
void insertCopy(char a, char b);
struct Node *createNodeCopy(struct Node *node, char nodeData);
struct Node *CopyRoot(struct Node *orig, struct Node *duplicate);
int queryCopy(char a, char b);
int qhcCopy(char a, char b);
void queryHelperChildCopy(struct Start *traverse, char a, char b);

struct rqst *rList = NULL;

struct Node *copy;

//char *str= "insert A->B B->C C->D; insert E->F; query A E;";
char *ins= "insert";
char *q= "query";
char *res= "reset";
int CopyHelperChildFlag=0;
int CopyTraversedFlag=0;


struct rqst {
	char a;
	char b;
	struct rqst *next;
};



void createrList(char ch1, char ch2) {
	struct rqst *temp1= rList;
	if (rList == NULL) {
		rList= malloc(sizeof(struct rqst));
		rList->a= ch1;
		rList->b= ch2;
		rList->next= NULL;
	}
	else {
		while(temp1->next != NULL)
			temp1= temp1->next;
		temp1->next= malloc(sizeof(struct rqst));
		temp1= temp1->next;
		temp1->a= ch1;
		temp1->b= ch2;
		temp1->next= NULL;
	}
}


//this function separates each command using semicolon as a delimiter
void tokenize(char *str){
	char s[512];
	strcpy(s, str);
	char * output;
	char *token = strtok(s,";");
	printf("\nIncoming String = %s", str);
	//char *newstr= "response from server: ";
	while (token) {
		char *temp = token;
		output= tokenizer(temp);
		token= strtok(NULL,";");
	}
	//return newstr;
//	printf("%s",output);
}

//to separate insert and query commands
char * tokenizer(char *token) {
	if(token == '\0' || token == '\n') {
			return "";
		}
	printf("\n Request = %s \n", token);

	if (strstr(token,ins)) {
		return rqstList(token); //create a linkedlist of all the insert requests
		printGraph(root);
	}
	else if(strstr(token,q)) {
		//do handle this query over here
		return queryCatcherPro(token);
	}
	else if(strstr(token,res)) {
		reset();
		printf("\n RESET DONE");
		return "RESET DONE";
	}
	else {
		printf("\n INVALID COMMAND");
		return "INVALID COMMAND";
	}
	return "";
}

char * queryCatcherPro(char *s) {
	int i=0;
	for (i=0; i<strlen(s);i++) {
		if((s[i]>64) && (s[i]<91) && (s[i+1]== 32)) {
			return queryCatcher(s[i], s[i+2]);
		}
	}
	return "\0";
}

char * rqstList(char *request) {
	char * ret;
	int i=0;
	for(i=0; i<=strlen(request); i++) {
		if(request[i] == '>') {
			createrList(request[i-2], request[i+1]);
		}
	}
	if (initialCheck() && finalcheck()) {
		while(rList != NULL) {
			insert(rList->a, rList->b);//push requests into the main data structure
		rList= rList->next;
		printf("\n INSERTION DONE");
		ret= "\n INSERTION DONE";//printf("\n insertion done");
		}
	}
	else {
		ret= "\n INSERTION FAILED";
		printf("\n insertion failed");
		//return 2;
	}
	deallocateNode(copy);
	copy= NULL;
	deallocaterList();
	return ret;
}

int initialCheck() {
	struct rqst *tnode= rList;

	while(tnode != NULL) {
		if(/*(query(tnode->a, tnode->b) != 0) &&*/ (query(tnode->a,tnode->b) !=2))
			tnode= tnode->next;
		else
			return 0;
	}
	return 1;
}

int finalcheck(){
	struct Node *temp3 = root;
	while(temp3 != NULL) {
	struct Node *fakeNode= CopyRoot(temp3, copy);//get all elements into *copy from *root
	temp3= temp3->next;
	}
	struct rqst *tempNode= rList;
	while (tempNode != NULL) {
		if((queryCopy(tempNode->a, tempNode->b) != 2))//rotate through linkedlist rqst and check via queryCopy than do insertCopy
			insertCopy(tempNode->a, tempNode->b);//insert a request step by step and check on each step that the next request is valid
		else
			return 0;
		tempNode = tempNode->next;
	}
	return 1;
}

int queryCopy(char a, char b) {
	if(!hasNode(copy, a))
		return 3;
	if(!hasNode(copy,b))
		return 4;
	else if (qhcCopy(a,b))
		return 1;
	else if (qhcCopy(b,a))
		return 2;
	else
		return 0;
}

int qhcCopy(char a, char b) {
	CopyHelperChildFlag=0;
	CopyTraversedFlag=0;
	struct Start *traverse= createFirst(traverse,a);//create traversednodes
	queryHelperChildCopy(traverse,a,b);
	deallocateFirst(traverse);
	traverse= NULL;
	return CopyHelperChildFlag;
}

//search if a has a child as b down the graph somewhere
void queryHelperChildCopy(struct Start *traverse, char a, char b) {
	if(isTraversed(traverse,a) && isTraversedFlag==1)
		return;
	else {
		appendFirst(traverse,a);
		CopyTraversedFlag=1;
	}
	//do the following when this node is not already traversed once
	int i=0;
	struct Node *aNode = getNode(copy,a);
	while(aNode->child[i] != 0) {
		if(aNode->child[i] == b) {
			CopyHelperChildFlag= 1;
			return;
		}
		else
			queryHelperChildCopy(traverse,(aNode->child[i++]),b);
	}
}

void insertCopy(char a, char b){
	struct Node *pNode, *cNode;
			if (hasNode(copy,a))
				pNode= getNode(copy,a);
			else {
				pNode= createNodeCopy(copy,a); //change here
			}
			if (hasNode(copy, b))
				cNode= getNode(copy,b);
			else
				cNode= createNodeCopy(copy, b); //change here
		//printf("%c %c", pNode->data, cNode->data);
		pNode->child[(pNode->cindex)] = b;
		pNode->cindex++;
		cNode->parent[(cNode->pindex)]= a;
		cNode->pindex++;
}

struct Node *createNodeCopy(struct Node *node, char nodeData) {
	if(copy == NULL) {
			int i=0;
			node= malloc(sizeof(struct Node));
			node->data = nodeData;
			while(i<50) {
				node->parent[i]= '\0';
				node->child[i]= '\0';
				i++;
			}
			node->pindex=0;
			node->cindex=0;
			node->next= NULL;//malloc(sizeof(struct Node));
			copy=node;
			return node;
		}
		else if (node->next == NULL) {
			int j=0;
			node->next= malloc(sizeof(struct Node));
			node= node->next;
			node->data = nodeData;
			while (j<50) {
				node->parent[j]= '\0';
				node->child[j]= '\0';
				j++;
			}
			node->pindex=0;
			node->cindex=0;
			node->next= NULL;
			return node;
		}
		else
			return createNodeCopy(node->next, nodeData);
}

struct Node *CopyRoot(struct Node *orig, struct Node *duplicate) {
	if(duplicate == NULL) {
		int i =0;
		duplicate = malloc(sizeof(struct Node));
		duplicate->data= orig->data;
		while(i<50) {
			duplicate->child[i]= orig->child[i];
			duplicate->parent[i]= orig->child[i];
			i++;
		}
			duplicate->pindex = orig->pindex;
			duplicate->cindex= orig->cindex;
			duplicate->next= NULL;
			copy= duplicate;
			return duplicate;
	}
	else if (duplicate->next== NULL){
		int j=0;
		duplicate->next= malloc(sizeof(struct Node));
		duplicate= duplicate->next;
		duplicate->data= orig->data;
		while(j<50) {
			duplicate->child[j]= orig->child[j];
			duplicate->parent[j]= orig->child[j];
			j++;
		}
		duplicate->pindex = orig->pindex;
		duplicate->cindex= orig->cindex;
		duplicate->next= NULL;
		return duplicate;
	}
	else
		return CopyRoot(orig, duplicate->next);
}

void deallocaterList() {
	struct rqst *phead= rList;
	while(phead != NULL) {
	 struct rqst *temp = phead;
	 phead = phead->next;
	 free(temp);
 	 }
	rList=NULL;
}

/*int main() {
	 //other();
	int k=0;
	char str[256];
	for(k=0;k<20;k++) {
	printf("\nInput your command:");
	gets(str);
	tokenize(str);
	}
	return 0;
}
*/
