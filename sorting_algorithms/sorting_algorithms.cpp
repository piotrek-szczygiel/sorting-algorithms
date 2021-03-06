/*
 *
 *  Piotr Szczygieł 2018
 *  Algorytmy i struktury danych
 *
 *      Porównanie algorytmów sortujących
 *
 */


#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>


void allocate_tables();
void free_tables();
void randomize_tables(int64_t max);
void test_sorting_algorithms();


void sort_quick(int64_t *table, int64_t size);
std::vector<std::chrono::microseconds::rep> times_quick[5];

void sort_quick_iterative(int64_t *table, int64_t size);
std::vector<std::chrono::microseconds::rep> times_quick_iterative[5];

void sort_radix(int64_t *table, int64_t size);
std::vector<std::chrono::microseconds::rep> times_radix[5];


int64_t *table_100;
int64_t *table_1K;
int64_t *table_10K;
int64_t *table_100K;
int64_t *table_1M;

int64_t *copy_100;
int64_t *copy_1K;
int64_t *copy_10K;
int64_t *copy_100K;
int64_t *copy_1M;


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::left;

    allocate_tables();

    randomize_tables(INT8_MAX);
    test_sorting_algorithms();

    randomize_tables(INT16_MAX);
    test_sorting_algorithms();

    randomize_tables(INT32_MAX);
    test_sorting_algorithms();

    randomize_tables(INT64_MAX);
    test_sorting_algorithms();

    std::cout << std::endl << std::endl;

    std::cout << "Consecutive columns are ranges:     INT8      INT16     INT32     INT64" << std::endl;
    for (int64_t i = 0; i < 5; i++) {
        switch (i) {
        case 0:
            std::cout << "* 100 elements" << std::endl;
            break;
        case 1:
            std::cout << "* 1K elements" << std::endl;
            break;
        case 2:
            std::cout << "* 10K elements" << std::endl;
            break;
        case 3:
            std::cout << "* 100K elements" << std::endl;
            break;
        case 4:
            std::cout << "* 1M elements" << std::endl;
            break;
        }

        std::cout << std::setw(36) << "Quick sort times:";
        for (auto &time : times_quick[i]) {
            std::cout << std::setw(10) << time;
        }
        std::cout << std::endl;

        std::cout << std::setw(36) << "Quick sort iterative times:";
        for (auto &time : times_quick_iterative[i]) {
            std::cout << std::setw(10) << time;
        }
        std::cout << std::endl;

        std::cout << std::setw(36) << "Radix sort times:";
        for (auto &time : times_radix[i]) {
            std::cout << std::setw(10) << time;
        }
        std::cout << std::endl << std::endl;
    }

    std::cout << std::endl << "Press any key to close the program." << std::endl;
    std::cin.ignore();

    free_tables();
    return 0;
}


/*
 *  Sorting algorithms
 */

// Quick sort
int64_t partition(int64_t *table, int64_t low, int64_t high) {
    int64_t pivot = table[high];
    int64_t i = low - 1;

    for (int64_t j = low; j <= high - 1; j++) {
        if (table[j] <= pivot) {
            i++;
            std::swap(table[i], table[j]);
        }
    }

    std::swap(table[i + 1], table[high]);
    return i + 1;
}

void quick_sort(int64_t *table, int64_t low, int64_t high) {
    if (low < high) {
        int64_t partitioning_index = partition(table, low, high);
        quick_sort(table, low, partitioning_index - 1);
        quick_sort(table, partitioning_index + 1, high);
    }
}

void sort_quick(int64_t *table, int64_t size) {
    quick_sort(table, 0, size - 1);
}


// Quick sort iterative version with stack
void quick_sort_iterative(int64_t *table, int64_t low, int64_t high) {
    int64_t *stack = new int64_t[high - low + 1];

    int64_t top = -1;

    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        high = stack[top--];
        low = stack[top--];

        int64_t partitioning_index = partition(table, low, high);

        if (partitioning_index - 1 > low) {
            stack[++top] = low;
            stack[++top] = partitioning_index - 1;
        }

        if (partitioning_index + 1 < high) {
            stack[++top] = partitioning_index + 1;
            stack[++top] = high;
        }
    }

    delete stack;
}

void sort_quick_iterative(int64_t *table, int64_t size) {
    quick_sort_iterative(table, 0, size - 1);
}


// Radix sort
int64_t max_element(int64_t *table, int64_t size) {
    int64_t max = table[0];
    for (int64_t i = 1; i < size; i++) {
        if (table[i] > max) {
            max = table[i];
        }
    }

    return max;
}

void count_sort(int64_t *table, int64_t size, int64_t exp) {
    int64_t *output = new int64_t[size];
    int64_t i;
    int64_t count[10] = { 0 };

    for (i = 0; i < size; i++) {
        count[(table[i] / exp) % 10]++;
    }

    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (i = size - 1; i >= 0; i--) {
        output[count[(table[i] / exp) % 10] - 1] = table[i];
        count[(table[i] / exp) % 10]--;
    }

    for (i = 0; i < size; i++) {
        table[i] = output[i];
    }

    delete output;
}

void sort_radix(int64_t *table, int64_t size) {
    int64_t m = max_element(table, size);

    for (int64_t exp = 1; m / exp > 0; exp *= 10) {
        count_sort(table, size, exp);
    }
}


/*
 *  Utility functions
 */
void allocate_tables() {
    table_100  = new int64_t[100];
    table_1K   = new int64_t[1000];
    table_10K  = new int64_t[10000];
    table_100K = new int64_t[100000];
    table_1M   = new int64_t[1000000];

    copy_100   = new int64_t[100];
    copy_1K    = new int64_t[1000];
    copy_10K   = new int64_t[10000];
    copy_100K  = new int64_t[100000];
    copy_1M    = new int64_t[1000000];
}


void copy_tables() {
    for (int64_t i = 0; i < 100; i++)     copy_100[i]  = table_100[i];
    for (int64_t i = 0; i < 1000; i++)    copy_1K[i]   = table_1K[i];
    for (int64_t i = 0; i < 10000; i++)   copy_10K[i]  = table_10K[i];
    for (int64_t i = 0; i < 100000; i++)  copy_100K[i] = table_100K[i];
    for (int64_t i = 0; i < 1000000; i++) copy_1M[i]   = table_1M[i];
}


void free_tables() {
    delete[] table_100;
    delete[] table_1K;
    delete[] table_10K;
    delete[] table_100K;
    delete[] table_1M;

    delete[] copy_100;
    delete[] copy_1K;
    delete[] copy_10K;
    delete[] copy_100K;
    delete[] copy_1M;
}


template<typename F, typename ...Args>
std::chrono::microseconds::rep measure_time(F &&func, Args &&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    std::forward<decltype(func)>(func)(std::forward<Args>(args)...);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count();
}


void randomize_table(int64_t *table, int64_t size, int64_t max) {
    std::random_device rd;
    std::mt19937_64 eng(rd());

    std::uniform_int_distribution<int64_t> distr;

    for (int64_t i = 0; i < size; i++) {
        table[i] = distr(eng) % max;
    }
}


void randomize_tables(int64_t max) {
    std::cout << "* Filling tables with random numbers in [0, " << max << "] range" << std::endl;
    randomize_table(table_100,  100,     max);
    randomize_table(table_1K,   1000,    max);
    randomize_table(table_10K,  10000,   max);
    randomize_table(table_100K, 100000,  max);
    randomize_table(table_1M,   1000000, max);
}


void test_sorting_algorithm(std::string name, std::function<void(int64_t *, int64_t)> sort, std::vector<std::chrono::microseconds::rep> *times) {
    std::chrono::microseconds::rep current_time = 0;

    copy_tables();
    std::cout << "Testing " << name << ":" << std::endl;

    current_time = measure_time(sort, copy_100, 100);
    times[0].push_back(current_time);
    std::cout << std::setw(18) << "  100 elements:" << current_time << "us" << std::endl;

    current_time = measure_time(sort, copy_1K, 1000);
    times[1].push_back(current_time);
    std::cout << std::setw(18) << "  1K elements:" << current_time << "us" << std::endl;

    current_time = measure_time(sort, copy_10K, 10000);
    times[2].push_back(current_time);
    std::cout << std::setw(18) << "  10K elements:" << current_time << "us" << std::endl;

    current_time = measure_time(sort, copy_100K, 100000);
    times[3].push_back(current_time);
    std::cout << std::setw(18) << "  100K elements:" << current_time << "us" << std::endl;

    current_time = measure_time(sort, copy_1M, 1000000);
    times[4].push_back(current_time);
    std::cout << std::setw(18) << "  1M elements:" << current_time << "us" << std::endl;

}


void test_sorting_algorithms() {
    test_sorting_algorithm("Quick sort", sort_quick, times_quick);
    test_sorting_algorithm("Quick sort iterative", sort_quick_iterative, times_quick_iterative);
    test_sorting_algorithm("Radix sort", sort_radix, times_radix);
    std::cout << std::endl;
}