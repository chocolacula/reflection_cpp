#pragma once

#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>

class ThreadPool {
 public:
  explicit ThreadPool(size_t thread_number) {
    _threads.reserve(thread_number);

    for (auto i = 0; i < thread_number; i++) {
      _threads.emplace_back(&ThreadPool::loop, this);
    }
  }

  ~ThreadPool() {
    _is_down = true;

    for (auto&& thread : _threads) {
      thread.join();
    }
  }

  template <typename T, typename... Args>
  auto run(T&& function, Args&&... args) {
    auto task = std::make_shared<std::packaged_task<decltype(function(args...))()>>(
        std::bind(std::forward<T>(function), std::forward<Args>(args)...));

    {
      std::lock_guard<std::mutex> lock(_mutex);

      // wrap any function
      _queue.emplace([task]() { (*task)(); });
    }

    _cv.notify_one();

    return task->get_future();
  }

 private:
  std::vector<std::thread> _threads;
  std::queue<std::function<void()>> _queue;

  std::mutex _mutex;
  std::condition_variable _cv;

  std::atomic_bool _is_down = false;

  void loop() {
    while (true) {

      std::unique_lock<std::mutex> lock(_mutex);
      _cv.wait(lock, [this] {
        // go ahead after notification only if not shutting down and not empty
        return !_is_down.load() && !_queue.empty();
      });

      auto task = _queue.front();
      _queue.pop();

      task();
    }
  }
};
