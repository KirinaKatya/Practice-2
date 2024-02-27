#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;
int sharedValue = 0;

void nonThreadSafeAccess(int id) {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread " << id << " (non-thread-safe): Shared value = " << ++sharedValue << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void synchronizedAccess(int id) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Thread " << id << " (synchronized): Shared value = " << ++sharedValue << std::endl;
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    const int numThreads = 3;
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Without synchronization:\n";
    std::thread threadsNonSafe[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        threadsNonSafe[i] = std::thread(nonThreadSafeAccess, i);
    }
    for (int i = 0; i < numThreads; ++i) {
        threadsNonSafe[i].join();
    }

    auto endWithoutSync = std::chrono::high_resolution_clock::now();
    auto durationWithoutSync = std::chrono::duration_cast<std::chrono::milliseconds>(endWithoutSync - start);
    std::cout << "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << std::endl << std::endl;
    std::cout << "Total execution time without synchronization: " << durationWithoutSync.count() << " ms\n";

    std::cout << std::endl << "____________________________________________________" << std::endl;

    sharedValue = 0; 

    auto startWithSync = std::chrono::high_resolution_clock::now();
    std::cout << "\nWith synchronization:\n";
    std::thread threadsSynchronized[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        threadsSynchronized[i] = std::thread(synchronizedAccess, i);
    }
    for (int i = 0; i < numThreads; ++i) {
        threadsSynchronized[i].join();
    }

    auto endWithSync = std::chrono::high_resolution_clock::now();
    auto durationWithSync = std::chrono::duration_cast<std::chrono::milliseconds>(endWithSync - startWithSync);
    std::cout << "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << std::endl << std::endl;
    std::cout << "Total execution time with synchronization: " << durationWithSync.count() << " ms\n";

    return 0;
}
