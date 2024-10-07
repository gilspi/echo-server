#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#define MAX 80
#define PORT 8004
#define SA struct sockaddr


void func(int connfd) {
    char buff[MAX];

    for (;;) {
        bzero(buff, MAX);  // Очищаем буфер перед чтением

        // Чтение сообщения от клиента
        int bytes_read = read(connfd, buff, sizeof(buff) - 1);  // Чтение данных от клиента
        if (bytes_read > 0) {
            buff[bytes_read] = '\0';  // Завершаем строку нуль-терминатором
            printf("Received %d bytes: %s\n", bytes_read, buff);

            // Отправляем обратно клиенту только полученные байты
            write(connfd, buff, bytes_read);
        } else if (bytes_read == 0) {
            printf("Client disconnected\n");
            break;  // Клиент закрыл соединение
        } else {
            perror("read error");
            break;
        }

        // Проверка на команду выхода
        if (strncmp(buff, "exit", 4) == 0){
            printf("Server exit...\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("socket successfully created...\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Привязка IP и порта
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Привязка сокета к IP
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) == -1) {
        printf("socket bind failed...\n");
        exit(0);
    } else {
        printf("socket successfully binded...\n");
    }

    // Ожидание подключения клиента
    if ((listen(sockfd, 5)) == -1) {
        printf("listen failed...\n");
    } else {
        printf("server listening...\n");
    }

    len = sizeof(cli);

    // Принимаем клиента
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd == -1) {
        printf("server accept failed...\n");
    } else {
        printf("server accept the client...\n");
    }

    // Обработка сообщений клиента
    func(connfd);

    // Закрытие сокета
    close(connfd);
    close(sockfd);
}
