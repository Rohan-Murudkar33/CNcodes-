#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(server_fd, 1);

    printf("Server is waiting...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&client, &len);

    int a, b;
    char op;
    int result;

    read(client_fd, &a, sizeof(a));
    read(client_fd, &b, sizeof(b));
    read(client_fd, &op, sizeof(op));

    switch(op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if(b != 0)
                result = a / b;
            else
                result = 0;
            break;
        default:
            result = -1;
    }

    write(client_fd, &result, sizeof(result));

    close(client_fd);
    close(server_fd);

    return 0;
}
