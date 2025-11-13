#include <iostream>
#include <cstring>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Создание UDP сокета
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Подготовка адресной структуры сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1300); // Порт daytime службы
    server_addr.sin_addr.s_addr = INADDR_ANY; // Принимать соединения на все интерфейсы

    // Привязка сокета к адресу
    int bind_result = bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Daytime сервер запущен на порту 1300 (UDP)" << std::endl;
    std::cout << "Ожидание запросов..." << std::endl;

    // Основной цикл обработки запросов
    while (true) {
        char buffer[1024];
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Прием запроса от клиента
        ssize_t recv_result = recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr*)&client_addr, &client_addr_len);
        
        if (recv_result == -1) {
            std::cerr << "Ошибка приема запроса" << std::endl;
            continue;
        }

        // Получение текущего времени
        std::time_t current_time = std::time(nullptr);
        std::string time_string = std::ctime(&current_time);

        // Вывод информации о клиенте
        std::cout << "Запрос от " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Отправка времени клиенту
        ssize_t send_result = sendto(server_socket, time_string.c_str(), time_string.length(), 0,
                                    (const struct sockaddr*)&client_addr, client_addr_len);
        
        if (send_result == -1) {
            std::cerr << "Ошибка отправки ответа" << std::endl;
        } else {
            std::cout << "Отправлено время: " << time_string;
        }
    }

    // Закрытие сокета (недостижимый код в бесконечном цикле)
    close(server_socket);
    return 0;
}