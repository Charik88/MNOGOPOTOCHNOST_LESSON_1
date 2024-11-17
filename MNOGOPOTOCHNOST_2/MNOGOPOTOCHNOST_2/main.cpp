//
//  main.cpp
//  MNOGOPOTOCHNOST_2
//
//  Created by Anastasiya Charykova on 16.11.2024.
//
/*Задание 2
 Параллельные вычисления

     Напишите программу для расчёта суммы двух векторов.
     Распараллельте эту программу на 2, 4, 8 и 16 потоков.
     Определите, какое количество потоков даёт самый быстрый результат.
     Сравните результаты выполнения для массивов из 1 000, 10 000, 100 000 и 1 000 000 элементов.
     Время выполнения для каждого варианта сведите в таблицу и выведите её в консоль.
     Первый запущенный поток должен вывести на экран доступное количество аппаратных ядер.
*/

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
        int end = (i == numThreads - 1) ? length : start + blockSize; // Ensure the last thread processes the remainder
        threads.emplace_back(vectorAdd, std::ref(a), std::ref(b), std::ref(result), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    // Определяем количество доступных ядер
    unsigned int numHardwareThreads = std::thread::hardware_concurrency();
    std::cout << "Available hardware threads: " << numHardwareThreads << std::endl;

    // Размеры массивов для тестирования
    std::vector<int> sizes = {1000, 10000, 100000, 1000000};
    std::vector<int> threadCounts = {2, 4, 8, 16};

    std::cout << std::setw(15) << "Size" << std::setw(15) << "Threads" << std::setw(20) << "Time (ms)" << std::endl;

    for (int size : sizes) {
        // Создаем два вектора
        std::vector<int> a(size, 1); // Заполняем вектор 1
        std::vector<int> b(size, 2); // Заполняем вектор 2
        std::vector<int> result(size);

        for (int numThreads : threadCounts) {
            auto start = std::chrono::high_resolution_clock::now();
            parallelVectorAdd(a, b, result, numThreads);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            std::cout << std::setw(15) << size << std::setw(15) << numThreads << std::setw(20) << duration.count() << std::endl;
        }
    }

    return 0;
}

