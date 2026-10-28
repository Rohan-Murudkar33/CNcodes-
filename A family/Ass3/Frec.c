#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
    int freq[]={9000,9001,9002,9003,9004};
    int hop_seq[]={1,3,2,4,0};
    char message[256];
    int rec_socket;
    struct sockaddr_in transmitterAddress,receiverAddress;

    int i=0;
    receiverAddress.sin_family=AF_INET;
    while(i<5){
        rec_socket=socket(AF_INET,SOCK_DGRAM,0);
        receiverAddress.sin_port=htons(freq[hop_seq[i]]);
        receiverAddress.sin_addr.s_addr=INADDR_ANY;
        bind(rec_socket,
         (struct sockaddr *)&receiverAddress,
         sizeof(receiverAddress));
         socklen_t len = sizeof(transmitterAddress);
        recvfrom(rec_socket,
             message,
             sizeof(message),
             0,
             (struct sockaddr *)&transmitterAddress,
             &len);
        printf("Message from transmitter on Frequency %d: %s\n", freq[hop_seq[i]],message);  
        i++;  
        close(rec_socket);
    }


    return 0;
}
