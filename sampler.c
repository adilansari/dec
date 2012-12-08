#include "dec_algo.c"

//functions
void rqstList(char *rqst);
void tokenize(char *str);
int tokenizer(char *token);
void createrList(char a,char b);
void deallocate();

struct rqst *rList = NULL;

struct Node *copy;

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

char *str= "insert A->B B->C C->D; insert E->F; query A E;";
char *ins= "insert";
char *q= "query";



//this function separates each command using semicolon as a delimiter
void tokenize(char *str){
	char s[512];
	strcpy(s, str);
	int output;
	char *token = strtok(s,";");
	while (token) {
		output= tokenizer(token);
		token= strtok(NULL,";");
	}
}

//to separate insert and query commands
int tokenizer(char *token) {
	if (strstr(token,ins)) {
		rqstList(token); //create a linkedlist of all the insert requests
	}
	else if(strstr(token,q)) {
		//do handle this query over here
		printf("%s \n", token);
	}
	else {
		printf("invalid command");
	}
	return 0;
}

void rqstList(char *request) {
	int i=0;
	for(i=0; i<strlen(request); i++) {
		if(request[i] == '>') {
			//printf("\n%c %c",request[i-2], request[i+1]);
			createrList(request[i-2], request[i+1]);
		}
	}
	deallocate();
	//initialCheck(rList); //check if all the requests are valid; i.e. query(a,b) != (0 or 2)
}

void deallocate() {
	struct rqst *phead= rList;
	while(phead != NULL) {
	 struct rqst *temp = phead;
	 phead = phead->next;
	 free(temp);
 	 }
	rList=NULL;
}

int main() {
	 other();
	 tokenize(str);
	 //free(rList);
	 return 0;
}
