#include "../src/walltimer.hpp"

#include <chrono>
#include <thread>

void test1()
{
  auto temp = FuncTimer(__FUNCTION__);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void test2()
{
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int main()
{

  for(int i = 0; i < 2; ++i)
  {
    auto &timer = WallTimer::GetInstance();
    timer.ClearFuncTime();
    for(int j = 0; j < 2; ++j)
    {
      timer.ClearRapTime();

      timer.RapTimeStack();
      test1();
      RAP_TIMER;
      test2();
      timer.RapTimeStack();

      std::cout << "trial:" << i << "-" << j << std::endl;
      // instance access
      timer.OutputFuncTime();
      // macro access
      OUTPUT_RAP_TIME;
      std::cout << std::endl;
    }
  }
  return 0;
}
