#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false), taskCounter(0), completedTasks(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();

                    // Increment the completed task counter
                    {
                        std::unique_lock<std::mutex> lock(completedTasksMutex);
                        completedTasks++;
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        stopThreads();

        condition.notify_all();

        for (std::thread& worker : workers) {
            worker.join();
        }
    }

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([f, args...] { f(args...); });
            taskCounter++;
        }

        condition.notify_one();
    }

    bool allTasksCompleted() {
        std::unique_lock<std::mutex> lock(completedTasksMutex);
        return taskCounter == completedTasks;
    }
    void stopThreads() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }

        condition.notify_all();
    }
    bool GetStop(){
        return stop;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    size_t taskCounter;
    size_t completedTasks;
    std::mutex completedTasksMutex;
};