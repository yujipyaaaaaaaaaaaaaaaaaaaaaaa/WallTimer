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
function name, test1, total, 1000343837, called, 1, average, 1000343837
function name, test2, total, 2000293891, called, 1, average, 2000293891

1000378579,2000306116,

trial:0-1
function name, test1, total, 2000620041, called, 2, average, 1000310020
function name, test2, total, 4000575296, called, 2, average, 2000287648

1000281843,2000287583,

trial:1-0
function name, test1, total, 1000293069, called, 1, average, 1000293069
function name, test2, total, 2000292105, called, 1, average, 2000292105

1000302716,2000303630,

trial:1-1
function name, test1, total, 2000574051, called, 2, average, 1000287025
function name, test2, total, 4000401777, called, 2, average, 2000200888

1000285296,2000115403,

```
