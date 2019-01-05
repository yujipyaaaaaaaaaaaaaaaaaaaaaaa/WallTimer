## what
this is C++ Wall time timer.

## how to use

please read test/test.cpp.
for function time.

```c++
void test1()
{
  auto temp = FuncTimer(__FUNCTION__);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void test2()
{
  FUNC_TIMER; // this is defined macro.
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

// and call for outpu
timer.OutputFuncTime();
```

for process time.

```c++

WallTimer::GetInstance().RapTimeStack();
any_process();
RAP_TIMER;  // this is defined macro
any_process();
WallTimer::GetInstance().RapTimeStack();

// and call for outpu
timer.OutputRapTime();
```

out put example. this is out by test/test.cpp of WallTimerTest.
```
trial:0-0
FunctionName:test1,ReturnFunctionName:test2,time:100290670,average:100290670,total:100290670,called:1
FunctionName:test1,ReturnFunctionName:unknown,time:100167346,average:100167346,total:100167346,called:1
FunctionName:test2,ReturnFunctionName:unknown,time:300440537,average:300440537,total:300440537,called:1
RapTime, 100185801,300463185,

trial:0-1
FunctionName:test1,ReturnFunctionName:test2,time:100285835,average:100288252,total:200576505,called:2
FunctionName:test1,ReturnFunctionName:unknown,time:100135124,average:100151235,total:200302470,called:2
FunctionName:test2,ReturnFunctionName:unknown,time:300456364,average:300448450,total:600896901,called:2
RapTime, 100172709,300475437,

trial:1-0
FunctionName:test1,ReturnFunctionName:test2,time:100302451,average:100292985,total:300878956,called:3
FunctionName:test1,ReturnFunctionName:unknown,time:100093515,average:100131995,total:300395985,called:3
FunctionName:test2,ReturnFunctionName:unknown,time:300622763,average:300506554,total:901519664,called:3
RapTime, 100129390,300642363,

trial:1-1
FunctionName:test1,ReturnFunctionName:test2,time:100300697,average:100294913,total:401179653,called:4
FunctionName:test1,ReturnFunctionName:unknown,time:100104336,average:100125080,total:400500321,called:4
FunctionName:test2,ReturnFunctionName:unknown,time:300607932,average:300531899,total:1202127596,called:4
RapTime, 100118650,300616491,

destruction
FunctionName:test1,ReturnFunctionName:test2,time:100300697,average:100294913,total:401179653,called:4
FunctionName:test1,ReturnFunctionName:test3,time:100104336,average:100125080,total:400500321,called:4
FunctionName:test2,ReturnFunctionName:test3,time:300607932,average:300531899,total:1202127596,called:4
FunctionName:test3,ReturnFunctionName:unknown,time:1603461561,average:1603461561,total:1603461561,called:1
RapTime, 100118650,300616491,
```
