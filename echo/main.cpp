#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Создание TCP сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Подготовка адресной структуры сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7000); // Порт echo службы
    server_addr.sin_addr.s_addr = INADDR_ANY; // Принимать соединения на все интерфейсы

    // Привязка сокета к адресу
    int bind_result = bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    // Перевод сокета в режим прослушивания
    int listen_result = listen(server_socket, 5); // Очередь из 5 соединений
    if (listen_result == -1) {
        std::cerr << "Ошибка перевода сокета в режим прослушивания" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Echo сервер запущен на порту 7000 (TCP)" << std::endl;
    std::cout << "Ожидание подключений..." << std::endl;

    // Основной цикл обработки подключений
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Принятие клиентского соединения
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            std::cerr << "Ошибка принятия соединения" << std::endl;
            continue;
        }

        // Вывод информации о клиенте
        std::cout << "Новое подключение от " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Прием сообщения от клиента
        char buffer[1024];
        ssize_t recv_result = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (recv_result == -1) {
            std::cerr << "Ошибка приема сообщения" << std::endl;
            close(client_socket);
            continue;
        }

        buffer[recv_result] = '\0'; // Добавление нулевого символа
        std::cout << "Получено сообщение: " << buffer << std::endl;

        // Отправка эха клиенту
        ssize_t send_result = send(client_socket, buffer, recv_result, 0);
        
        if (send_result == -1) {
            std::cerr << "Ошибка отправки эха" << std::endl;
        } else {
            std::cout << "Отправлено эхо: " << buffer << std::endl;
        }

        // Закрытие клиентского сокета
        close(client_socket);
        std::cout << "Соединение с клиентом закрыто" << std::endl;
    }

    // Закрытие серверного сокета (недостижимый код)
    close(server_socket);
    return 0;
}