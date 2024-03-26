#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdlib.h>
#include <string.h>

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
struct Node* head = NULL;


struct req_buf { 
	long mesg_id; 
	enum optype operation;
	struct employee data; 
} request;

void add_node(struct Node *node){
        if(head==NULL){
                head=node;
                return;
        }
        struct Node *current=head;
        while(current->next!=NULL){
                current=current->next;
        }
        current->next=node;
} 

void create_node(struct req_buf *request){
        struct Node *node=(struct Node *)malloc(sizeof(struct Node));

        strcpy(node->data.firstname,request->data.firstname);
        strcpy(node->data.lastname,request->data.lastname);
        strcpy(node->data.contact,request->data.contact);
        strcpy(node->data.project,request->data.project);
        node->data.emp_id=request->data.emp_id;
        node->data.exp=request->data.exp;
        node->next=NULL;
        add_node(node);


}

void traverse(){
	struct Node *current=head;
	if(head==NULL){
		printf("no elements\n");
		return;
	}
	while(current!=NULL){
		printf("Node : %s\n",current->data.firstname);
		current=current->next;
	}
	
}
void delete_node(struct req_buf *request){
	if(head==NULL){
		printf("list is empty\n");
		return;
	}
	struct Node *current=head;
	struct Node *prev=NULL;
	while(current!=NULL){
		if(current->data.emp_id==request->data.emp_id){
			if(prev==NULL){
				head=current->next;
			}
			else{
				prev->next=current->next;
			}
			free(current);
		}
		prev=current;
		current=current->next;
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
			//addNode(request.data);
			create_node(&request);
			traverse();
		}else if(request.operation == delete){
			
		}
	}
	}
	// to destroy the message queue 
	msgctl(msgid, IPC_RMID, NULL); 
	traverse();
	return 0; 
} 
