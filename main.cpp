#include "src/walltimer.hpp"

#include <chrono>
#include <thread>

void test1()
{
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void test2()
{
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  test1();
}

void test3()
{
  FUNC_TIMER;
  for(int i = 0; i < 2; ++i)
  {
    for(int j = 0; j < 2; ++j)
    {
      CLEAR_RAP_TIMER;

      RAP_TIMER;
      test1();
      RAP_TIMER;
      test2();
      RAP_TIMER;

      std::cout << "trial:" << i << "-" << j << std::endl;
      // instance access
      OUTPUT_FUNC_TIME;
      // macro access
      OUTPUT_RAP_TIME;
      std::cout << std::endl;
    }
  }
}

int main()
{
  test3();
  return 0;
}
