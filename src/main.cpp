#include <chrono>
#include <thread>
#include <iostream>
#include <random>

int64_t get_random_duration_us() {
  return 10 + (std::rand() % 1000);
}

int64_t get_current_time_us() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

void produce_past_logs() {
  while (true) {
    const int64_t duration = get_random_duration_us();
    const int64_t start = get_current_time_us();
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
    const auto end = get_current_time_us();

    std::cerr << "[T2] start: " << start << ", end: " << end << std::endl;
  }
}

void produce_logs() {
  while (true) {
    const int64_t duration = get_random_duration_us();

    std::cerr << "[T1] start!" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
    std::cerr << "[T1] end!" << std::endl;
  }
}

int main( )
{
  std::thread producer(produce_past_logs);
  produce_logs();

  return 0;
}
