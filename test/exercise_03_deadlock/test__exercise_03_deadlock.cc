#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include <catch2/catch_test_macros.hpp>

using namespace std;

void increment_counter_ver_1 (size_t & counter, std::mutex & mutex_a, std::mutex & mutex_b, size_t num_times)
{
  for (size_t idx=0; idx<num_times; ++idx) {
    this_thread::sleep_for(1ms); // simluate a complicated calculation
    {
      lock_guard<std::mutex> lg_a(mutex_a);
      lock_guard<std::mutex> lg_b(mutex_b);
      counter++;
    }
  }
}

void increment_counter_ver_2 (size_t & counter, std::mutex & mutex_a, std::mutex & mutex_b, size_t num_times)
{
  for (size_t idx=0; idx<num_times; ++idx) {
    this_thread::sleep_for(1ms); // simluate a complicated calculation
    {
      lock_guard<std::mutex> lg_a(mutex_a);
      lock_guard<std::mutex> lg_b(mutex_b);
      counter++;
    }
  }
}

TEST_CASE("deadlock example")
{
  size_t const num_times   = 1'000;
  size_t       counter     = 0;
  std::mutex   mutex_x;
  std::mutex   mutex_y;

  std::thread t_1(increment_counter_ver_1, std::ref(counter), std::ref(mutex_x), std::ref(mutex_y), num_times);
  std::thread t_2(increment_counter_ver_2, std::ref(counter), std::ref(mutex_x), std::ref(mutex_y), num_times);
  t_1.join();
  t_2.join();
  cout << "display counter:" << counter;

  REQUIRE(counter == 2*num_times);
}

// ver_1 locks a while ver_2 locks b but 1 needs b after and 2 needs a after so this creates a deadlock.

// The code just pauses and doesnt show a result because its deadlocked.
