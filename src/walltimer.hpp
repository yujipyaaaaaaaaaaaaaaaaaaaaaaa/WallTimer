#ifndef _WALL_TIMER_HPP_
#define _WALL_TIMER_HPP_

#include <chrono>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
// #define __USE_GNU
// #include <dlfcn.h>

class WallTimer{
private:
  static std::unique_ptr<WallTimer> wt;

  std::vector<std::chrono::system_clock::time_point> rapTime;
  std::map<std::string, std::vector<std::chrono::system_clock::time_point>> funcTime;
  WallTimer(){}

public:
  WallTimer(const WallTimer &) = delete;
  WallTimer & operator=(const WallTimer &) = delete;
  WallTimer(WallTimer &&) = delete;
  WallTimer & operator=(WallTimer &&) = delete;


  // 最後に出力する
  ~WallTimer()
  {
    __builtin_return_address(0);
    std::cout << "destruction" << std::endl;
    OutputFuncTime();
    OutputRapTime();
  }
  // get singlton instance
  static WallTimer & GetInstance()
  {
    if(wt == nullptr)
    {
      wt = std::unique_ptr<WallTimer>(new WallTimer());
    }
    return *wt;
  }

  // RapTime vector clear
  void ClearRapTime()
  {
    rapTime.clear();
  }

  // FunTime vector clear
  void ClearFuncTime()
  {
    for(auto &func : funcTime){
      func.second.clear();
    }
    funcTime.clear();
  }

  void FuncTimeStack(std::string func_name)
  {
    // 要素がある場合は1、ない場合は0
    if(funcTime.count(func_name)){
      funcTime[func_name].push_back(std::chrono::system_clock::now());
    }
    else
    {
      std::vector<std::chrono::system_clock::time_point> second;
      second.push_back(std::chrono::system_clock::now());
      funcTime.insert(std::make_pair(func_name, second));

    }
  }

  void OutputFuncTime(std::string filename="")
  {
    std::ostringstream ost;
    for(auto &func : funcTime){
      auto &timeVec = func.second;

      int64_t total = 0;
      uint   called = 0;

      for(size_t i = 1; i < timeVec.size(); i += 2){
        auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(timeVec[i - 0] - timeVec[i - 1]).count();
        total += count;
        ++called;
      }
      ost << "FunctionName, " << func.first << ", total, " << total << ", called, " << called << ", average, " << total / called << std::endl;
    }

    std::string outstr = ost.str();
    std::cout << outstr;
    if(filename != ""){
      std::ofstream elapsedlogger;
      elapsedlogger.open(filename, std::ios::app);
      elapsedlogger << outstr;
      elapsedlogger.close();
    }

  }

  void RapTimeStack()
  {
    rapTime.push_back(std::chrono::system_clock::now());
  }

  void OutputRapTime(std::string filename="")
  {
    std::ostringstream ost;
    ost << "RapTime, ";
    for (size_t i = 1; i < rapTime.size(); ++i) {
      auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(rapTime[i - 0] - rapTime[i - 1]).count();
      ost << count << ",";
    }
    ost << std::endl;
    std::string outstr = ost.str();
    std::cout << outstr;

    if(filename != ""){
      std::ofstream elapsedlogger;
      elapsedlogger.open(filename, std::ios::app);
      elapsedlogger << outstr;
      elapsedlogger.close();
    }
  }

};
std::unique_ptr<WallTimer> WallTimer::wt = nullptr;

class FuncTimer{
private:
  FuncTimer() = delete;
  std::string func;

public:
  FuncTimer(std::string func_name){
    func = func_name;
    WallTimer::GetInstance().FuncTimeStack(func);
  }
  ~FuncTimer(){
    WallTimer::GetInstance().FuncTimeStack(func);
  }
};

// #define FUNC_TIMER std::cout << __builtin_return_address(0) << std::endl;;\
//                    Dl_info info;\
//                    dladdr(__builtin_return_address(0), &info);\
//                    auto temp8234901684 = FuncTimer(__FUNCTION__);
#define FUNC_TIMER auto temp8234901684 = FuncTimer(__FUNCTION__);
#define RAP_TIMER WallTimer::GetInstance().RapTimeStack();
#define OUTPUT_FUNC_TIME WallTimer::GetInstance().OutputFuncTime();
#define OUTPUT_RAP_TIME  WallTimer::GetInstance().OutputRapTime();

#endif // _WALL_TIMER_HPP_
