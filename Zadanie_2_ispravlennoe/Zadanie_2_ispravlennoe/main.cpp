//
//  main.cpp
//  Zadanie_2_ispravlennoe
//
//  Created by Anastasiya Charykova on 18.11.2024.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

void vectorAdd(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

void parallelVectorAdd(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int numThreads) {
    int length = a.size();
    std::vector<std::thread> threads;
    int blockSize = length / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * blockSize;
        int end = (i == numThreads - 1) ? length : start + blockSize; // Обработка остатка
        threads.emplace_back(vectorAdd, std::ref(a), std::ref(b), std::ref(result), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {

   // Определяем количество доступных ядер
      unsigned int numHardwareThreads =
      std::thread::hardware_concurrency(); // получаем количество ядер
      std::cout << "Колличество аппаратных ядер: " << numHardwareThreads
     << std::endl;// выводим количество ядер
    
    std::cout << std::endl;
    
    // Размеры массивов для тестирования
    std::vector<int> sizes = {1000, 10000, 100000, 1000000};
    std::vector<int> threadCounts = {1, 2, 4, 8, 16}; // Включены 1, 2, 4, 8 и 16 потоков

    // Вывод заголовка (setw() - колличество отступов)
    std::cout << std::setw(6) << "" << std::setw(16) << "1000" << std::setw(17) << "10000" << std::setw(17) << "100000" << std::setw(16) << "1000000" << std::endl;

    for (int numThreads : threadCounts) {
        std::cout << std::setw(2) << numThreads << " потоков"; // Вывод количества потоков
        for (int size : sizes) {
            // Создаем два вектора
            std::vector<int> a(size, 1); // Заполняем вектор 1
            std::vector<int> b(size, 2); // Заполняем вектор 2
            std::vector<int> result(size);

            auto start = std::chrono::high_resolution_clock::now();
            parallelVectorAdd(a, b, result, numThreads);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << std::setw(15) << duration.count() * 0.001 << "s"; // Вывод времени в секундах
        }
        std::cout << std::endl; // Новая строка после каждой строки с потоками
    }

    return 0;
}
