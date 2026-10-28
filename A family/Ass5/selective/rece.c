#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int sock;

    struct sockaddr_in receiver,transmitter;

    int packet;
    int total_packets,window_size;
    int tt,tp,loss;

    int recv_base = 1;
    int global_counter = 0;

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = INADDR_ANY;

    sock = socket(AF_INET,SOCK_DGRAM,0);

    if(sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    bind(sock,
         (struct sockaddr *)&receiver,
         sizeof(receiver));

    socklen_t len = sizeof(transmitter);

    recvfrom(sock,&total_packets,
             sizeof(total_packets),0,
             (struct sockaddr *)&transmitter,
             &len);

    recvfrom(sock,&window_size,
             sizeof(window_size),0,
             (struct sockaddr *)&transmitter,
             &len);

    recvfrom(sock,&tt,sizeof(tt),0,
             (struct sockaddr *)&transmitter,
             &len);

    recvfrom(sock,&tp,sizeof(tp),0,
             (struct sockaddr *)&transmitter,
             &len);

    recvfrom(sock,&loss,sizeof(loss),0,
             (struct sockaddr *)&transmitter,
             &len);

    printf("\n============================================\n");
    printf("             GO-BACK-N RECEIVER\n");
    printf("============================================\n");

    printf("\nReceiver configuration\n");
    printf("--------------------------------------------\n");

    printf("Expected Packets : %d\n",total_packets);
    printf("Window Size      : %d\n",window_size);
    printf("Tx Time          : %d\n",tt);
    printf("Propagation Time : %d\n",tp);
    printf("Loss Interval    : %d\n",loss);

    printf("--------------------------------------------\n");
    printf("Receiver is ready. Waiting for packets...\n");

    int *buffered =
        calloc(total_packets+2,sizeof(int));

    while(recv_base <= total_packets)
    {
        recvfrom(sock,&packet,sizeof(packet),0,
                 (struct sockaddr *)&transmitter,
                 &len);

        if(packet < recv_base)
        {
            sendto(sock,&packet,sizeof(packet),0,
                   (struct sockaddr *)&transmitter,
                   len);

            printf("\nPacket %d --> OLD PACKET\n",
                   packet);

            printf("ACK %d --> SENT AGAIN\n",packet);

            continue;
        }

        if(packet >= recv_base + window_size)
        {
            printf("\nPacket %d --> DROPPED\n",
                   packet);

            printf("Reason : Outside receiver window\n");

            continue;
        }

        global_counter++;

        if(global_counter % loss == 0)
        {
            printf("\n============================================\n");
            printf("              LOSS SIMULATION\n");
            printf("============================================\n");

            printf("Packet %d --> LOST\n",packet);

            printf("Receiver will wait for retransmission.\n");

            continue;
        }

        if(!buffered[packet])
        {
            buffered[packet] = 1;

            printf("\nPacket %d --> RECEIVED\n",
                   packet);
        }

        sendto(sock,&packet,sizeof(packet),0,
               (struct sockaddr *)&transmitter,
               len);

        printf("ACK %d --> SENT\n",packet);

        while(recv_base <= total_packets &&
              buffered[recv_base])
        {
            printf("Delivered to application : Packet %d\n",
                   recv_base);

            recv_base++;
        }
    }

    printf("\n============================================\n");
    printf("             RECEIVING COMPLETED\n");
    printf("============================================\n");

    printf("All %d packets received successfully.\n",
           total_packets);

    printf("Final Status : SUCCESS\n");

    printf("============================================\n");

    free(buffered);

    close(sock);

    return 0;
}
