#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#define KEY  999

struct employee{
	char firstname[10];
	char lastname[10];
	int emp_id;
	char contact[10];
	float exp;
	char project[50];	
};
struct Node{
	struct employee data;
	struct Node* next;
};
enum optype  {add =1, update,delete};
struct Node* Head = NULL;


struct req_buffer { 
	long mesg_id; 
	enum optype operation;
	struct employee data; 
} request; 
void addNode(struct employee data){
	struct Node node;
	node.data = data;
	node.next = NULL;
	if(Head == NULL){
		Head =  &node ;
		printf("at %p\n",Head);
	}
	else{
		struct Node* t = Head;
		while(t->next != NULL){
			t = t->next;
		}
		t->next = &node;
		printf("at %p\n",&node);
	}
}

void traverse(){
	if(Head == NULL){
		printf("EMPTY LIST");
	}
	else{
		struct Node* t = Head;
		while(t != NULL){
			printf("Node : %s\n",t->data.firstname);
			t = t->next;
		}
		
	}
}
int main() 
{ 
	key_t key = KEY; 
	int msgid;  
	msgid = msgget(key, 0666 | IPC_CREAT); 
	while(1){
	if(msgrcv(msgid, &request, sizeof(request), 1, 0)>0)
	{ 
		printf("Data Received is : %s \n",request.data.firstname); 
		printf("Data Received is : %d \n",request.operation); 
		if(request.operation == add){
		printf("adding node ");
		addNode(request.data);
		traverse();
		
	}
	}
	}
	// to destroy the message queue 
	msgctl(msgid, IPC_RMID, NULL); 
	traverse();
	return 0; 
} 
