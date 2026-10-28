#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
    char* msg[]={"HEL","LO","WOR","LD","!!"};
    int freq[]={9000,9001,9002,9003,9004};
    int hop_seq[]={1,3,2,4,0};
    struct sockaddr_in receiverAddress;

    int i=0;
    receiverAddress.sin_family=AF_INET;
    receiverAddress.sin_addr.s_addr=inet_addr("127.0.0.1");

    while(i<5){
        int tran_socket;
        tran_socket=socket(AF_INET,SOCK_DGRAM,0);
        receiverAddress.sin_port=htons(freq[hop_seq[i]]);
    
        sendto(tran_socket,msg[i],strlen(msg[i])+1,0,(struct sockaddr*)&receiverAddress,sizeof(receiverAddress));
        sleep(10);
        close(tran_socket);
        i++;
    }
    
    return 0;
}
