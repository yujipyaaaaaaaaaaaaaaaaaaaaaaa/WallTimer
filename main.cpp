#include "src/walltimer.hpp"

#include <chrono>
#include <thread>

void test1()
{
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void test2()
{
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  test1();
}

void test3()
{
  FUNC_TIMER;
  for(int i = 0; i < 100; ++i)
  {
    RAP_TIMER;
    test1();
    RAP_TIMER;
    test2();
    RAP_TIMER;
 }
}

void test4()
{
  FUNC_TIMER;
  for(int i = 0; i < 100; ++i)
  {
    std::thread th1(test1);
    std::thread th2(test2);
    th1.join();
    th2.join();
 }


}

int main()
{
  test3();

  OUTPUT_RAP_TIME;
  OUTPUT_FUNC_TIME;
  std::cout << std::endl;

  CLEAR_RAP_TIMER;
  CLEAR_FUNC_TIMER;

  std::thread th1(test4);
  std::thread th2(test3);
  th1.join();
  th2.join();

  return 0;
}
