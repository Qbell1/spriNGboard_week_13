#include <iostream>
#include <thread>

#include <catch2/catch_test_macros.hpp>

using namespace std;

void print_letter (char letter, size_t num_times)
{
  for (size_t idx=0; idx<num_times; ++idx) {
    cout << letter << flush;
  }
}

TEST_CASE("letters example", "")
{
  cout << "this will print before any letters" << endl;
  std::thread t_1(print_letter, 'a', 100);
  std::thread t_2(print_letter, 'b', 100);
  std::thread t_3(print_letter, 'c', 100);
  // LINE A
  t_1.join();
  t_2.join();
  t_3.join();
  cout << endl << "this will print after all letters" << endl;
}
  // The way this prints will be decided by the OS and the scheduler.

  // runs as expected but i notice that A is always the first letter to print but B is not always the 2nd letter to print.

  // When LINE A is reached, there are 4 threads running.

  // the lines execute but a test fails. The cores get dumped because the threads are not joined/detached so the OS recognizes it as an error.

  // I cant see any other problems with this code.
