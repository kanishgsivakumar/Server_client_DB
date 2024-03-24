 
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

enum optype  {add, update,delete};

struct req_buffer { 
	long mesg_id; 
	enum optype operation;
	struct employee data; 
} request; 

int main() 
{ 
	key_t key = KEY; 
	int msgid; 
	msgid = msgget(key, 0666 | IPC_CREAT); 
	request.mesg_id= 1; 

	printf("Write Data : "); 
	fgets(request.data.firstname,10,stdin); 

	msgsnd(msgid, &request, sizeof(request), 0); 
 
	printf("Data send is : %s \n", request.data.firstname); 

	return 0; 
} 

