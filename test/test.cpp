#include "../src/walltimer.hpp"

#include <chrono>
#include <thread>

void test1()
{
  auto temp = FuncTimer(__FUNCTION__);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test2()
{
  // ; is not needed, but put.
  FUNC_TIMER;
  std::this_thread::sleep_for(std::chrono::seconds(2));
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

      WallTimer::GetInstance().RapTimeStack();
      test1();
      RAP_TIMER;
      test2();
      WallTimer::GetInstance().RapTimeStack();

      std::cout << "trial:" << i << "-" << j << std::endl;
      timer.OutputFuncTime();
      timer.OutputRapTime();
      std::cout << std::endl;
    }
  }
  return 0;
}
