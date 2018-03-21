/*
 *
 *  Piotr Szczygieł 2018
 *  Algorytmy i struktury danych
 *
 *      Porównanie algorytmów sortujących
 *
 */


#include <chrono>
#include <iostream>
#include <random>


// Utility functions
void allocate_tables();
void copy_tables();
void print_table(uint64_t *table, size_t size);
void randomize_table(uint64_t *table, size_t size, uint64_t max);
void randomize_tables(uint64_t max);

template<typename F, typename ...Args>
std::chrono::milliseconds::rep measure_time(F&& func, Args&&... args);


// Sorting algorithms
void sort_bubble(uint64_t *table, size_t size);


// Pointers to used tables
uint64_t *table_100;
uint64_t *table_1K;
uint64_t *table_10K;
uint64_t *table_100K;
uint64_t *table_1M;

uint64_t *copy_100;
uint64_t *copy_1K;
uint64_t *copy_10K;
uint64_t *copy_100K;
uint64_t *copy_1M;


int main() {
    std::ios_base::sync_with_stdio(false);

    allocate_tables();
    randomize_tables(UINT64_MAX);
    copy_tables();

    std::cin.ignore();
    return 0;
}


void sort_bubble(uint64_t *table, size_t size) {
    size_t i, j;
    bool swapped;
    for (i = 0; i < size - 1; i++) {
        swapped = false;
        for (j = 0; j < size - i - 1; j++) {
            if (table[j] > table[j + 1]) {
                std::swap(table[j], table[j + 1]);
                swapped = true;
            }
        }

        if (swapped == false) {
            break;
        }
    }
}


template<typename F, typename ...Args>
std::chrono::milliseconds::rep measure_time(F&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
    
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}


void print_table(uint64_t *table, size_t size) {
    std::cout << "*** PRINTING TABLE OF " << size << " ELEMENTS ***" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << i+1 << ": \t\t\t" << table[i] << std::endl;
    }
    std::cout << "*** END ***" << std::endl;
}


void randomize_table(uint64_t *table, size_t size, uint64_t max) {
    std::random_device rd;
    std::mt19937_64 eng(rd());

    std::uniform_int_distribution<uint64_t> distr;

    for (size_t i = 0; i < size; i++) {
        table[i] = distr(eng) % max;
    }
}


void allocate_tables() {
    std::cout << "Allocating memory for tables" << std::endl;
    table_100  = new uint64_t[100];
    table_1K   = new uint64_t[1000];
    table_10K  = new uint64_t[10000];
    table_100K = new uint64_t[100000];
    table_1M   = new uint64_t[1000000];

    copy_100   = new uint64_t[100];
    copy_1K    = new uint64_t[1000];
    copy_10K   = new uint64_t[10000];
    copy_100K  = new uint64_t[100000];
    copy_1M    = new uint64_t[1000000];
}


void randomize_tables(uint64_t max) {
    std::cout << "Filling tables with random numbers in [0, " << max << "] range" << std::endl;
    randomize_table(table_100,  100,     max);
    randomize_table(table_1K,   1000,    max);
    randomize_table(table_10K,  10000,   max);
    randomize_table(table_100K, 100000,  max);
    randomize_table(table_1M,   1000000, max);
}


void copy_tables() {
    std::cout << "Copying values to temporary arrays" << std::endl;
    for (size_t i = 0; i < 100; i++)     copy_100[i]  = table_100[i];
    for (size_t i = 0; i < 1000; i++)    copy_1K[i]   = table_1K[i];
    for (size_t i = 0; i < 10000; i++)   copy_10K[i]  = table_10K[i];
    for (size_t i = 0; i < 100000; i++)  copy_100K[i] = table_100K[i];
    for (size_t i = 0; i < 1000000; i++) copy_1M[i]   = table_1M[i];
}
