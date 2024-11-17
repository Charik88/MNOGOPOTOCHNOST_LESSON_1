//
//  main.cpp
//  MNOGOPOTOCHNOST
//
//  Created by Anastasiya Charykova on 16.11.2024.
/*адание 1
 Очередь клиентов

 Вам нужно создать приложение, которое имитирует очередь в окошко. Для этого нужно создать два потока, работающие с одной разделяемой переменной.

 Первый поток имитирует клиента: раз в секунду он обращается к счётчику клиентов и увеличивает его на 1. Максимальное количество клиентов должно быть параметризировано.

 Второй поток имитирует операциониста: раз в 2 секунды он обращается к счётчику клиентов и уменьшает его на 1. «Операционист» работает до последнего клиента.*/


#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Атомическая переменная для счётчика клиентов
std::atomic<int> clientCounter(0);
const int maxClients = 10; // Максимальное количество клиентов

// Функция-поток для клиента
void client() {
    for (int i = 0; i < maxClients; i++) {
        clientCounter++;
        std::cout << "Client: " << clientCounter << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Функция-поток для оператора
void operatorService() {
    while (clientCounter > 0) {
        clientCounter--;
        std::cout << "Operator: " << clientCounter << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    std::thread clientThread(client);
    std::thread operatorThread(operatorService);

    // Ждём окончания работы потоков
    clientThread.join();
    operatorThread.join();

    return 0;
}

