#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in receiver;

    char chip[] = "10110111000";
    char data[100];
    char spread[256];

    printf("Enter binary data: ");
    scanf("%s", data);

    int k = 0;

    for(int i = 0; i < strlen(data); i++)
    {
        for(int j = 0; j < strlen(chip); j++)
        {
            int d = data[i] - '0';
            int c = chip[j] - '0';

            spread[k++] = (d ^ c) + '0';
        }
    }

    spread[k] = '\0';

    printf("Original Data : %s\n", data);
    printf("Spread Data   : %s\n", spread);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(sock,
           spread,
           strlen(spread) + 1,
           0,
           (struct sockaddr *)&receiver,
           sizeof(receiver));

    close(sock);

    return 0;
}
