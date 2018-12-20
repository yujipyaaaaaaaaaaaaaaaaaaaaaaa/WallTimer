#ifndef _WALL_TIMER_HPP_
#define _WALL_TIMER_HPP_

#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


class WallTimer{
private:
  static WallTimer wt;

  std::vector<std::chrono::system_clock::time_point> rapTime;
  std::map<std::string, std::vector<std::chrono::system_clock::time_point>> funcTime;
  WallTimer(){ }

public:
  // get singlton instance
  static WallTimer & GetInstance(){ return wt; }

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

  void RapTimeStuck()
  {
    rapTime.push_back(std::chrono::system_clock::now());
  }

  void OutputRapTime(uint64 div=1, std::string filename="")
  {
    std::ostringstream ost;
    for (size_t i = 1; i < funcTime.size(); ++i) {
      ost << std::chrono::duration_cast<std::chrono::nanoseconds>(timevec[i - 0] - timevec[i - 1]).count() << ",";
    }
    ost << std::endl;
    std::string outstr = ost.str();
    std::cout << outstr;

  }

};


#endif // _WALL_TIMER_HPP_
