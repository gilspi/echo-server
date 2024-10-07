#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MAX 80
#define PORT 8003
#define SA struct sockaddr


void func(int sockfd){
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, sizeof(buff));  // Очищаем буфер перед вводом
        printf("enter the string: ");
        n = 0;
        // Чтение строки от пользователя
        while ((buff[n++] = getchar()) != '\n');
        buff[n] = '\0';  // Завершаем строку нуль-терминатором

        // Отправляем только фактические данные
        write(sockfd, buff, strlen(buff));

        // Очищаем буфер для чтения ответа от сервера
        bzero(buff, sizeof(buff));

        // Чтение ответа от сервера
        int bytes_read = read(sockfd, buff, sizeof(buff) - 1);
        if (bytes_read > 0) {
            buff[bytes_read] = '\0';  // Завершаем строку нуль-терминатором
            printf("From server: %s\n", buff);
        }

        // Проверка на команду выхода
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client exit...\n");
            break;
        }
    }
}

int main(){
    int sockfd;
    struct sockaddr_in servaddr;

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("socket creation successfully...\n");
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Подключение к серверу
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1) {
        printf("connection failed...\n");
        exit(0);
    } else {
        printf("connection successfully created....\n");
    }

    // Запуск функции для общения с сервером
    func(sockfd);

    // Закрытие сокета
    close(sockfd);
}
