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
    char spread[256];
    char chip[] = "10110111000";
    char original[100];

    struct sockaddr_in receiver, sender;
    socklen_t len = sizeof(sender);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = INADDR_ANY;

    bind(sock,
         (struct sockaddr *)&receiver,
         sizeof(receiver));

    recvfrom(sock,
             spread,
             sizeof(spread),
             0,
             (struct sockaddr *)&sender,
             &len);

    printf("Received Spread Data : %s\n", spread);

    int chip_len = strlen(chip);
    int k = 0;

    for(int i = 0; i < strlen(spread); i += chip_len)
    {
        int spread_bit = spread[i] - '0';
        int chip_bit = chip[0] - '0';

        original[k++] = (spread_bit ^ chip_bit) + '0';
    }

    original[k] = '\0';

    printf("Original Data : %s\n", original);

    close(sock);

    return 0;
}
