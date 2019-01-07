#ifndef _WALL_TIMER_HPP_
#define _WALL_TIMER_HPP_

#include <chrono>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <tuple>
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

#define FUNC_TIMER \
                uint64_t returnPC128473801 = reinterpret_cast<uint64_t>(__builtin_return_address(0));\
                auto temp8234901684 = FuncTimer(__FUNCTION__, returnPC128473801);
#define CLEAR_RAP_TIMER WallTimer::GetInstance().ClearRapTime();
#define CLEAR_FUNC_TIMER WallTimer::GetInstance().ClearFuncTime();
#define RAP_TIMER WallTimer::GetInstance().RapTimeStack();
#define OUTPUT_FUNC_TIME WallTimer::GetInstance().OutputFuncTime();
#define OUTPUT_RAP_TIME  WallTimer::GetInstance().OutputRapTime();


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
    std::map<std::thread::id, std::vector<std::chrono::system_clock::time_point>> startTime;
    std::map<std::thread::id, std::vector<std::chrono::system_clock::time_point>> endTime;
    std::map<std::thread::id, std::vector<uint64_t>> diffTime;
    bool returnFuncSearched;
    uint64_t lastDiffTime;

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
      auto thread_id = std::this_thread::get_id();
      if(startTime.count(thread_id))
      {
        startTime[thread_id].push_back(std::chrono::system_clock::now());
      }
      else
      {
        std::vector<std::chrono::system_clock::time_point> vec;
        vec.push_back(std::chrono::system_clock::now());
        startTime.insert(std::make_pair(thread_id, vec));
      }
    }

    void EndLogger(const uint64_t &end_pc)
    {
      auto thread_id = std::this_thread::get_id();
      endPC = end_pc;
      if(endTime.count(thread_id))
      {
        endTime[thread_id].push_back(std::chrono::system_clock::now());
        diffTime[thread_id].push_back(std::chrono::duration_cast<std::chrono::nanoseconds>
          (endTime[thread_id].back() - startTime[thread_id].back()).count());
      }
      else
      {
        std::vector<std::chrono::system_clock::time_point> end;
        std::vector<uint64_t> diff;
        auto now = std::chrono::system_clock::now();
        end.push_back(now);
        diff.push_back((now - startTime[thread_id].back()).count());
        endTime.insert(std::make_pair(thread_id, end));
        diffTime.insert(std::make_pair(thread_id, diff));

      }
      lastDiffTime = diffTime[thread_id].back();
    }

    static std::string OutputHeader()
    {
      std::ostringstream ost;
      ost << "| FunctionName | ReturnFunctionName | Last Proc Time[ns] | Average[ns] | Total[ns] | Called |" << std::endl;
      ost << "|:-------------|:-------------------|-------------------:|------------:|----------:|-------:|" << std::endl;
      return ost.str();
    }

    std::string OutputBody()
    {
      std::ostringstream ost;
      uint64_t total = 0;
      uint32_t called = 0;
      for(const auto &threadEach : diffTime)
      {
        for(const auto &time : threadEach.second)
        {
          total += time;
          ++called;
        }
      }

      if(called > 0)
      {
        ost << "|" << funcName
            << "|" << returnFuncName
            << "|" << lastDiffTime
            << "|" << total / called
            << "|" << total
            << "|" << called
            << "|" << std::endl;
      }
      return ost.str();
    }

  };

  static std::unique_ptr<WallTimer> wt;
  std::vector<std::chrono::system_clock::time_point> rapTime;
  std::map<std::tuple<std::string, uint64_t>, funcLogger> funcTime;
  std::mutex mtx_;

  WallTimer(){}

  void parentFunctionMatching()
  {
    for(auto &ft : funcTime){
      auto &func = ft.second;
      if(!func.ReturnFuncSearched())
      {
        for(auto &tft : funcTime)
        {
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


  //todo 確実にデストラクタを呼ぶにはどうすればいいか
  //todo 例えばCtrl-cで終了した際にも呼びたい
  ~WallTimer()
  {
    OutputFuncTime("FunctionTime.log");
    OutputRapTime("RapTime.log");
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
    std::lock_guard<std::mutex> lock(mtx_);
    rapTime.clear();
  }

  // FunTime vector clear
  void ClearFuncTime()
  {
    std::lock_guard<std::mutex> lock(mtx_);
    funcTime.clear();
  }

  void StartFuncTime(const std::string &func, const uint64_t &start_pc, const uint64_t &return_pc)
  {
    // 要素がある場合は1、ない場合は0
    std::lock_guard<std::mutex> lock(mtx_);
    auto tuple = std::tuple<std::string, uint64_t>(func, return_pc);
    if(funcTime.count(tuple)){
      funcTime[tuple].StartLogger();
    }
    else
    {
      funcLogger fl(func, start_pc, return_pc);
      funcTime.insert(std::make_pair(tuple, fl));
    }
  }

  void EndFuncTime(const std::string &func, const uint64_t &return_pc, const uint64_t &end_pc)
  {
    std::lock_guard<std::mutex> lock(mtx_);
    auto tuple = std::tuple<std::string, uint64_t>(func, return_pc);
    funcTime[tuple].EndLogger(end_pc);
  }

  void OutputFuncTime(std::string filename="")
  {
    parentFunctionMatching();

    std::ostringstream ost;
    ost << funcLogger::OutputHeader();
    for(auto &func : funcTime){
      ost << func.second.OutputBody();
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
    std::lock_guard<std::mutex> lock(mtx_);
    rapTime.push_back(std::chrono::system_clock::now());
  }

  void OutputRapTime(std::string filename="")
  {
    std::ostringstream ost;
    ost << "RapTime, ";
    for (size_t i = 0; i < rapTime.size(); ++i) {
      // auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(rapTime[i].time_since_epoch()).count();
      auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(rapTime[i] - rapTime[0]).count();
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

class FuncTimer{
private:
  FuncTimer() = delete;
  uint64_t returnPC;
  std::string funcName;

public:
  FuncTimer(const std::string &func_name, const uint64_t &return_pc);
  ~FuncTimer();
};
#endif // _WALL_TIMER_HPP_
