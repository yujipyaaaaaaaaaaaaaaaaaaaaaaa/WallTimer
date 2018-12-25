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

class WallTimer{
private:
  class funcLogger
  {
  private:
    std::string funcName;
    std::string returnFuncName;
    uint64_t startPC;
    uint64_t endPC;
    uint64_t returnPC;
    std::vector<std::chrono::system_clock::time_point> startTime;
    std::vector<std::chrono::system_clock::time_point> endTime;
    std::vector<uint64_t> diffTime;
    bool returnFuncSearched;

  // setter and getter
  public:
    const std::string & FuncName() const { return funcName; }
    const uint64_t & StartPC() const { return startPC; }
    const uint64_t & EndPC() const { return endPC; }
    const uint64_t & ReturnPC() const { return returnPC; }

    const bool & ReturnFuncSearched() { return returnFuncSearched; }

    // is this setter?
    void ReturnFunc(std::string func_name)
    {
      returnFuncName = func_name;
      returnFuncSearched = true;
    }

  public:
    funcLogger()
    {
      funcName = "unknown";
      returnFuncName = "unknown";
      startPC = 0; endPC = 0; returnPC = 0;
      startTime.clear();
      endTime.clear();
      diffTime.clear();
      returnFuncSearched = false;
    }

    funcLogger(const std::string &func, const uint64_t &start_pc, const uint64_t &return_pc)
    : funcLogger()
    {
      StartLogger(func, start_pc, return_pc);
      returnFuncName = "unknown";
    }

    ~funcLogger()
    {
      startTime.clear();
      endTime.clear();
      diffTime.clear();
    }

    void StartLogger(const std::string &func, const uint64_t &start_pc, const uint64_t &return_pc)
    {
      funcName = func;
      startPC = start_pc;
      returnPC = return_pc;
      StartLogger();
    }

    void StartLogger()
    {
      startTime.push_back(std::chrono::system_clock::now());
    }

    void EndLogger(const uint64_t &end_pc)
    {
      endPC = end_pc;
      endTime.push_back(std::chrono::system_clock::now());
      auto aaa = endTime.back() - startTime.back();
      diffTime.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime.back() - startTime.back()).count());
    }

    std::string OutputBase()
    {
      std::ostringstream ost;
      uint64_t total = 0;
      uint32_t called = 0;
      for(const auto &c : diffTime)
      {
        total += c;
        ++called;
      }

      if(called > 0)
      {
        ost << "FunctionName:" << funcName
            << ",ReturnFunctionName:" << returnFuncName
            << ",time:" << diffTime.back()
            << ",average:" << total / called
            << ",total:" << total
            << ",called:" << called
            // << "," << startPC
            // << "," << endPC
            // << "," << returnPC
            << std::endl;
      }
      return ost.str();
    }

  };

  static std::unique_ptr<WallTimer> wt;
  std::vector<std::chrono::system_clock::time_point> rapTime;
  std::map<uint64_t, funcLogger> funcTime;
  WallTimer(){}

  void parentFunctionMatching()
  {
    for(auto &ft : funcTime){
      auto &func = ft.second;
      if(!func.ReturnFuncSearched())
      {
        for(auto &tft : funcTime){
          const auto &tfunc = tft.second;
          if(tfunc.StartPC() < func.ReturnPC() && func.ReturnPC() < tfunc.EndPC())
          {
            func.ReturnFunc(tfunc.FuncName());
            break;
          }
        }
      }
    }
  }

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
    funcTime.clear();
  }

  void StartFuncTime(const std::string &func, const uint64_t &start_pc, const uint64_t &return_pc)
  {
    // 要素がある場合は1、ない場合は0
    if(funcTime.count(return_pc)){
      funcTime[return_pc].StartLogger();
    }
    else
    {
      funcLogger fl(func, start_pc, return_pc);
      funcTime.insert(std::make_pair(return_pc, fl));
    }
  }
  void EndFuncTime(const uint64_t &return_pc, const uint64_t &end_pc)
  {
      funcTime[return_pc].EndLogger(end_pc);
  }

  void OutputFuncTime(std::string filename="")
  {
    parentFunctionMatching();

    std::ostringstream ost;
    for(auto &func : funcTime){
      ost << func.second.OutputBase();
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
  uint64_t returnPC;

public:
  FuncTimer(const std::string &func_name, const uint64_t &return_pc);
  ~FuncTimer();
};
// inline展開を防ぐためにクラス外定義をする
FuncTimer::FuncTimer(const std::string &func_name, const uint64_t &return_pc){
  uint64_t startPC = (uint64_t) __builtin_return_address(0);
  returnPC = return_pc;
  WallTimer::GetInstance().StartFuncTime(func_name, startPC, return_pc);
}
FuncTimer::~FuncTimer(){
  uint64_t endPC = (uint64_t) __builtin_return_address(0);
  auto &func = WallTimer::GetInstance();
  func.EndFuncTime(returnPC, endPC);
}


#define FUNC_TIMER \
                uint64_t returnPC128473801 = (uint64_t)__builtin_return_address(0);\
                auto temp8234901684 = FuncTimer(__FUNCTION__, returnPC128473801);
#define CLEAR_RAP_TIMER WallTimer::GetInstance().ClearRapTime();
#define CLEAR_FUNC_TIMER WallTimer::GetInstance().ClearFuncTime();
#define RAP_TIMER WallTimer::GetInstance().RapTimeStack();
#define OUTPUT_FUNC_TIME WallTimer::GetInstance().OutputFuncTime();
#define OUTPUT_RAP_TIME  WallTimer::GetInstance().OutputRapTime();

#endif // _WALL_TIMER_HPP_
