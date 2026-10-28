#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<math.h>

int min(int x,int y)
{
    return (x<y ? x : y);
}

int total_packets, window_size, base;
int *sent, *acked;

void printWindow()
{
    printf("\nActive Window : { ");

    for(int i=base;
        i<base+window_size && i<=total_packets;
        i++)
    {
        if(sent[i] && !acked[i])
            printf("P%d ",i);
    }

    printf("}\n");
    printf("Current Base : %d\n\n",base);
}

int main()
{
    int sock;

    struct sockaddr_in receiver;

    socklen_t len = sizeof(receiver);

    int tp,tt,loss;
    int total_time=0;
    int total_transmissions=0;

    printf("\n============================================\n");
    printf("        GO-BACK-N TRANSMITTER\n");
    printf("============================================\n");

    printf("\nEnter Transmission Time : ");
    scanf("%d",&tt);

    printf("Enter Propagation Time  : ");
    scanf("%d",&tp);

    printf("Enter Loss Interval     : ");
    scanf("%d",&loss);

    if(loss < 2)
        loss = 2;

    int a = tp / tt;

    total_packets = 1 + 2 * a;

    int bits = (int)floor(log2(total_packets));

    window_size =
        min((int)pow(2,bits),total_packets);

    printf("\n--------------------------------------------\n");
    printf("Calculated Parameters\n");
    printf("--------------------------------------------\n");

    printf("Propagation Ratio : %d\n",a);
    printf("Total Packets     : %d\n",total_packets);
    printf("Sequence Bits     : %d\n",bits);
    printf("Window Size       : %d\n",window_size);

    printf("--------------------------------------------\n");

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr =
        inet_addr("127.0.0.1");

    sock = socket(AF_INET,SOCK_DGRAM,0);

    if(sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    sendto(sock,&total_packets,sizeof(total_packets),0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    sendto(sock,&window_size,sizeof(window_size),0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    sendto(sock,&tt,sizeof(tt),0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    sendto(sock,&tp,sizeof(tp),0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    sendto(sock,&loss,sizeof(loss),0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    struct timeval timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,
               &timeout,sizeof(timeout));

    sent =
        calloc(total_packets+2,sizeof(int));

    acked =
        calloc(total_packets+2,sizeof(int));

    int *deadline =
        calloc(total_packets+2,sizeof(int));

    base = 1;

    int next_seq = 1;

    while(base <= total_packets)
    {
        while(next_seq <= total_packets &&
              next_seq < base + window_size)
        {
            sleep(tt);

            total_time += tt;

            sendto(sock,&next_seq,
                   sizeof(next_seq),0,
                   (struct sockaddr *)&receiver,
                   sizeof(receiver));

            sent[next_seq] = 1;

            deadline[next_seq] =
                total_time + 2*tp;

            total_transmissions++;

            printf("[TIME %d]  Packet %d --> SENT\n",
                   total_time,next_seq);

            next_seq++;
        }

        printWindow();

        int ack;

        int result =
            recvfrom(sock,&ack,sizeof(ack),0,
                     (struct sockaddr *)&receiver,
                     &len);

        if(result > 0)
        {
            total_time += tp;

            if(ack >= base &&
               ack < base + window_size &&
               sent[ack])
            {
                acked[ack] = 1;
            }

            printf("--------------------------------------------\n");
            printf("ACK EVENT\n");
            printf("--------------------------------------------\n");

            printf("ACK Received : %d\n",ack);
            printf("Time         : %d\n",total_time);

            while(base <= total_packets &&
                  acked[base])
            {
                base++;
            }

            printf("New Base     : %d\n",base);

            printWindow();
        }
        else
        {
            total_time += 1;

            printf("\n============================================\n");
            printf("             TIMEOUT EVENT\n");
            printf("============================================\n");

            for(int s=base;
                s<next_seq;
                s++)
            {
                if(sent[s] &&
                   !acked[s] &&
                   deadline[s] <= total_time)
                {
                    sleep(tt);

                    total_time += tt;

                    sendto(sock,&s,sizeof(int),0,
                           (struct sockaddr *)&receiver,
                           sizeof(receiver));

                    deadline[s] =
                        total_time + 2*tp;

                    total_transmissions++;

                    printf("Packet %d --> RETRANSMITTED\n",s);
                }
            }

            printf("============================================\n");
        }
    }

    printf("\n============================================\n");
    printf("        TRANSMISSION COMPLETED\n");
    printf("============================================\n");

    printf("All packets delivered successfully.\n");
    printf("Total Time          : %d\n",total_time);
    printf("Total Transmissions : %d\n",
           total_transmissions);
    printf("Final Status        : SUCCESS\n");

    printf("============================================\n");

    free(sent);
    free(acked);
    free(deadline);

    close(sock);

    return 0;
}
