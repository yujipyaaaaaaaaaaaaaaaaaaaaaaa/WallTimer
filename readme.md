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
FunctionName, test1, total, 100335612, called, 1, average, 100335612
FunctionName, test2, total, 200290962, called, 1, average, 200290962
RapTime, 100371386,200303444,

trial:0-1
FunctionName, test1, total, 200479166, called, 2, average, 100239583
FunctionName, test2, total, 400583763, called, 2, average, 200291881
RapTime, 100149030,200299327,

trial:1-0
FunctionName, test1, total, 100300343, called, 1, average, 100300343
FunctionName, test2, total, 200284535, called, 1, average, 200284535
RapTime, 100310912,200296649,

trial:1-1
FunctionName, test1, total, 200580923, called, 2, average, 100290461
FunctionName, test2, total, 400494002, called, 2, average, 200247001
RapTime, 100286646,200216054,

destruction
FunctionName, test1, total, 200580923, called, 2, average, 100290461
FunctionName, test2, total, 400494002, called, 2, average, 200247001
RapTime, 100286646,200216054,
```
