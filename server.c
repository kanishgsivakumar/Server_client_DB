#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdlib.h>
#include <string.h>
#include<pthread.h>

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
enum optype  {add = 1, delete,list};
struct Node* head = NULL;
pthread_mutex_t mutex;

struct req_buf { 
	long mesg_id; 
	enum optype operation;
	struct employee data; 
} request;

void WriteDB(){
        FILE *file=fopen("data.txt","w");
        struct Node *node=head;
        while(node!=NULL){
               fprintf(file,"%s,%s,%d,%s,%lf,%s\n",node->data.firstname,node->data.lastname,node->data.emp_id,node->data.contact,node->data.exp,node->data.project);
               node=node->next;
        }
        fclose(file);
}

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
        WriteDB();
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
void ReadDB(){
        FILE *file=fopen("data.txt","r");
        struct employee data;
	char line[100]; 

        while(fgets(line, sizeof(line), file) != NULL){
		if (sscanf(line,"%[^,],%[^,],%d,%[^,],%f,%[^\n]", data.firstname, data.lastname, &data.emp_id, data.contact,  &data.exp, data.project) == 6) {
			printf("Reading data from file: %s %s %d %s %lf %s\n", data.firstname, data.lastname, data.emp_id, data.contact, data.exp, data.project);

                         struct Node *node=(struct Node*)malloc(sizeof(struct Node));
                         if(node==NULL){
				 printf("memory allocation error\n");
                                 fclose(file);
                                 return;
                         }
                         strcpy(node->data.firstname,data.firstname);
                         strcpy(node->data.lastname,data.lastname);
                         strcpy(node->data.contact,data.contact);
                         strcpy(node->data.project,data.project);
                         node->data.emp_id=data.emp_id;
                         node->data.exp=data.exp;
                         node->next=NULL;
                         struct req_buf read_req;
                         read_req.data = data;
                         create_node(&read_req);
           } 
	         else {
                         printf("Error parsing line: %s\n", line);
                         printf("read data from file: %s %s %d %s %f %s\n", data.firstname, data.lastname, data.emp_id, data.contact, data.exp, data.project);
            }
    }

    fclose(file);
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
	ReadDB();
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
	WriteDB();
}
void *client_handler(void *arg) {
	struct req_buf *req = (struct req_buf *)arg;
	printf("Data Received is : %s \n",req->data.firstname); 
	printf("Data operation is : %d \n",req->operation); 
	switch(request.operation)
	{
	case add :
			printf("adding node\n");
			pthread_mutex_lock(&mutex);
			create_node(req);
			pthread_mutex_unlock(&mutex);
			traverse();
			break;
	case delete : 
			printf("deleting node\n");
			pthread_mutex_lock(&mutex);
			delete_node(req);
			pthread_mutex_unlock(&mutex);
			traverse();
			break;
	}
	if(request.operation == add){
			printf("adding node\n");
			pthread_mutex_lock(&mutex);
			create_node(req);
			pthread_mutex_unlock(&mutex);
			traverse();
		}

    pthread_exit(NULL);
}
int main() 
{ 
	ReadDB();
	key_t key = KEY; 
	int msgid;  
	msgid = msgget(key, 0666 | IPC_CREAT); 
	while(1){
	if(msgrcv(msgid, &request, sizeof(request), 1, 0)>0)
	{ 
		pthread_t client_thread;
        	if(pthread_create(&client_thread,NULL,client_handler,(void *)&request)!=0) {
        		printf("error in pthread_create");
            		exit(EXIT_FAILURE);
        	}
	}
	}
	// to destroy the message queue 
	msgctl(msgid, IPC_RMID, NULL); 
	traverse();
	return 0; 
} 
