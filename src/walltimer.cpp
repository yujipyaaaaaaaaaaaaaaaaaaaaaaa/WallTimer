#include "walltimer.hpp"

std::unique_ptr<WallTimer> WallTimer::wt = nullptr;
// inline展開を防ぐためにクラス外定義をする
FuncTimer::FuncTimer(const std::string &func_name, const uint64_t &return_pc, const std::string &return_func)
{
  funcName = func_name;
  uint64_t startPC = reinterpret_cast<uint64_t>(__builtin_return_address(0));
  returnPC = return_pc;
  WallTimer::GetInstance().StartFuncTime(func_name, startPC, return_pc, return_func);
}

FuncTimer::~FuncTimer()
{
  uint64_t endPC = reinterpret_cast<uint64_t>(__builtin_return_address(0));
  WallTimer::GetInstance().EndFuncTime(funcName, returnPC, endPC);
}

