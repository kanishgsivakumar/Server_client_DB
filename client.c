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
enum optype  {add = 1, update,delete};
struct req_buffer { 
	long mesg_id; 
	enum optype operation;
	struct employee data; 
} request; 
void getData(struct employee* const emp){
     printf("Enter Employee firstname : "); 
     scanf("%s",emp->firstname);
     printf("Enter Employee lastname : ");
     scanf("%s",emp->lastname);
     printf("Enter Employee ID : ");
     scanf("%d",&emp->emp_id);
     gets();
     printf("Enter Employee contact : ");
     fgets(emp->contact,10,stdin);
}
int main() 
{ 
	key_t key = KEY; 
	int msgid; 
	msgid = msgget(key, 0666 | IPC_CREAT); 
	request.mesg_id= 1; 
	printf("Select the operation\n");
	printf("1.Add\n");
	printf("2.update\n");
	printf("3.delete\n");
	int operation;
	scanf("%d",&operation);
	request.operation = operation ;
	getData(&request.data); 

	msgsnd(msgid, &request, sizeof(request), 0); 

	printf("Data send is : %s \n", request.data.firstname); 

	return 0; 
} 
