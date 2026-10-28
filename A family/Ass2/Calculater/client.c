#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    int a, b;

    char op;

    printf("Enter first number: ");
    scanf("%d", &a);

    printf("Enter operator (+ - * /): ");
    scanf(" %c", &op);

    printf("Enter second number: ");
    scanf("%d", &b);

    write(sock, &a, sizeof(a));
    write(sock, &b, sizeof(b));
    write(sock, &op, sizeof(op));

    int result;

    read(sock, &result, sizeof(result));

    printf("Result = %d\n", result);

    close(sock);

    return 0;
}
